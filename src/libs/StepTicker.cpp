/*  
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>. 
*/

using namespace std;
#include <vector>
#include "libs/nuts_bolts.h"
#include "libs/Module.h"
#include "libs/Kernel.h"
#include "SlowTicker.h"
#include "libs/Hook.h"

//STM Sepcific
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

//REMOVE
#include "stm32f4_discovery.h"

StepTicker* global_step_ticker;

StepTicker::StepTicker(){

    // STM_EVAL_LEDInit(LED5);
    // STM_EVAL_LEDInit(LED6);
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;
    
    NVIC_InitTypeDef NVIC_InitStructure;
	
    global_step_ticker = this;
	
    // TIM3 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Since we are dealing with the 48MHz domain for RCC_APB1 - we should be 2x which is 84MHz.  We can get there 
    // from our system clock of 168MHz / 2
    // PrescalerValue = 10000;
    PrescalerValue = 0;
    TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
    // TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 2; // default to 1Hz
    // TIM_TimeBaseStructure.TIM_Period = 100000; // default to 1Hz
    TIM_TimeBaseStructure.TIM_Period = 10000; // default to 1Hz
    // TIM3->CCR1 = 8399;
    TIM3->CCR2 = 5000;

    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // TIM3->CCR3 = 3000;

    //Enable interrupt
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 , ENABLE);
    
    //Enable Counter
    TIM_Cmd(TIM3, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
}

void StepTicker::set_frequency( double frequency ){
    // this->frequency = frequency * 8400;
    this->frequency = frequency;
    TIM_Cmd(TIM3, DISABLE);
    TIM3->ARR = (SystemCoreClock/2)/frequency;
    TIM_Cmd(TIM3, ENABLE);
}

void StepTicker::set_reset_delay( double seconds ){
//    LPC_TIM0->MR1 = int(floor(double(SystemCoreClock/4)*( seconds )));  // SystemCoreClock/4 = Timer increments in a second
    // TIM3->CCR2 = int( floor( double( ( ((SystemCoreClock/2)/10000) * seconds ) ));
    TIM_Cmd(TIM3, DISABLE);
    TIM3->ARR = (SystemCoreClock/2)/this->frequency;
    double d_tmp = seconds * 84000000.0;
    int delay_val = int(floor(d_tmp));
    TIM3->CCR2 = delay_val;
    TIM_Cmd(TIM3, ENABLE);

}

void StepTicker::tick(){
    int tmp_cnt = TIM3->CNT;
    for (int i=0; i<this->hooks.size(); i++){ 
        this->hooks.at(i)->call();
    }
    tmp_cnt = TIM3->CNT;
}

void StepTicker::reset_tick(){
    int tmp_cnt = TIM3->CNT;
    for (int i=0; i<this->reset_hooks.size(); i++){ 
        this->reset_hooks.at(i)->call();
    }
    tmp_cnt = TIM3->CNT;
}

extern "C" void TIM3_IRQHandler(void){
    // global_step_ticker->kernel->serial->printf("IRQ hit at count: %u\r\n", TIM3->CNT);

	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
        // global_step_ticker->kernel->serial->printf(" CC1 hit at count: %u\r\n", TIM3->CNT);
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		global_step_ticker->tick();
        // TIM_SetCounter(TIM3, 0);
        // STM_EVAL_LEDToggle(LED5);
        // STM_EVAL_LEDToggle(LED6);
	}

	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
        // global_step_ticker->kernel->serial->printf(" CC2 hit at count: %u\r\n", TIM3->CNT);
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		global_step_ticker->reset_tick();
		// TIM_SetCounter(TIM3, 0);
        // STM_EVAL_LEDToggle(LED6);
	}
}


