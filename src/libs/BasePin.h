#ifndef BASE_PIN_H
#define BASE_PIN_H

#include "platform/Platform.h"

struct PinDesc {
    PinDesc() : inverting(false), pin(0), port_number(0) {}
    PinDesc(char aport_number, char apin, bool ainverting = false) :
        inverting(ainverting), pin(apin), port_number(aport_number) {}

#if SMOOTHIE_USE_STRINGS
    PinDesc(const smt_string& value){
        port_number = atoi(value.substr(0,1).c_str());
        inverting = ( value.find_first_of("!")!=smt_string::npos ? true : false );
        pin = atoi( value.substr(2, value.size()-2-(this->inverting?1:0)).c_str() );
    }
#endif

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
