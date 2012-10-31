#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "variables_private.h"

#ifdef __cplusplus
extern "C" {
#endif


void _mj_variables_string_destroy(mj_variables_t* obj)
{
	ENSURE(obj, mj_variable_type_string, EMPTY);
	my_free(obj->o.c_string.str);
	obj->o.c_string.str = 0;
	obj->o.c_string.len = 0;
}

void _mj_variables_string_clear(mj_variables_t* obj)
{
	ENSURE(obj, mj_variable_type_string, EMPTY);
	obj->o.c_string.len = 0;
}

size_t _mj_variables_string_length(mj_variables_t* obj)
{
	ENSURE(obj, mj_variable_type_string, 0);
	return obj->o.c_string.len;
}

void _mj_variables_string_print(mj_variables_t *obj)
{
	mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
	ENSURE(obj, mj_variable_type_string, EMPTY);
	mj_buffer_appendLen(&buffer, "\"", 1);
	mj_buffer_appendLen(&buffer, obj->o.c_string.str, obj->o.c_string.len);
	mj_buffer_appendLen(&buffer, "\"", 1);
    mj_buffer_release(&buffer, &obj->_buffer);
}

#ifdef __cplusplus
}
#endif