/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILECONFIGSOURCE_H
#define FILECONFIGSOURCE_H

#include "platform/Platform.h"

#if SMOOTHIE_USE_FILES

#include "ConfigValue.h"
#include "ConfigSource.h"
#include "ConfigCache.h"

#define FILE_CONFIGSOURCE_CHECKSUM    5281      // "file"

class FileConfigSource : public ConfigSource {
    public:
        FileConfigSource(smt_string config_file = "/sd/config", uint16_t name_checksum = FILE_CONFIGSOURCE_CHECKSUM);
        void transfer_values_to_cache( ConfigCache* cache );
        bool is_named( uint16_t check_sum );
        void write( smt_string setting, smt_string value );
        smt_string read( smt_vector<uint16_t>::type check_sums );
        bool has_config_file();
        void try_config_file(smt_string candidate);
        smt_string get_config_file();

        smt_string config_file;         // Path to the config file
        bool   config_file_found;   // Wether or not the config file's location is known


};

#endif

#endif // SMOOTHIE_USE_FILES

