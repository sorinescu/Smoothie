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

SlowTicker* global_slow_ticker;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

SlowTicker::SlowTicker(){
    this->max_frequency = 1;
    global_slow_ticker = this;
    // Enable timer 2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //Enable interupts
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);



//    LPC_SC->PCONP |= (1 << 22);     // Power Ticker ON
//    LPC_TIM2->MR0 = 10000;        // Initial dummy value for Match Register
//    LPC_TIM2->MCR = 3;              // Match on MR0, reset on MR0
//    LPC_TIM2->TCR = 1;              // Enable interrupt
//    NVIC_EnableIRQ(TIMER2_IRQn);    // Enable interrupt handler

}

void SlowTicker::set_frequency( int frequency ){
//    LPC_TIM2->MR0 = int(floor((SystemCoreClock/4)/frequency));  // SystemCoreClock/4 = Timer increments in a second
//    LPC_TIM2->TCR = 3;  // Reset
//    LPC_TIM2->TCR = 1;  // Reset

	TIM_Cmd(TIM3, ENABLE);
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

extern "C" void TIMER2_IRQHandler (void){
//    if((LPC_TIM2->IR >> 0) & 1){  // If interrupt register set for MR0
//        LPC_TIM2->IR |= 1 << 0;   // Reset it
//        global_slow_ticker->tick();
//    }
}

