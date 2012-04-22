#include "serial.h"

// int Serial::putc(int c) {
// 	VCP_put_char((char)c);
// };

char Serial::getc() {
	uint8_t c;
	return VCP_get_char(&c);
};

void Serial::puts(char *buf) {
	VCP_send_str((uint8_t *)buf);
};

