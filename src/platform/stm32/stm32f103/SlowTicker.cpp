/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/nuts_bolts.h"
#include "libs/Module.h"
#include "libs/Kernel.h"
#include "libs/SlowTicker.h"
#include "libs/Hook.h"

SlowTicker* global_slow_ticker;

#define MAX_SLOW_TICKER_FREQ 10000

SlowTicker::SlowTicker(){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    max_frequency = 1;
    global_slow_ticker = this;

    // TIM2 clock enable - we are using RCC for TIM2 which is an internal clock of 36MHz
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // The timer clock is calculated like this (assuming a SYSCLK of 72MHz):
    // - if prescaler is 1 (TIM_Prescaler = 0), timer clock is 36MHz
    // - if prescaler is >1, timer clock is 72MHz
    // We want a maximum possible frequency of MAX_SLOW_TICKER_FREQ (prescaler > 1)
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / MAX_SLOW_TICKER_FREQ) - 1;
    TIM_TimeBaseStructure.TIM_Period = MAX_SLOW_TICKER_FREQ / max_frequency;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //Enable interrupt
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    //Enable Counter
    TIM_Cmd(TIM2, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
}

void SlowTicker::set_frequency( uint16_t frequency ){
    TIM_Cmd(TIM2, DISABLE);
    TIM2->ARR = MAX_SLOW_TICKER_FREQ/frequency;
    TIM_Cmd(TIM2, ENABLE);
}

void SlowTicker::tick(){
    for (int i=0; i<this->hooks.size(); i++)
    {
        Hook* hook = this->hooks.at(i);
        hook->counter += hook->frequency;
        if (hook->counter > max_frequency)
        {
            hook->counter-=max_frequency;
            hook->call();
        }
    }
}

extern "C" void TIM2_IRQHandler(void){
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        global_slow_ticker->tick();
    }
}

