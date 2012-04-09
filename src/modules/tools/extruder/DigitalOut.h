//#include "PinNames.h"
#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

class DigitalOut {

public:

    /* Constructor: DigitalOut
     *  Create a DigitalOut connected to the specified pin
     *
     * Variables:
     *  pin - DigitalOut pin to connect to
     */
    DigitalOut(PinName pin, const char* name = NULL);

};

#endif
