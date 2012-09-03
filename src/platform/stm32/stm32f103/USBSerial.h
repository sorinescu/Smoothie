/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USB_SERIAL_H_
#define _USB_SERIAL_H_

#include "libs/FPointer.h"

// GCC strangeness
#ifdef getc
#undef getc
#endif

class Serial
{
public:
    Serial(int tx, int rx);

    static Serial* g_Serial;

    void baud(int);

    char getc();
    void puts(char *);

    template<typename T>
    void attach(T* tptr, uint32_t (T::*mptr)(uint32_t)) {
        if ((mptr != NULL) && (tptr != NULL))
        {
            callbackFptr = new FPointer();
            callbackFptr->attach(tptr, mptr);
        }
    }

    // readable - says if getc() will return anything, I think
    bool readable();

    void on_char_received(char c);

protected:
    FPointer *callbackFptr;
    bool hasChar;
    char ch;
};

#endif  // _USB_SERIAL_H_
