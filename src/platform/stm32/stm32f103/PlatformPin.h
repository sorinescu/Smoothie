#ifndef PLATFORM_PIN_H
#define PLATFORM_PIN_H

#include "libs/BasePin.h"

typedef int PinName;

class Pin;  // the only descendent of this class

class PlatformPin : public BasePin {
protected:
    PlatformPin(PinAsUint16 pin) : BasePin(pin) {
        if (!this->connected) {
            this->port = 0;
            return;
        }

        GPIO_TypeDef* gpios[3] = { GPIOA, GPIOB, GPIOC};
        uint32_t GPIO_CLK[3] = { RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOC};

        RCC_APB2PeriphClockCmd(GPIO_CLK[this->port_number], ENABLE);

        this->port = gpios[ this->port_number ];

        as_input();   // it's dangerous to configure as output by default
    }

    GPIO_TypeDef* port;

public:
    inline Pin* as_output(){
        if (this->connected) {
            GPIO_InitTypeDef  GPIO_InitStructure;

            GPIO_InitStructure.GPIO_Pin = pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(port, &GPIO_InitStructure);
        }

        return(Pin *)this;
    }

    inline Pin* as_input(){
        if (this->connected) {
            GPIO_InitTypeDef  GPIO_InitStructure;

            GPIO_InitStructure.GPIO_Pin = pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(port, &GPIO_InitStructure);
        }

        return(Pin *)this;
    }

    inline bool get(){
        if (!this->connected)
            return false;

        if (this->inverting)
            return GPIO_ReadInputDataBit(port, pin) == 0;
        else
            return GPIO_ReadInputDataBit(port, pin) != 0;
    }

    inline void set(bool value){
        if (!this->connected)
            return;

        // TODO: This should be bitmath
        if (this->inverting)
            value = !value;

        if (value)
            GPIO_SetBits(port, pin);
        else
            GPIO_ResetBits(port, pin);
    }
};

#endif

