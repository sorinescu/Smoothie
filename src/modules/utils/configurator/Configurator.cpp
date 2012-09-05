/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/


#include "libs/Kernel.h"
#include "Configurator.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/SerialMessage.h"
#include "libs/StreamOutput.h"


void Configurator::on_module_loaded(){
    this->register_for_event(ON_CONSOLE_LINE_RECEIVED);
//    this->register_for_event(ON_GCODE_EXECUTE);
//    this->register_for_event(ON_MAIN_LOOP);
}

// When a new line is received, check if it is a command, and if it is, act upon it
void Configurator::on_console_line_received( void* argument ){
    SerialMessage new_message = *static_cast<SerialMessage*>(argument);
    smt_string possible_command = new_message.message;

    // We don't compare to a smt_string but to a checksum of that smt_string, this saves some space in flash memory
    uint16_t check_sum = get_checksum( possible_command.substr(0,possible_command.find_first_of(" \r\n")) );  // todo: put this method somewhere more convenient

    // Act depending on command
    switch( check_sum ){
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
        case config_get_command_checksum: this->config_get_command(  get_arguments(possible_command), new_message.stream ); break;
        case config_set_command_checksum: this->config_set_command(  get_arguments(possible_command), new_message.stream ); break;
#endif
        case config_load_command_checksum: this->config_load_command(  get_arguments(possible_command), new_message.stream ); break;
    }
}

// Process and respond to eeprom gcodes (M50x)
void Configurator::on_gcode_execute(void* argument){
    Gcode* gcode = static_cast<Gcode*>(argument);
    if( gcode->has_letter('G') ){
        int code = gcode->get_value('G');
        switch( code ){
        }
    }
    else if( gcode->has_letter('M') ){
        int code = gcode->get_value('M');
        switch( code ){
        }
    }
}

void Configurator::on_main_loop(void* argument){}

#if SMOOTHIE_HAS_CONFIG_VALUE_STRING

// Output a ConfigValue from the specified ConfigSource to the stream
void Configurator::config_get_command( smt_string parameters, StreamOutput* stream ){
    smt_string source = shift_parameter(parameters);
    smt_string setting = shift_parameter(parameters);
    if (setting == "") { // output live setting
        setting = source;
        source = "";
        smt_vector<uint16_t>::type setting_checksums = get_checksums( setting );
        ConfigValue* cv = this->kernel->config->value(setting_checksums);
        smt_string value = "";
        if(cv->valid()){ value = cv->as_string(); }
        stream->printf( "live: %s is set to %s\r\n", setting.c_str(), value.c_str() );
    } else { // output setting from specified source
        uint16_t source_checksum = get_checksum( source );
        smt_vector<uint16_t>::type setting_checksums = get_checksums( setting );
        for(int i=0; i < this->kernel->config->config_sources.size(); i++){
            if( this->kernel->config->config_sources[i]->is_named(source_checksum) ){
                smt_string value = this->kernel->config->config_sources[i]->read(setting_checksums);
                stream->printf( "%s: %s is set to %s\r\n", source.c_str(), setting.c_str(), value.c_str() );
                break;
            }
        }
    }
}

// Write the specified setting to the specified ConfigSource
void Configurator::config_set_command( smt_string parameters, StreamOutput* stream ){
    smt_string source = shift_parameter(parameters);
    smt_string setting = shift_parameter(parameters);
    smt_string value = shift_parameter(parameters);
    if (value == "") {
        value = setting;
        setting = source;
        source = "";
        this->kernel->config->set_string(setting, value);
        stream->printf( "live: %s has been set to %s\r\n", setting.c_str(), value.c_str() );
    } else {
        uint16_t source_checksum = get_checksum(source);
        for(int i=0; i < this->kernel->config->config_sources.size(); i++){
            if( this->kernel->config->config_sources[i]->is_named(source_checksum) ){
                this->kernel->config->config_sources[i]->write(setting, value);
                stream->printf( "%s: %s has been set to %s\r\n", source.c_str(), setting.c_str(), value.c_str() );
                break;
            }
        }
    }
}

#endif // SMOOTHIE_HAS_CONFIG_VALUE_STRING

// Reload config values from the specified ConfigSource
void Configurator::config_load_command( smt_string parameters, StreamOutput* stream ){
    smt_string source = shift_parameter(parameters);
    if(source == ""){
        this->kernel->config->config_cache_load();
        this->kernel->call_event(ON_CONFIG_RELOAD);
        const char *message = "Reloaded settings\r\n";
        if(stream)
            stream->printf( message );
        else
            kernel->streams->printf( message );
#if SMOOTHIE_USE_FILES
    } else if(file_exists(source)){
        FileConfigSource fcs(source);
        fcs.transfer_values_to_cache(&this->kernel->config->config_cache);
        this->kernel->call_event(ON_CONFIG_RELOAD);
        stream->printf( "Loaded settings from %s\r\n", source.c_str() );
#endif
    } else {
        uint16_t source_checksum = get_checksum(source);
        for(int i=0; i < this->kernel->config->config_sources.size(); i++){
            if( this->kernel->config->config_sources[i]->is_named(source_checksum) ){
                this->kernel->config->config_sources[i]->transfer_values_to_cache(&this->kernel->config->config_cache);
                this->kernel->call_event(ON_CONFIG_RELOAD);
                stream->printf( "Loaded settings from %s\r\n", source.c_str() );
                break;
            }
        }
    }
}

