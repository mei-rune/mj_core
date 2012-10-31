
#ifndef _mj_win32_time_h_
#define _mj_win32_time_h_ 1

#include <stddef.h>

#include "meijing/platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_VARIABLE void gettimeofday(struct timeval* tv, void* dummy);

#ifdef __cplusplus
}
#endif 

#endif 

