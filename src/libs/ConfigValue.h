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

class ConfigValue{
public:
    enum Type {
        CVT_NotSet,
        CVT_Bool,
        CVT_Double,
        CVT_PinDesc
    };

    ConfigValue(){
        type = CVT_NotSet;
    };

    uint16_t checksum() const {
        return check_sum;
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

    ConfigValue* set(const PinDesc &value)
    {
        pin_desc_val = value;
        type = CVT_PinDesc;
        return this;
    }

    /* Still a PinDesc value; see init() */
    ConfigValue* set(PinDescAsInt value)
    {
        PinDesc::init(pin_desc_val, value);
        type = CVT_PinDesc;
        return this;
    }

    double as_double(){
        switch (type) {
            case CVT_Bool: return (double)bool_val;
            case CVT_Double: return double_val;
            default: SMT_ASSERT(0);
        }
    }

    bool as_bool(){
        switch (type) {
            case CVT_Bool: return bool_val;
            case CVT_Double: return double_val != 0.0;
            default: SMT_ASSERT(0);
        }
    }

    Pin* as_pin(){
        switch (type) {
            //case CVT_Double: return new Pin(double_val);
            case CVT_PinDesc: return new Pin(pin_desc_val);
            default: SMT_ASSERT(0);
        }
    }

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

    ConfigValue* by_default(const PinDesc &value){
        if (type == CVT_NotSet)
            set(value);
        return this;
    }

protected:
    Type type;
    union {
        bool bool_val;
        double double_val;
        PinDesc pin_desc_val;
    };
    uint16_t check_sum;
};

#endif
