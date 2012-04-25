#include "serial.h"


char Serial::getc() {
	uint8_t c;
	return VCP_get_char(&c);
};

void Serial::puts(char *buf) {
	VCP_send_str((uint8_t *)buf);
};


void Serial::attach(void *called, int (*fptr)(void) ){

};