#ifndef PIN_H
#define PIN_H

//#include "mbed.h" //Required for LPC_GPIO* . can probably be found in one othe the files mbed.h includes. TODO
//#include "../gcc4mbed/external/mbed/LPC1768/LPC17xx.h"
#include "platform/Platform.h"

class Pin : public PlatformPin {
    Pin(const PinDesc &desc) : PlatformPin(desc) {}
};

#endif
