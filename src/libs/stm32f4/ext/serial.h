#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <cstdio>

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usbd_conf.h"



class Serial {
	public:
		Serial(int tx, int rx);
		
		void * callbackObject;
		typedef void (<typename T>::*callback_fn)();

		void baud(int);

		char getc();
		void puts(char *);
		
		// this->serial->attach(this, &SerialConsole::on_serial_char_received);
		// void attach(void *called, void (*fptr)());

		template<typename T>
    	void attach(T* tptr, void (T::*mptr)(void)) {
	        if((mptr != NULL) && (tptr != NULL)) {
	            
	        }
    	};

		
		// readable - says if getc() will return anything, I think
		bool readable();

		void handleCallbacks();

		static Serial *g_Serial;

};

extern "C" void Serial_memberFn_wrapper();

#endif
