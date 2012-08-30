#ifndef PIN_H
#define PIN_H

#include "platform/Platform.h"

class Pin : public PlatformPin {
public:
    Pin(PinAsUint16 pin) : PlatformPin(pin) {}
};

#endif
