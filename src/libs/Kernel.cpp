/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "libs/Module.h"
#include "libs/Config.h"
#include "libs/nuts_bolts.h"
#include "libs/SlowTicker.h"
#include "libs/Pauser.h"
#include "libs/StreamOutputPool.h"

#include "modules/communication/GcodeDispatch.h"
#include "modules/robot/Planner.h"
#include "modules/robot/Robot.h"
#include "modules/robot/Stepper.h"
#include "modules/robot/Player.h"


// List of callback functions, ordered as their corresponding events
const ModuleCallback kernel_callback_functions[NUMBER_OF_DEFINED_EVENTS] = {
        &Module::on_main_loop,
        &Module::on_console_line_received,
        &Module::on_gcode_received,
        &Module::on_stepper_wake_up,
        &Module::on_gcode_execute,
        &Module::on_speed_change,
        &Module::on_block_begin,
        &Module::on_block_end,
        &Module::on_config_reload,
        &Module::on_play,
        &Module::on_pause,
        &Module::on_idle
};

#define baud_rate_setting_checksum 10922
#define uart0_checksum             16877

// The kernel is the central point in Smoothie : it stores modules, and handles event calls
Kernel::Kernel(){
    // Config first, because we need the baud_rate setting before we start serial
    this->config         = new Config();
    // Serial second, because the other modules might want to say something
    this->streams        = new StreamOutputPool();
    //this->serial         = new SerialConsole(NULL, NULL, NULL);

    this->add_module( this->config );
    //this->add_module( this->serial );

    init_platform();    // defined in PlatformKernel

    this->slow_ticker          = new SlowTicker();
    this->slow_ticker->kernel = this; // DEBUG: To remove
    this->step_ticker          = new StepTicker();
    this->step_ticker->kernel = this; // DEBUG: To remove

    // Core modules
    this->add_module( this->gcode_dispatch = new GcodeDispatch() );
    this->add_module( this->robot          = new Robot()         );
    this->add_module( this->stepper        = new Stepper()       );
    this->add_module( this->planner        = new Planner()       );
    this->add_module( this->player         = new Player()        );
    this->add_module( this->pauser         = new Pauser()        );

    // this->adc                  = new Adc();
    // this->digipot              = new Digipot();
}

void Kernel::add_module(Module* module){
    module->kernel = this;
    module->on_module_loaded();
    module->register_for_event(ON_CONFIG_RELOAD);
}

void Kernel::register_for_event(unsigned int id_event, Module* module){
    this->hooks[id_event].push_back(module);
}

void Kernel::call_event(unsigned int id_event){
    for(unsigned int i=0; i < this->hooks[id_event].size(); i++){
        (this->hooks[id_event][i]->*kernel_callback_functions[id_event])(this);
    }
}

void Kernel::call_event(unsigned int id_event, void * argument){
    for(unsigned int i=0; i < this->hooks[id_event].size(); i++){
        (this->hooks[id_event][i]->*kernel_callback_functions[id_event])(argument);
    }
}
