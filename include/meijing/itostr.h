#ifndef _mj_itostr_h
#define _mj_itostr_h 1

#include <string.h>
#include "meijing/platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_VARIABLE size_t mj_ui64tostr_r(uint64_t value, char *string, size_t len, int radix);

DLL_VARIABLE size_t mj_i64tostr_r(int64_t value, char *string, size_t len, int radix);

DLL_VARIABLE char * mj_i64tostr(int64_t value, char *string, int radix);

DLL_VARIABLE char * mj_ui64tostr(uint64_t value, char *string, int radix);

DLL_VARIABLE char * mj_itostr(int value, char *string, int radix);

DLL_VARIABLE char * mj_uitostr(unsigned int value, char *string, int radix);

DLL_VARIABLE char * mj_ltostr(long value, char *string, int radix);

DLL_VARIABLE char * mj_ultostr(unsigned long value, char *string, int radix);

#ifdef __cplusplus
}
#endif

#ifndef _WIN32
#define itoa mj_itostr
#define ltoa mj_ltostr
#endif

#endif
