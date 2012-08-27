#ifndef utils_h
#define utils_h

#include "platform/Platform.h"
#include <stdint.h>

smt_string lc(smt_string str);

smt_string remove_non_number( smt_string str );

uint16_t get_checksum(smt_string to_check);

smt_vector<uint16_t>::type get_checksums(smt_string key);

smt_string shift_parameter( smt_string &parameters );

smt_string get_arguments( smt_string possible_command );

#if SMOOTHIE_USE_FILES
bool file_exists( smt_string file_name );
#endif

void system_reset( void );


#endif
