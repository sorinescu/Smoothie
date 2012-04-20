#ifndef MY_SERIAL_H
#define MY_SERIAL_H
#include <cstdio>
//#include <stdarg.h>
//
class Serial {
	public:
		Serial(int tx, int rx);
		int putc_(int c);
		char getc();
};
#endif
