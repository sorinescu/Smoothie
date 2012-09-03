/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform/Platform.h"

#if SMOOTHIE_HAS_SERIAL_CONSOLE

#include "libs/nuts_bolts.h"

Serial* Serial::g_Serial = 0;

extern "C" 
void USB_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes);

/*******************************************************************************
* Function Name  : USB_On_Data_Recv.
* Description    : send the received data from USB to the recipient.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
extern "C" 
void USB_On_Data_Recv(uint8_t* data_buffer, uint8_t Nb_bytes)
{
    uint32_t i;

    if (!Serial::g_Serial)
        return;

    for (i = 0; i < Nb_bytes; i++)
        Serial::g_Serial->on_char_received((char)data_buffer[i]);
}

Serial::Serial(int tx, int rx)
{
    hasChar = false;
    Serial::g_Serial = this;
}

void Serial::baud(int b) 
{
    // TODO implement
}

void Serial::on_char_received(char c)
{
    SMT_ASSERT(!hasChar);

    ch = c;
    hasChar = true;

    // consume char
    g_Serial->callbackFptr->call(0);

    SMT_ASSERT(!hasChar);   // must be cleared by getc
}

char Serial::getc() 
{
    SMT_ASSERT(hasChar);

    if (hasChar) {
        hasChar = false;
        return ch;
    }
    else
        return 0;
}

void Serial::puts(char *buf) 
{
    int len = strlen(buf);
    while (len) {
        uint8_t l = (uint8_t)min(255,len);

        USB_Send_Data((uint8_t*)buf, l);
        len -= l;
        buf += l;
    }
}

bool Serial::readable() 
{
    return hasChar;
}

#endif // SMOOTHIE_HAS_SERIAL_CONSOLE

