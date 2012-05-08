#include "serial.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

__IO uint32_t TimingDelay;


Serial::Serial(int tx, int rx){
   __IO uint32_t i = 0;  
   uint8_t buf[255];
   uint8_t len;
   USBD_Init(&USB_OTG_dev,
    USB_OTG_FS_CORE_ID, 
    &USR_desc, 
    &USBD_CDC_cb, 
    &USR_cb);
};

void Serial::baud(int b) {

};

char Serial::getc() {
    uint8_t c;
    return VCP_get_char(&c);
};

void Serial::puts(char *buf) {
    VCP_send_str((uint8_t *)buf);
};


bool Serial::readable() {
    return false;
};


extern "C" {
    void EVAL_COM_IRQHandler() {

    }
}
