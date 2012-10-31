

#ifndef _mj_buffer_h_
#define _mj_buffer_h_ 1

// Include files
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include "meijing/platform.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct string_s
{
	size_t          len;
	char*           str;
} mj_string_t;

typedef struct cstring_s
{
	size_t          len;
	const char*     str;
} cstring_t;

typedef struct mj_buffer_s
{
	size_t          len;       
	char*           str;    
	size_t          capacity; 
} mj_buffer_t;

#define STRING_BUFFER_DEFAULT  { 0, NULL, 0 }
#define STRING_DEFAULT  { 0, NULL }

#define  string_init(s, p, l) (s)->str = (p); (s)->len = (l);

#define  mj_buffer_init(s) (s)->str = 0; (s)->len = 0; (s)->capacity = 0

#define  mj_string_destroy(s) my_free((s)->str); (s)->str = 0; (s)->len = 0

#define  string_data(s) (s)->str

#define  string_length(s) (s)->len

#define  string_clear(s) (s)->len = 0;

#define  string_assign(s, v) (s)->len = (v)->len; (s)->str = (v)->str

#define  string_truncate(s, l) (s)->len = (l);

DLL_VARIABLE void mj_buffer_destroy(mj_buffer_t* pcs);

DLL_VARIABLE void mj_buffer_release(mj_buffer_t* lhs, mj_string_t* rhs);

DLL_VARIABLE mj_buffer_t* mj_buffer_vsprintf(mj_buffer_t* pcs, const char*fmt, va_list argList);

DLL_VARIABLE mj_buffer_t* mj_buffer_sprintf(mj_buffer_t* pcs, const char*fmt, ...);

DLL_VARIABLE mj_buffer_t* mj_buffer_append(mj_buffer_t*   pcs
				   ,   char const*         s);

DLL_VARIABLE mj_buffer_t* mj_buffer_appendLen(mj_buffer_t*   pcs
					  , char const*         s
					  , size_t              len);

DLL_VARIABLE mj_buffer_t* mj_buffer_appendN(mj_buffer_t*   pcs
					  , char                ch
					  , size_t              n);

#ifdef __cplusplus
};
#endif


#endif

