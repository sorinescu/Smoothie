#ifndef PLATFORM_PIN_H
#define PLATFORM_PIN_H

#include "libs/BasePin.h"
#include "libs/LPC17xx/sLPC17xx.h" // smoothed mbed.h lib

class PlatformPin : public BasePin {
    public:
        PlatformPin(const PinDesc &desc) : BasePin(desc) {
            LPC_GPIO_TypeDef* gpios[5] ={LPC_GPIO0,LPC_GPIO1,LPC_GPIO2,LPC_GPIO3,LPC_GPIO4};
            this->port = gpios[this->port_number];
        }

        inline PlatformPin*  as_output(){
            this->port->FIODIR |= 1<<this->pin;
            return this;
        }

        inline PlatformPin*  as_input(){
            this->port->FIODIR &= ~(1<<this->pin);
            return this;
        }

        inline PlatformPin* as_open_drain(){
            if( this->port_number == 0 ){ LPC_PINCON->PINMODE_OD0 |= (1<<this->pin); }
            if( this->port_number == 1 ){ LPC_PINCON->PINMODE_OD1 |= (1<<this->pin); }
            if( this->port_number == 2 ){ LPC_PINCON->PINMODE_OD2 |= (1<<this->pin); }
            if( this->port_number == 3 ){ LPC_PINCON->PINMODE_OD3 |= (1<<this->pin); }
            if( this->port_number == 4 ){ LPC_PINCON->PINMODE_OD4 |= (1<<this->pin); }
            return this;
        }

        inline bool get(){
            return this->inverting ^ (( this->port->FIOPIN >> this->pin ) & 1);
        }

        inline void set(bool value){
            value = this->inverting ^ value;
            if( value ){
                this->port->FIOSET = 1 << this->pin;
            }else{
                this->port->FIOCLR = 1 << this->pin;
            }
        }

        LPC_GPIO_TypeDef* port;
};

#endif
