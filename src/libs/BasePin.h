#ifndef BASE_PIN_H
#define BASE_PIN_H

#include "platform/Platform.h"

typedef int16_t PinDescAsInt;
#define PIN_DESC_TO_INT(inverting, port, pin) PinDescAsInt(inverting ? -((port) << 8 + (pin)) : ((port) << 8 + (pin)))

struct PinDesc {
    static PinDesc& init(PinDesc& desc, const smt_string& value) {
        desc.port_number = atoi(value.substr(0,1).c_str());
        desc.inverting = ( value.find_first_of("!")!=smt_string::npos ? true : false );
        desc.pin = atoi( value.substr(2, value.size()-2-(desc.inverting?1:0)).c_str() );
        return desc;
    }
    static PinDesc& init(PinDesc& desc, PinDescAsInt value) {
        if (value < 0) {
            desc.inverting = true;
            value = -value;
        }
        else
            desc.inverting = false;

        desc.port_number = value >> 8;
        desc.pin = value & 0xff;

        return desc;
    }

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
protected:
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
