/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLATFORM_KERNEL_H
#define PLATFORM_KERNEL_H

//STM Specific
#if SMOOTHIE_PLATFORM == SMOOTHIE_PLATFORM_STM32F103
    #include "stm32f10x.h"
#elif SMOOTHIE_PLATFORM == SMOOTHIE_PLATFORM_STM32F4XX
    #include "stm32f4xx.h"
#else
    #error "Unsupported SMOOTHIE_PLATFORM"
#endif


#define DELAY_TIM_FREQUENCY 1000000 /* = 1MHZ -> timer runs in microseconds */

class PlatformKernel
{
public:
    void delay_us( uint16_t uSecs ) {
        volatile uint16_t start = TIM4->CNT;
        while ((TIM4->CNT - start) <= uSecs) ;
    };

protected:
    void init_platform() {
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
};

#endif


