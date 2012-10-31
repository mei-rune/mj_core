
#ifndef _mj_hooks_h_
#define _mj_hooks_h_ 1

#include <stdio.h>
#include <stdlib.h>
#include "meijing/platform.h"
#include "meijing/cycle.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_VARIABLE mj_module_t* mj_load_module(mj_cycle_* server
	, size_t argc
	, const char*const* args
	, char* err
	, size_t len);

#ifdef __cplusplus
}
#endif


#endif // __hooks_h_