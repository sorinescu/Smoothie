#ifndef PLATFORM_PIN_H
#define PLATFORM_PIN_H

#include "libs/BasePin.h"

typedef int PinName;

class PlatformPin : public BasePin {
    public:
        PlatformPin(const PinDesc &desc) : BasePin(desc) {
          GPIO_InitTypeDef  GPIO_InitStructure;

          GPIO_TypeDef* gpios[4] ={ GPIOA, GPIOB, GPIOC, GPIOD };
          uint32_t GPIO_CLK[4] = { RCC_AHB1Periph_GPIOA, RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOD};

          this->port = gpios[ this->port_number ];

          RCC_AHB1PeriphClockCmd(GPIO_CLK[this->port_number], ENABLE);

          GPIO_InitStructure.GPIO_Pin = this->pin;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
          GPIO_Init(this->port, &GPIO_InitStructure);
        }

        inline Pin* as_output(){
            this->port->MODER |= 1 << this->pin*2;
            return this;
        }

        inline Pin* as_input(){
            this->port->MODER &= ~(1 << (this->pin*2));
            return this;
        }

        inline bool get(){
           if( this->inverting ){
              return ~(1 & this->port->ODR << this->pin);
           }else{
              return 1 & this->port->ODR << this->pin;
           }
        }

        inline void set(bool value){
//            // TODO : This should be bitmath
           if( this->inverting ){ value = !value; }
           if( value ){
               this->port->BSRRL |= 1 << this->pin;
           }else{
               this->port->BSRRH |= 1 << this->pin;
           }
        }

        GPIO_TypeDef* port;
};

#endif
