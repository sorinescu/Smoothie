#ifndef PAUSEBUTTON_H
#define PAUSEBUTTON_H

#include "libs/Kernel.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/Pin.h"

#define pause_button_pin_checksum 32709
#define pause_led_pin_checksum    48477

class PauseButton : public Module {
    public:
        PauseButton();
       
        virtual void on_module_loaded();
        uint32_t button_tick(uint32_t dummy);
        virtual void on_play( void* argument );
        virtual void on_pause( void* argument );
        
        Pin*       button;
        Pin*       led; 
        bool       button_state;
        bool       play_state;
};











#endif
