#ifndef PIN_H
#define PIN_H

//#include "mbed.h" //Required for LPC_GPIO* . can probably be found in one othe the files mbed.h includes. TODO
//#include "../gcc4mbed/external/mbed/LPC1768/LPC17xx.h"
#include "libs/Kernel.h"
#include "libs/utils.h"
#include <string>
#include <cmath>
#include <cstdlib>

typedef int PinName;
// enum PinName {
// PIN1 = 1  
// };

class Pin{
    public:
        Pin(){ }

        Pin* from_string(std::string value){
          GPIO_InitTypeDef  GPIO_InitStructure;

          GPIO_TypeDef* gpios[4] ={ GPIOA, GPIOB, GPIOC, GPIOD };
          uint32_t GPIO_CLK[4] = { RCC_AHB1Periph_GPIOA, RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOD};

          this->port_number = atoi(value.substr(0,1).c_str());
          this->port = gpios[ this->port_number ];

          RCC_AHB1PeriphClockCmd(GPIO_CLK[this->port_number], ENABLE);

          this->inverting = ( value.find_first_of("!")!=string::npos ? true : false );
          this->pin  = atoi( value.substr(2, value.size()-2-(this->inverting?1:0)).c_str() );


          GPIO_InitStructure.GPIO_Pin = this->pin;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
          GPIO_Init(this->port, &GPIO_InitStructure);

          return this;
        }

        inline Pin*  as_output(){
            this->port->MODER |= 1 << this->pin*2;
            return this;
        }  

        inline Pin*  as_input(){
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

        bool inverting;
        GPIO_TypeDef* port;
        char pin;
        char port_number;
};




#endif
