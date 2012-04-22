#ifndef MY_SERIAL_H
#define MY_SERIAL_H
#include <cstdio>
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usbd_conf.h"

//#include <stdarg.h>
//
class Serial {
	public:
		Serial(int tx, int rx);
		
		char getc();
		void puts(char *);
		
		// this->serial->attach(this, &SerialConsole::on_serial_char_received, Serial::IrqType::RxIrq);
		// readable - says if getc() will return anything, I think


};
#endif
