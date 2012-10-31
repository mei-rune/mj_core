#ifndef _mj_json_tokener_h_
#define _mj_json_tokener_h_ 1

#include "meijing/platform.h"
#include "meijing/variables.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_VARIABLE mj_variables_t* mj_json_parse_from_string(const char* input, size_t len,
                          char *error_buffer, size_t error_buffer_size);
//DLL_VARIABLE mj_variables_t* mj_json_parse_from_file(const char* name);


#ifdef __cplusplus
}
#endif

#endif
