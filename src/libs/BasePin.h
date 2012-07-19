#ifndef BASE_PIN_H
#define BASE_PIN_H

//#include "mbed.h" //Required for LPC_GPIO* . can probably be found in one othe the files mbed.h includes. TODO
//#include "../gcc4mbed/external/mbed/LPC1768/LPC17xx.h"
#include "platform/Platform.h"
#include "libs/BasePin.h"
#include "libs/utils.h"
#include <string>
#include <cmath>
#include <cstdlib>

struct PinDesc {
    PinDesc() : inverting(false), pin(0), port_number(0) {}
    PinDesc(char aport_number, char apin, bool ainverting = false) :
        inverting(ainverting), pin(apin), port_number(aport_number) {}

    bool inverting;
    char pin;
    char port_number;
};

/*
  Base class for PlatformPin.
  All PlatformPin implementations must implement these methods:

      PlatformPin(const PinDesc &desc);
      Pin* as_output();
      Pin* as_input();
      bool get();
      void set(bool value);
 */
class BasePin {
public:
    BasePin(const PinDesc &desc) {
        inverting = desc.inverting;
        port_number = desc.port_number;
        pin = desc.pin;
    }

    bool inverting;
    char pin;
    char port_number;
};

#endif
