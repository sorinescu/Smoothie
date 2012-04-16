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

SlowTicker* global_slow_ticker;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

//__IO uint16_t CCR1_Val = 54618;
uint16_t PrescalerValue = 0;
uint16_t capture = 0;

SlowTicker::SlowTicker(){
	NVIC_InitTypeDef NVIC_InitStructure;

	this->max_frequency = 1;
    global_slow_ticker = this;

    // TIM2 clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 65538;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//TODO:  I copied this from an example...need to review this.
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;
	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

    //Enable interrupt
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    //Enable Counter
    TIM_Cmd(TIM2, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

}

void SlowTicker::set_frequency( int frequency ){
	//Not sure if I should be casting this to double...
	TIM2->CCR1 = int(floor((double)(SystemCoreClock/4)/frequency));  // SystemCoreClock/4 = Timer increments in a second
//    LPC_TIM2->MR0 = int(floor((SystemCoreClock/4)/frequency));  // SystemCoreClock/4 = Timer increments in a second
//    LPC_TIM2->TCR = 3;  // Reset
//    LPC_TIM2->TCR = 1;  // Reset
}

void SlowTicker::tick(){
    for (int i=0; i<this->hooks.size(); i++){
        Hook* hook = this->hooks.at(i);
        hook->counter += ( hook->frequency / this->max_frequency );
        if( hook->counter > 0 ){
            hook->counter-=1;
            hook->call();
        }
    }
}

extern "C" void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		global_slow_ticker->tick();
		capture = TIM_GetCapture1(TIM2);

//		TIM_SetCompare1(TIM2, capture + );
	}
//Original Smoothie code:
//    if((LPC_TIM2->IR >> 0) & 1){  // If interrupt register set for MR0
//        LPC_TIM2->IR |= 1 << 0;   // Reset it
//        global_slow_ticker->tick();
//    }
}

