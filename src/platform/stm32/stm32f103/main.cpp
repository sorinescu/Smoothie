/*  
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>. 
*/

#include "platform/Platform.h"

#include "modules/robot/Player.h"
#include "modules/utils/simpleshell/SimpleShell.h"
#include "modules/utils/configurator/Configurator.h"
#include "modules/utils/currentcontrol/CurrentControl.h"
#include "modules/utils/pausebutton/PauseButton.h"
#include "libs/Config.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"

extern "C" {
#include "glue/hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
}

enum StepMode {
    StepMode_FullStep,
    StepMode_HalfStep,
    StepMode_MicroStep_8,
    StepMode_MicroStep_16
};

static void CNC_SetStepMode(enum StepMode mode, Pin *m1, Pin *m2)
{
    switch (mode) {
    case StepMode_FullStep:
        m2->set(false);
        m1->set(false);
        break;
    case StepMode_HalfStep:
        m2->set(false);
        m1->set(true);
        break;
    case StepMode_MicroStep_8:
        m2->set(true);
        m1->set(false);
        break;
    case StepMode_MicroStep_16:
        m2->set(true);
        m1->set(true);
        break;
    }
}

// This is only used during initialization.
// The timer is taken over by Kernel after it's constructed.
static void CNC_StartTmpTimer(void)
{
    // Enable timer clock - use TIMER4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // The timer clock is calculated like this (assuming a SYSCLK of 72MHz):
    // - if prescaler is 1 (TIM_Prescaler = 0), timer clock is 36MHz
    // - if prescaler is >1, timer clock is 72MHz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / DELAY_TIM_FREQUENCY) - 1;
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* Enable counter */
    TIM_Cmd(TIM4, ENABLE);
}

// This is only used during initialization.
// The timer is taken over by Kernel after it's constructed.
static inline void CNC_DelayUS(uint16_t us)
{
    volatile uint16_t start = TIM4->CNT;
    while ((TIM4->CNT - start) <= us) ;
}

static void CNC_Init(void)
{
    /* Initialize TB6560AHQ pins and start it up. From the Toshiba docs:
     
    Turn on VDD. Then, when the VDD voltage has stabilized, turn on VMA/B.
    Hold the control input pins Low while turning on VDD and VMA/B.
    (All the control input pins are internally pulled down.)
    After VDD and VMA/B completely stabilizes at the rated voltages, the RESET and ENABLE pins can be
    set High. If this sequence is not properly followed, the IC may not operate correctly, or the IC and the
    peripheral parts may be damaged.
    When RESET is released High, the CLK signal is applied and excitation is started. Only after ENABLE
    is also set High, outputs are enabled. When only RESET is set High, outputs are disabled and only the
    internal counter advances. Likewise, when only ENABLE is set High, the excitation will not be performed
    even if the CLK signal is applied and the outputs will remain in the initial state.
    A power-off sequence should be the reverse of this sequence. */

    Pin xyz_enable_pin(XYZ_ENABLE_PIN);
    Pin xyz_reset_pin(XYZ_RESET_PIN);

    Pin x_vm_switch_pin(X_VM_SWITCH_PIN);
    Pin y_vm_switch_pin(Y_VM_SWITCH_PIN);
    Pin z_vm_switch_pin(Z_VM_SWITCH_PIN);

    Pin x_m1_pin(X_M1_PIN);
    Pin x_m2_pin(X_M2_PIN);
    Pin y_m1_pin(Y_M1_PIN);
    Pin y_m2_pin(Y_M2_PIN);
    Pin z_m1_pin(Z_M1_PIN);
    Pin z_m2_pin(Z_M2_PIN);

    // First set output register, then configure port as output
    // (to avoid transient 1's at output).
    // This sequence puts driver chip in initial mode.
    xyz_enable_pin.set(false);
    xyz_enable_pin.as_output();
    xyz_reset_pin.set(true);
    xyz_reset_pin.as_output();

    x_m1_pin.as_output();
    x_m2_pin.as_output();
    y_m1_pin.as_output();
    y_m2_pin.as_output();
    z_m1_pin.as_output();
    z_m2_pin.as_output();

    // Half-step mode on all axes
    CNC_SetStepMode(StepMode_HalfStep, &x_m1_pin, &x_m2_pin);
    CNC_SetStepMode(StepMode_HalfStep, &y_m1_pin, &y_m2_pin);
    CNC_SetStepMode(StepMode_HalfStep, &z_m1_pin, &z_m2_pin);

    // Turn on motor relays and apply voltage to VM pins
    x_vm_switch_pin.as_output()->set(true);
    y_vm_switch_pin.as_output()->set(true);
    z_vm_switch_pin.as_output()->set(true);

    // Initialize temporary timer.
    // TODO: should use Kernel timer
    CNC_StartTmpTimer();

    // Wait a while (60ms)
    CNC_DelayUS(60000);

    // The motor relays must be stable by now.
    // Enable chip.
    xyz_reset_pin.set(false);
    xyz_enable_pin.set(true);
}

#include "usb_pwr.h"
extern "C"
void USB_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes);

int main() {
    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    CNC_Init();

    // Wait for USB to stabilize
    while (bDeviceState != CONFIGURED) ;

    Kernel* kernel = new Kernel();

    kernel->streams->printf("Smoothie ( grbl port ) version 0.6.1 \r\n");

    Configurator* conf = new Configurator();
    kernel->add_module( conf );

    // Force loading of configuration
    conf->config_load_command("", 0);
  
    kernel->streams->printf("start\r\n");

    while(1){
        kernel->call_event(ON_MAIN_LOOP);
        kernel->call_event(ON_IDLE);
        //kernel->streams->printf("ugabugabuga\r\n");
    }
}

