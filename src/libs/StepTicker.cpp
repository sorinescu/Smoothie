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
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;
    
    NVIC_InitTypeDef NVIC_InitStructure;
	
    global_step_ticker = this;
	
    // TIM3 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Since we are dealing with the 48MHz domain for RCC_APB1 - we should be 2x which is 84MHz.  We can get there 
    // from our system clock of 168MHz / 2
    PrescalerValue = 0;
    TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
    TIM_TimeBaseStructure.TIM_Period = 10000; // default to 1Hz
    TIM3->CCR2 = 5000;

    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //Enable interrupt
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 , ENABLE);
    
    //Enable Counter
    TIM_Cmd(TIM3, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
}

void StepTicker::set_frequency( double frequency ){
    this->frequency = frequency;
    TIM_Cmd(TIM3, DISABLE);
    TIM3->ARR = (SystemCoreClock/2)/frequency;
    TIM3->CCR1 = TIM3->ARR - 1;
    TIM_Cmd(TIM3, ENABLE);
}

void StepTicker::set_reset_delay( double seconds ){
    TIM_Cmd(TIM3, DISABLE);
    TIM3->ARR = (SystemCoreClock/2)/this->frequency;
    double d_tmp = seconds * 84000000.0;
    int delay_val = int(floor(d_tmp));
    TIM3->CCR1 = TIM3->ARR - 1;
    TIM3->CCR2 = delay_val;
    TIM_Cmd(TIM3, ENABLE);
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	    global_step_ticker->tick();
    }

	if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    	global_step_ticker->reset_tick();
    }
}


