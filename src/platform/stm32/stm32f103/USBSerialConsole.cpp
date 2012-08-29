#include "platform/Platform.h"

extern "C" {

/*******************************************************************************
* Function Name  : USB_On_Data_Recv.
* Description    : send the received data from USB to the recipient.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_On_Data_Recv(uint8_t* data_buffer, uint8_t Nb_bytes)
{
  
  uint32_t i;
  
  for (i = 0; i < Nb_bytes; i++)
  {
//    USART_SendData(EVAL_COM1, *(data_buffer + i));
//    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET); 
  }  
}

} // extern "C"
