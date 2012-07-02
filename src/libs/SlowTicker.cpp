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

SlowTicker* global_slow_ticker;

SlowTicker::SlowTicker(){
//	REMOVE LED Stuff - just for testing 

	// STM_EVAL_LEDInit(LED4);
	// STM_EVAL_LEDInit(LED3);
 //    STM_EVAL_LEDToggle(LED4);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

	NVIC_InitTypeDef NVIC_InitStructure;

    // What does this do???
	this->max_frequency = 1;
    global_slow_ticker = this;

    // TIM2 clock enable - we are using RCC for TIM2 which is an internal clock of 48MHz I believe
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Since we are dealing with the 48MHz domain for RCC_APB1 - we should be 2x which is 84MHz.  We can get there 
    // from our system clock of 168MHz / 2
    // PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 100000) - 1; //  This + a TIM_Period of 100000 makes it tick every second.
    TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
    TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 2; // default to 1Hz
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //Enable interrupt
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    //Enable Counter
    TIM_Cmd(TIM2, ENABLE);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);


}

void SlowTicker::set_frequency( int frequency ){
    // Take from st docs:
    // The update event period is calculated as follows:
    // Update_event = TIM_CLK/((PSC + 1)*(ARR + 1)*(RCR + 1))

    // TIM_CLK = 72 MHz
    // Prescaler = 1
    // Auto reload = 65535
    // No repetition counter RCR = 0
    // Update_event = 72*106/((1 + 1)*(65535 + 1)*(1))
    // Update_event = 549.3 Hz
    // Where: TIM_CLK = timer clock input
    // PSC = 16-bit prescaler register
    // ARR = 16/32-bit Autoreload register
    // RCR = 16-bit repetition counter
    
    // TODO - do I need to set this->frequency?
    TIM_Cmd(TIM2, DISABLE);
    TIM2->ARR = (SystemCoreClock/2)/frequency;
    TIM_Cmd(TIM2, ENABLE);
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
		
        // TIM_SetCounter(TIM2, 0);

		// STM_EVAL_LEDToggle(LED4);
		// STM_EVAL_LEDToggle(LED3);
//		capture = TIM_GetCapture1(TIM2);
//		TIM_SetCompare1(TIM2, capture + );
	}
//Original Smoothie code:
//    if((LPC_TIM2->IR >> 0) & 1){  // If interrupt register set for MR0
//        LPC_TIM2->IR |= 1 << 0;   // Reset it
//        global_slow_ticker->tick();
//    }
}

