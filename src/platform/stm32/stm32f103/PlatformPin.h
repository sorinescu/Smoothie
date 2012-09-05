#ifndef PLATFORM_PIN_H
#define PLATFORM_PIN_H

#include "libs/BasePin.h"

typedef int PinName;

class Pin;  // the only descendent of this class

class PlatformPin : public BasePin {
protected:
    PlatformPin(PinAsUint16 apin) : BasePin(apin) {
        if (!this->connected) {
            this->port = 0;
            return;
        }

        GPIO_TypeDef* gpios[3] = { GPIOA, GPIOB, GPIOC};
        uint32_t GPIO_CLK[3] = { RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOC};

        this->port = gpios[ this->port_number ];
        this->gpio_pin = (uint16_t)1 << this->pin;

        SMT_ASSERT(GPIO_Pin_0 <= gpio_pin && gpio_pin <= GPIO_Pin_15);

        as_input();   // it's dangerous to configure as output by default

        // start clock after port is configured as input (to avoid transient outputs)
        RCC_APB2PeriphClockCmd(GPIO_CLK[this->port_number], ENABLE);
    }

    GPIO_TypeDef* port;
    uint16_t gpio_pin;

public:
    inline Pin* as_output(){
        if (this->connected) {
            GPIO_InitTypeDef  GPIO_InitStructure;

            GPIO_InitStructure.GPIO_Pin = gpio_pin;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(port, &GPIO_InitStructure);
        }

        return(Pin *)this;
    }

    inline Pin* as_input(){
        if (this->connected) {
            GPIO_InitTypeDef  GPIO_InitStructure;

            GPIO_InitStructure.GPIO_Pin = gpio_pin;
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
            return GPIO_ReadInputDataBit(port, gpio_pin) == 0;
        else
            return GPIO_ReadInputDataBit(port, gpio_pin) != 0;
    }

    inline void set(bool value){
        if (!this->connected)
            return;

        // TODO: This should be bitmath
        if (this->inverting)
            value = !value;

        if (value)
            GPIO_SetBits(port, gpio_pin);
        else
            GPIO_ResetBits(port, gpio_pin);
    }
};

#endif

