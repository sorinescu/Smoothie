/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform/Platform.h"

#if !SMOOTHIE_USE_STL   //EASTL

// EASTL expects us to define these, see allocator.h line 194 
void* operator new[](size_t size, const char* pName, int flags,
             unsigned debugFlags, const char* file, int line)
{
  return malloc(size);
}
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset,
    const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
  // this allocator doesn't support alignment
  EASTL_ASSERT(alignment <= 8);
  return malloc(size);
}

// EASTL also wants us to define this (see string.h line 197)
int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments)
{
#ifdef _MSC_VER
  return _vsnprintf(pDestination, n, pFormat, arguments);
#else
  return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}

#endif // SMOOTHIE_USE_STL

