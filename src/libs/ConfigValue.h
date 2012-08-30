/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIGVALUE_H
#define CONFIGVALUE_H

#include "libs/Kernel.h"
#include "libs/utils.h"
#include "libs/Pin.h"
#include "platform/DefaultCNCConfig.h"

class ConfigValue{
public:
    enum Type {
        CVT_NotSet,
        CVT_Bool,
        CVT_Double,
        CVT_Pin
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
        ,CVT_String
#endif
    };

    ConfigValue(){
        type = CVT_NotSet;
    };

    bool checksum_matches(const ConfigValue *other) const {
        if (check_sums.size() != other->check_sums.size())
            return false;

        for (int i=0; i<check_sums.size(); ++i)
            if (check_sums[i] != other->check_sums[i])
                return false;

        return true;
    }

    bool valid() const {
        return type != CVT_NotSet;
    }

    ConfigValue* required(){
        SMT_ASSERT(type);
        if (type)
            return this;

        return NULL;
    }

    ConfigValue* set(bool value)
    {
        bool_val = value;
        type = CVT_Bool;
        return this;
    }

    ConfigValue* set(double value)
    {
        double_val = value;
        type = CVT_Double;
        return this;
    }

    /*
    ConfigValue* set(const PinDesc &value)
    {
        pin_val = value;
        type = CVT_Pin;
        return this;
    }
    */

    /* Still a PinDesc value; see init() */
    ConfigValue* set(PinAsUint16 value)
    {
        pin_val = value;
        type = CVT_Pin;
        return this;
    }

#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
    ConfigValue* set(const smt_string &value)
    {
        str_val = value;
        type = CVT_String;
        return this;
    }
#endif

    double as_double(){
        switch (type) {
            case CVT_Bool: return (double)bool_val;
            case CVT_Double: return double_val;
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
            case CVT_String: {
                double result = atof(remove_non_number(this->str_val).c_str());
                if( result == 0.0 && this->str_val.find_first_not_of("0.") != smt_string::npos )
                    SMT_ASSERT(0);
                return result;
            }
#endif
            default: SMT_ASSERT(0);
        }
        return 0.0;
    }

    bool as_bool(){
        switch (type) {
            case CVT_Bool: return bool_val;
            case CVT_Double: return double_val != 0.0;
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
            case CVT_String: return str_val.find_first_of("t1") != smt_string::npos;
#endif
            default: SMT_ASSERT(0);
        }
        return false;
    }

    Pin* as_pin(){
        switch (type) {
            case CVT_Pin: return new Pin(pin_val);
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
            case CVT_String: return new Pin(Pin::from_str(str_val));
#endif
            default: SMT_ASSERT(0);
        }
        return 0;
    }

#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
    smt_string as_string(){
        char fmt[32] = "";
        switch (type) {
            case CVT_Bool: sprintf(fmt, "%s", bool_val ? "true" : "false");
            case CVT_Double: sprintf(fmt, "%lf", double_val);
            case CVT_Pin: PIN_IS_INVERTING(pin_val) ?
                sprintf(fmt, "%d.%d!", (int)PIN_PORT_NR(pin_val), (int)PIN_PIN(pin_val)) :
                sprintf(fmt, "%d.%d", (int)PIN_PORT_NR(pin_val), (int)PIN_PIN(pin_val));
            case CVT_String: return str_val;
            default: SMT_ASSERT(0);
        }
        return fmt;
    }
#endif

    ConfigValue* by_default(double value){
        if (type == CVT_NotSet)
            set(value);
        return this;
    }

    ConfigValue* by_default(bool value){
        if (type == CVT_NotSet)
            set(value);
        return this;
    }

    ConfigValue* by_default(PinAsUint16 value){
        if (type == CVT_NotSet)
            set(value);
        return this;
    }

#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
    ConfigValue* by_default(const smt_string &value){
        if (type == CVT_NotSet)
            set(value);
        return this;
    }
#endif

protected:
    Type type;
    union {
        bool bool_val;
        double double_val;
        PinAsUint16 pin_val;
    };
#if SMOOTHIE_HAS_CONFIG_VALUE_STRING
    smt_string str_val; // can't be put inside of union because it has non-static methods
#endif

    smt_vector<uint16_t>::type check_sums;

    friend class Config;
};

#endif
