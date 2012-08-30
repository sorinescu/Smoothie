/*
      Configuration file for the Smoothie project.                                                                                                                                                                                                                                          .
                                                                                                                                                                                                                                                                                            .
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __config_h__
#define __config_h__

// Valid platforms
#define SMOOTHIE_PLATFORM_LPC17XX       1
#define SMOOTHIE_PLATFORM_STM32F4XX     2
#define SMOOTHIE_PLATFORM_STM32F103     3

// These values are documented in ../Makefile.
// If you're using Keil MDK instead of GCC, you must set them manually in config.h (NOT config.h.in).

#define SMOOTHIE_PLATFORM SMOOTHIE_PLATFORM_STM32F103
//#define SMOOTHIE_USE_STL
//#define SMOOTHIE_USE_FILES
//#define SMOOTHIE_HAS_CONFIG_VALUE_STRING

#endif

