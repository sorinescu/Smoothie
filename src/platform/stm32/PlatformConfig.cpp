/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/


using namespace std;
#include <vector>
#include <string>
#include <stdio.h>
#include "libs/Kernel.h"
#include "PlatformConfig.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"

PlatformConfig::PlatformConfig(){
}

void PlatformConfig::on_console_line_received( void* argument ){
    // not needed - static configuration
}

// Get a value from the configuration
ConfigValue* PlatformConfig::value(vector<uint16_t> check_sums){
    ConfigValue* result = new ConfigValue;

    //TODO: there should be a static configuration (from a configuration header file?)
    return result;
}

void PlatformConfig::get_module_list(vector<uint16_t>* list, uint16_t family){
    // TODO: there should be a static list of modules (from a configuration header file?)
}

