#include "libs/Kernel.h"
#include "CurrentControl.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"

CurrentControl::CurrentControl(){}

void CurrentControl::on_module_loaded(){
    // if( !this->kernel->config->value( currentcontrol_module_enable_checksum )->by_default(CURRENTCONTROL_MODULE_ENABLE)->as_bool() ){ return; }

    // // Get configuration
    // this->alpha_current =           this->kernel->config->value(alpha_current_checksum  )->by_default(ALPHA_CURRENT)->as_double();
    // this->beta_current  =           this->kernel->config->value(beta_current_checksum   )->by_default(BETA_CURRENT)->as_double();
    // this->gamma_current =           this->kernel->config->value(gamma_current_checksum  )->by_default(GAMMA_CURRENT)->as_double();
    // this->delta_current =           this->kernel->config->value(delta_current_checksum  )->by_default(DELTA_CURRENT)->as_double();

    // this->kernel->digipot->set_current(0, this->alpha_current);
    // this->kernel->digipot->set_current(1, this->beta_current );
    // this->kernel->digipot->set_current(2, this->gamma_current);
    // this->kernel->digipot->set_current(3, this->delta_current);

}


