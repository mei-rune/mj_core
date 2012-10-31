#ifndef _mj_log_h_
#define _mj_log_h_ 1

#include "meijing/platform.h"


typedef struct mj_log_message_s
{
	void* context;
	const char* message;
	size_t length;
} mj_log_message_t;

typedef void (*mj_log_callback_t)(void* ctx, const mj_log_message_t * const * msg, size_t n);

#define LOG_SYSTEM   9000
#define LOG_FATAL    8000
#define LOG_NONE     7000
#define LOG_ERROR    6000
#define LOG_WARN     5000
#define LOG_TRACE    4000
#define LOG_ALL      0000

DLL_VARIABLE void mj_log_message(void* context, int level, const char* message);
DLL_VARIABLE void mj_log_format (void* context, int level, const char* fmt, ...);
DLL_VARIABLE void mj_log_vformat(void* context, int level, const char* fmt, va_list argList);


DLL_VARIABLE void mj_log_set_callback(mj_log_callback_t cb, void* ctx);

#endif // _log_h_