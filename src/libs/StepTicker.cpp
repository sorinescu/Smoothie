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
#include "StepTicker.h"

//STM Sepcific
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

StepTicker* global_step_ticker;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

//__IO uint16_t CCR1_Val = 54618;
uint16_t PrescalerValue = 0;
uint16_t capture = 0;

StepTicker::StepTicker(){

	global_step_ticker = this;
	NVIC_InitTypeDef NVIC_InitStructure;

	global_slow_ticker = this;

    // TIM3 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 65538;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//TODO:  I copied this from an example...need to review this.
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

    //Enable interrupt
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    //Enable Counter
    TIM_Cmd(TIM3, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

//    LPC_TIM0->MR0 = 1000000;        // Initial dummy value for Match Register
//    LPC_TIM0->MCR = 11;              // Match on MR0, reset on MR0, match on MR1
//    LPC_TIM0->TCR = 1;              // Enable interrupt
//    NVIC_EnableIRQ(TIMER0_IRQn);    // Enable interrupt handler
}

void StepTicker::set_frequency( double frequency ){
    this->frequency = frequency;
//    LPC_TIM0->MR0 = int(floor((SystemCoreClock/4)/frequency));  // SystemCoreClock/4 = Timer increments in a second
    TIM3->CCR1 = int(floor((double)(SystemCoreClock/4)/frequency));  // SystemCoreClock/4 = Timer increments in a second
}

void StepTicker::set_reset_delay( double seconds ){
//    LPC_TIM0->MR1 = int(floor(double(SystemCoreClock/4)*( seconds )));  // SystemCoreClock/4 = Timer increments in a second
}

void StepTicker::tick(){
    for (int i=0; i<this->hooks.size(); i++){ 
        this->hooks.at(i)->call();
    }
}

void StepTicker::reset_tick(){
    for (int i=0; i<this->reset_hooks.size(); i++){ 
        this->reset_hooks.at(i)->call();
    }
}

extern "C" void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		global_step_ticker->tick();
	}

	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		global_step_ticker->reset_tick();
	}

//    if((LPC_TIM0->IR >> 0) & 1){  // If interrupt register set for MR0
//        LPC_TIM0->IR |= 1 << 0;   // Reset it
//        global_step_ticker->tick();
//    }
//    if((LPC_TIM0->IR >> 1) & 1){  // If interrupt register set for MR1
//        LPC_TIM0->IR |= 1 << 1;   // Reset it
//        global_step_ticker->reset_tick();
//    }
}


