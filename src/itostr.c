
#include "meijing/itostr.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
 * @implemented
 */
DLL_VARIABLE size_t mj_ui64tostr_r(uint64_t value, char *string, size_t len, int radix)
{
    char *pos;
    uint64_t digit;
    uint64_t val = value;
    if(2 > len)
        return -1;

    pos = string + len -1;
    *pos = '\0';

    do {
      digit = val % radix;
      val = val / radix;
      if (digit < 10)
        *--pos = '0' + (char)digit;
      else
        *--pos = 'a' + (char)digit - 10;
    
      if(string == pos)
        return -1;
        
    } while (val != 0L);

    memcpy(string, pos, string + len - pos);
    return len - (pos - string);
}

DLL_VARIABLE size_t mj_i64tostr_r(int64_t value, char *string, size_t len, int radix)
{
    size_t ret;

    if (value >= 0 || radix != 10)
        return mj_ui64tostr_r(value, string, len, radix);

    if (len == 0)
       return -1;

    string[0] = '-';
    ret = mj_ui64tostr_r(-value, string + 1, len -1, radix);
    return (-1 == ret)?-1: ret + 1;
}

DLL_VARIABLE char * mj_i64tostr(int64_t value, char *string, int radix)
{
    char buffer[33];
    size_t len = mj_i64tostr_r(value, buffer, 33, radix);

    memcpy(string, &buffer[33-len], len);
    return string;
}

DLL_VARIABLE char * mj_ui64tostr(uint64_t value, char *string, int radix)
{
    char buffer[33];
    size_t len = mj_ui64tostr_r(value, buffer, 33, radix);

    memcpy(string, &buffer[33-len], len);
    return string;
}

DLL_VARIABLE char * mj_itostr(int value, char *string, int radix)
{
	return mj_i64tostr((int64_t)value, string, radix);
}

DLL_VARIABLE char * mj_uitostr(unsigned int value, char *string, int radix)
{
	return mj_ui64tostr((uint64_t)value, string, radix);
}

DLL_VARIABLE char * mj_ltostr(long value, char *string, int radix)
{
	return mj_i64tostr((int64_t)value, string, radix);
}

DLL_VARIABLE char * mj_ultostr(unsigned long value, char *string, int radix)
{
	return mj_ui64tostr((uint64_t)value, string, radix);
}

#ifdef __cplusplus
}
#endif
