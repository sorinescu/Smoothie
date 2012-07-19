/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

using namespace std;
#include <vector>
#include "libs/Config.h"

void Config::on_module_loaded(){
    register_for_event(ON_CONSOLE_LINE_RECEIVED);
}

ConfigValue* Config::value(uint16_t check_sum_a, uint16_t check_sum_b, uint16_t check_sum_c ){
    vector<uint16_t> check_sums;
    check_sums.push_back(check_sum_a);
    check_sums.push_back(check_sum_b);
    check_sums.push_back(check_sum_c);
    return value(check_sums);
}

ConfigValue* Config::value(uint16_t check_sum_a, uint16_t check_sum_b){
    vector<uint16_t> check_sums;
    check_sums.push_back(check_sum_a);
    check_sums.push_back(check_sum_b);
    return value(check_sums);
}

ConfigValue* Config::value(uint16_t check_sum){
    vector<uint16_t> check_sums;
    check_sums.push_back(check_sum);
    return value(check_sums);
}


