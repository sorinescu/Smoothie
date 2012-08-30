/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform/Platform.h"

#if SMOOTHIE_HAS_SERIAL_CONSOLE

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

    for (i = 0; i < Nb_bytes; i++)
    {
//    USART_SendData(EVAL_COM1, *(data_buffer + i));
//    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET); 
    }  
}

Serial* Serial::g_Serial = 0;

Serial::Serial(int tx, int rx)
{
    Serial::g_Serial = this;
};

void Serial::baud(int b) 
{
    SMT_ASSERT(0);  // TODO implement
};

char Serial::getc() 
{
    SMT_ASSERT(0);  // TODO implement
    return 0;
};

void Serial::puts(char *buf) 
{
    SMT_ASSERT(0);  // TODO implement
};


bool Serial::readable() 
{
    return true;
};


#endif // SMOOTHIE_HAS_SERIAL_CONSOLE

