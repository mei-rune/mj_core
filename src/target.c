
#include "meijing/target.h"
#include "meijing/string_buffer.h"

// Include files
#ifdef __cplusplus
extern "C"
{
#endif



DLL_VARIABLE void string_target_write(struct target_s* target, const void* buf, size_t len)
{
	mj_buffer_t* sb;
	if(0 == target)
		return;

	sb = (mj_buffer_t*)target->context;
	mj_buffer_appendLen(sb, (const char*)buf, len);
}


#ifdef __cplusplus
}
#endif