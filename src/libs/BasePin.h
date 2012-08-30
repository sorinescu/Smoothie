#ifndef BASE_PIN_H
#define BASE_PIN_H

#include "platform/Platform.h"

typedef uint16_t PinAsUint16;

#define PIN_INVERTING true
#define PIN_NON_INVERTING false

// Not Connected
#define PIN_NC PinAsUint16(0xffff)

// Pin description as int16
#define PIN(inverting, port_nr, pin) PinAsUint16((inverting ? 0x8000 : 0) | (((port_nr) & 0x7f) << 8) | ((pin) & 0xff))

#define PIN_IS_INVERTING(pin) ((pin) & 0x8000 != 0)
#define PIN_PORT_NR(pin) (((pin) >> 8) & 0x7f)
#define PIN_PIN(pin) ((pin) & 0xff)

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
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
    static PinAsUint16 from_str(const smt_string& value) {
        if (value.compare("nc") == 0)
            return PIN_NC;

        int port_number = atoi(value.substr(0,1).c_str());
        bool inverting = (value.find_first_of("!") != smt_string::npos);
        int pin = atoi(value.substr(2, value.size()-2-(inverting?1:0)).c_str());

        return PIN(inverting, port_number, pin);
    }
#endif

protected:
    BasePin(PinAsUint16 pin) {
        if (pin == PIN_NC) {
            connected   = false;
            port_number = 0xff;    // mainly for debug - large, invalid value
        }
        else {
            connected   = true;
            inverting   = PIN_IS_INVERTING(pin);
            port_number = PIN_PORT_NR(pin);
            pin         = PIN_PIN(pin);
        }
    }

    bool connected;
    bool inverting;
    char pin;
    char port_number;
};

#endif
