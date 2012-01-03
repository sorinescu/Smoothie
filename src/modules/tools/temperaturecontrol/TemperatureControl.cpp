#include "mbed.h"
#include "libs/Module.h"
#include "libs/Kernel.h"
#include <math.h>
#include "TemperatureControl.h"

TemperatureControl::TemperatureControl(){
    this->error_count = 0; 
}

void TemperatureControl::on_module_loaded(){
    
    // We start now desiring any temp
    this->desired_adc_value = UNDEFINED;

    // Settings
    this->readings_per_second = 5;

    this->r0 = 100000;               // Stated resistance eg. 100K
    this->t0 = 25 + 273.15;          // Temperature at stated resistance, eg. 25C
    this->beta = 4066;               // Thermistor beta rating. See http://reprap.org/bin/view/Main/MeasuringThermistorBeta
    this->vadc = 3.3;                // ADC Reference
    this->vcc  = 3.3;                // Supply voltage to potential divider
    this->k = this->r0 * exp( -this->beta / this->t0 );
    double r1 = 0;
    double r2 = 4700;

    if( r1 > 0 ){
        this->vs = r1 * this->vcc / ( r1 + r2 );
        this->rs = r1 * r2 / ( r1 + r2 );
    }else{
        this->vs = this->vcc;
        this->rs = r2;
    } 

    this->acceleration_factor = 10;

    // Setup pins and timer 
    this->thermistor_pin = new AnalogIn(p20); 
    this->kernel->slow_ticker->attach( this, &TemperatureControl::thermistor_read_tick );
    this->heater_pwm = new PwmOut(p22);
    this->heater_pwm->write(0);
    this->pwm_value = 0;

    // Register for events
    this->register_for_event(ON_GCODE_EXECUTE); 

}

void TemperatureControl::on_gcode_execute(void* argument){
    Gcode* gcode = static_cast<Gcode*>(argument);
    
    // Set temperature
    if( gcode->has_letter('M') && gcode->get_value('M') == 104 && gcode->has_letter('S') ){
        this->set_desired_temperature(gcode->get_value('S')); 
    } 

    // Get temperature
    if( gcode->has_letter('M') && gcode->get_value('M') == 105 ){
        this->kernel->serial->printf("get temperature: %f \r\n", this->get_temperature() );
    } 
}

void TemperatureControl::set_desired_temperature(double desired_temperature){
    this->desired_adc_value = this->temperature_to_adc_value(desired_temperature);
    this->tail_adc_value =  this->temperature_to_adc_value(desired_temperature-20);
    this->head_adc_value =  this->temperature_to_adc_value(desired_temperature+5);
}

double TemperatureControl::get_temperature(){
    double temp = this->new_thermistor_reading() ;
    return this->adc_value_to_temperature( this->new_thermistor_reading() );
}

double TemperatureControl::adc_value_to_temperature(double adc_value){
    double v = adc_value * this->vadc;            // Convert from 0-1 adc value to voltage
    double r = this->rs * v / ( this->vs - v );   // Resistance of thermistor
    return ( this->beta / log( r / this->k )) - 273.15;
}

double TemperatureControl::temperature_to_adc_value(double temperature){
    double r = this->r0 * exp( this->beta * ( 1 / (temperature + 273.15) -1 / this->t0 ) ); // Resistance of the thermistor 
    double v = this->vs * r / ( this->rs + r );                                             // Voltage at the potential divider
    return v / this->vadc * 1.00000;                                               // The ADC reading
}

void TemperatureControl::thermistor_read_tick(){

    double reading = this->new_thermistor_reading();
    if( this->desired_adc_value != UNDEFINED ){
        double difference = fabs( reading - this->desired_adc_value ); 
        double adjustment = difference / acceleration_factor / this->readings_per_second;
        if( reading > this->tail_adc_value ){
            this->heater_pwm->write( 1 );
        }else if( reading < this->head_adc_value ){
            this->pwm_value -= adjustment;
            this->heater_pwm->write( 0 );
        }else{
           if( reading > this->desired_adc_value ){
                this->pwm_value += adjustment;  // Heat up
            }else{
                this->pwm_value -= adjustment;  // Heat down
            }
            this->pwm_value = max( double(0), min( double(1), pwm_value ) );
            this->heater_pwm->write( pwm_value ); 
        }
    }

}

double TemperatureControl::new_thermistor_reading(){
    double new_reading = this->thermistor_pin->read();

    if( this->queue.size() < 15 ){
        this->queue.push_back( new_reading );
        //this->kernel->serial->printf("first\r\n");
        return new_reading;
    }else{
        double current_temp = this->average_adc_reading();
        double error = fabs(new_reading - current_temp); 
        if( error < 0.1 ){
            this->error_count = 0;
            double test;
            this->queue.pop_front(test); 
            this->queue.push_back( new_reading );
        }else{
            this->error_count++;
            if( this->error_count > 4 ){
                double test;
                this->queue.pop_front(test); 
            }
        } 
        return current_temp;
    }
}


double TemperatureControl::average_adc_reading(){
    double total;
    int j=0;
    int reading_index = this->queue.head;
    while( reading_index != this->queue.tail ){
        j++;
        total += this->queue.buffer[reading_index];
        reading_index = this->queue.next_block_index( reading_index );
    }
    return total / j;
}



