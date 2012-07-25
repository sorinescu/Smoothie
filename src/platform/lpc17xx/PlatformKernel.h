/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLATFORM_KERNEL_H
#define PLATFORM_KERNEL_H

#include "libs/Adc.h"
// #include "libs/Digipot.h"

class PlatformKernel {
protected:
    Adc* adc;
    // Digipot* digipot;

    inline void init_platform() {
        // HAL stuff
        this->adc = new Adc();

        // LPC17xx-specific
        // This is the stepper interrupt
        NVIC_SetPriority(TIM3_IRQn, 1);
        // this is the slow interrupt
        NVIC_SetPriority(TIM2_IRQn, 2);
    }
};

#endif

