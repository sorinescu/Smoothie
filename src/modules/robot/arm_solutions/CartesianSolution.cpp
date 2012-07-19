#include "CartesianSolution.h"
#include <math.h>

CartesianSolution::CartesianSolution(Config* passed_config) : config(passed_config){
	// this->microseconds_per_step_pulse   =  this->kernel->config->value(microseconds_per_step_pulse_ckeckusm  )->by_default(5   )->as_number();
    this->alpha_steps_per_mm = this->config->value( alpha_steps_per_mm_checksum)->by_default(1000)->as_double();
    this->beta_steps_per_mm  = this->config->value(  beta_steps_per_mm_checksum)->by_default(1000)->as_double();
    this->gamma_steps_per_mm = this->config->value( gamma_steps_per_mm_checksum)->by_default(1000)->as_double();
}

void CartesianSolution::millimeters_to_steps( double millimeters[], int steps[] ){
    steps[ALPHA_STEPPER] = lround( millimeters[X_AXIS] * this->alpha_steps_per_mm );
    steps[BETA_STEPPER ] = lround( millimeters[Y_AXIS] * this->beta_steps_per_mm );
    steps[GAMMA_STEPPER] = lround( millimeters[Z_AXIS] * this->gamma_steps_per_mm );
}

void CartesianSolution::steps_to_millimeters( int steps[], double millimeters[] ){}
