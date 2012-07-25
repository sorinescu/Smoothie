#include "libs/Kernel.h"
#include "Pauser.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"

Pauser::Pauser(){}

void Pauser::on_module_loaded(){
    this->counter = 0;
}

void Pauser::take(){
    this->counter++;
    if( this->counter == 1 ){
        this->kernel->call_event(ON_PAUSE, &this->counter);
    }
}

void Pauser::release(){
    this->counter--;
    if( this->counter == 0 ){
        this->kernel->call_event(ON_PLAY, &this->counter);
    }
}
