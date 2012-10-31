
#ifndef _internal_h_
#define _internal_h_ 1

#include <stddef.h>
#include "meijing/platform.h"

#ifdef __cplusplus
extern "C" {
#endif


char* my_strndup(const char* str, size_t n);
	
#ifdef __cplusplus
}
#endif

#endif //_internal_h_