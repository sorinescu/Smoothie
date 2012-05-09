#include "serial.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


static Serial * Serial::g_Serial = 0;

Serial::Serial(int tx, int rx){
   Serial::g_Serial = this;
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

    };

    void g_Serial_callback() {
      Serial::g_Serial->callbackFptr->call(0);
    }
}
