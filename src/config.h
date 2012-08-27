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

/****************************************************************************************
 *                              Start of user customizations                            *
 ****************************************************************************************/

// Actual platform used by Smoothie
#define SMOOTHIE_PLATFORM		SMOOTHIE_PLATFORM_STM32F103

// Set this to 1 if you prefer to use STL classes (like std::string and std::vector)
// instead of EASTL ones. STL has a higher overhead, but may be more stable on some
// platforms.
// By default, EASTL is used (smaller footprint).
//#define SMOOTHIE_USE_STL 1

// Set this to 1 if you need to use files (e.g. config files, SD card etc), 0 otherwise.
// If not defined here, it will have the default value for the platform (see Platform.h).
//#define SMOOTHIE_USE_FILES 1

/****************************************************************************************
 *                               End of user customizations                             *
 ****************************************************************************************/

#endif

