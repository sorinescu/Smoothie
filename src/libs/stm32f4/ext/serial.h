#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <cstdio>

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usbd_conf.h"

#include "fpointer.h"

class Serial {
	public:
		Serial(int tx, int rx);
		FPointer *callbackFptr;

		static Serial* g_Serial;
		
		void baud(int);

		char getc();
		void puts(char *);
		
		template<typename T>
    	void attach(T* tptr, uint32_t (T::*mptr)(uint32_t)) {
	        if((mptr != NULL) && (tptr != NULL)) {
	            callbackFptr = new FPointer();
	            callbackFptr->attach(tptr, mptr);
	        }
    	};
		
		// readable - says if getc() will return anything, I think
		bool readable();


};

extern "C" void g_Serial_callback();

#endif
