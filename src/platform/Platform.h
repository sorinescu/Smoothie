/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "config.h"

/*
    Basic checks and default values.
 */

#ifndef SMOOTHIE_PLATFORM
#error "SMOOTHIE_PLATFORM must be defined !"
#endif

// Use EASTL by default
#ifndef SMOOTHIE_USE_STL
#define SMOOTHIE_USE_STL 0
#endif

#ifndef SMOOTHIE_USE_FILES
#define SMOOTHIE_USE_FILES 0
#endif

#ifndef SMOOTHIE_HAS_CONFIG_VALUE_STRING
// Enable by default when files are also enabled
#define SMOOTHIE_HAS_CONFIG_VALUE_STRING SMOOTHIE_USE_FILES
#endif

/*
    Platform-specific configuration.
 */
 
#ifdef __cplusplus

#if SMOOTHIE_USE_STL
#include <string>
#include <vector>

typedef std::string smt_string;

template <typename T>
struct smt_vector
{
    typedef std::vector<T> type;
};

#else   // SMOOTHIE_USE_STL

// EASTL needs to be configured for the platform first.
// We only support 32bit ARM (little endian, 4 byte alignment).
#define EA_PLATFORM_LINUX
#define EA_SYSTEM_LITTLE_ENDIAN
#define EASTL_ALIGN_OF(type) 4

#include "EASTL/string.h"
#include "EASTL/vector.h"

typedef eastl::string smt_string;

template <typename T>
struct smt_vector
{
    typedef eastl::vector<T> type;
};

#endif  // SMOOTHIE_USE_STL

#endif // __cplusplus

#if SMOOTHIE_PLATFORM == SMOOTHIE_PLATFORM_STM32F103

// CPU type
#ifndef STM32F10X_LD
#error "STM32F10X_LD not defined; please define it in the project preprocessor flags !"
#endif

// Include file from glue code, which includes required peripheral headers
#include "stm32f10x_conf.h"

#ifdef  USE_FULL_ASSERT
  #define SMT_ASSERT(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
  #define SMT_ASSERT(expr) ((void)0)
#endif

#ifdef __cplusplus
#include "stm32/PlatformKernel.h"
#include "stm32/stm32f103/PlatformPin.h"
#endif // __cplusplus

#else   // SMOOTHIE_PLATFORM_STM32F103
#error "Unsupported SMOOTHIE_PLATFORM"
#endif
 
#endif  // _PLATFORM_H_
