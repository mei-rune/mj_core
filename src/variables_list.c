
#include <string.h>
#include "variables_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

void _mj_variables_array_destroy(mj_variables_t *obj)
{
	ARRAY_DESTROY(mj_variables_t*, &obj->o.c_array);
}

size_t _mj_variables_array_length(mj_variables_t *obj)
{
	return ARRAY_LENGTH(mj_variables_t*, &obj->o.c_array);
}

void _mj_variables_array_clear(mj_variables_t *obj)
{
	ARRAY_CLEAR(mj_variables_t*, &obj->o.c_array);
}

boolean _mj_variables_array_set(mj_variables_t *obj, size_t idx, mj_variables_t* val)
{
    mj_variables_destory(ARRAY_GET(mj_variables_t*, &obj->o.c_array, idx, NULL));	
	ARRAY_PUT(mj_variables_t*, &obj->o.c_array, idx, val);
	return true;
}

boolean _mj_variables_array_del(mj_variables_t *obj, size_t idx)
{
	mj_variables_destory(ARRAY_GET(mj_variables_t*, &obj->o.c_array, idx, NULL));
	ARRAY_DEL(mj_variables_t*, &obj->o.c_array, idx, MOVE_MEM);
	return true;
}

mj_variables_t* _mj_variables_array_at(mj_variables_t *obj, size_t idx)
{
	return ARRAY_GET(mj_variables_t*, &obj->o.c_array, idx, NULL);
}

void _mj_variables_collection_print(mj_variables_t *obj
	, const char* startTag, size_t startLen
	, const char* stopTag, size_t stopLen)
{
	mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
	size_t idx;
	mj_buffer_appendLen(&buffer, startTag, startLen);
	for( idx = 0; idx < mj_variables_length(obj); ++ idx) {
		cstring_t value_str = mj_variables_to_string(mj_variables_element_at(obj, idx), 0, 0);
		if(0 != idx)
		    mj_buffer_appendLen(&buffer, ",", 1);
		mj_buffer_appendLen(&buffer, string_data(&value_str), string_length(&value_str));
	}
	mj_buffer_appendLen(&buffer, stopTag, stopLen);

    mj_buffer_release(&buffer, &obj->_buffer);
}

void _mj_variables_array_print(mj_variables_t *obj)
{
	_mj_variables_collection_print(obj, "[", 1, "]", 1);
}

DLL_VARIABLE boolean mj_variables_element_push(mj_variables_t *obj, mj_variables_t *val)
{
	ENSURE(obj, mj_variable_type_list, false);

	string_clear(&obj->_buffer);
	ARRAY_PUSH(mj_variables_t*, &obj->o.c_array, val);
	return true;
}

DLL_VARIABLE mj_variables_t * mj_variables_element_pop(mj_variables_t *obj)
{
	mj_variables_t* ret;
	ENSURE(obj, mj_variable_type_list, NULL);

	string_clear(&obj->_buffer);
	ret = ARRAY_GET(mj_variables_t*, &obj->o.c_array, 0, NULL);
	ARRAY_DEL(mj_variables_t*, &obj->o.c_array, 0, MOVE_MEM);
	return ret;
}

DLL_VARIABLE boolean mj_variables_element_set(mj_variables_t *obj, size_t idx, mj_variables_t *val)
{
	if(0 == obj)
	    return false;

    if(0 == mj_meta_objects[obj->o_type].element_set)
	    return false;

	string_clear(&obj->_buffer);
    return (*(mj_meta_objects[obj->o_type].element_set))(obj, idx, val);
}

DLL_VARIABLE boolean mj_variables_element_del(mj_variables_t *obj, size_t idx)
{
	if(0 == obj)
	    return false;
  
    if(0 == mj_meta_objects[obj->o_type].element_del)
        return false;

    if(false == (*(mj_meta_objects[obj->o_type].element_del))(obj, idx))
        return false;
    	
	string_clear(&obj->_buffer);
	return true;

//	mj_variables_destory(ARRAY_GET(mj_variables_t*, &obj->o.c_array, idx, NULL));
//	ARRAY_DEL(mj_variables_t*, &obj->o.c_array, idx, MOVE_MEM);
//	return true;
}

DLL_VARIABLE mj_variables_t* mj_variables_element_at(mj_variables_t *obj,	size_t idx)
{
	if(0 == obj)
		return NULL;

    if(0 == mj_meta_objects[obj->o_type].element_at)
        return NULL;

    return (*(mj_meta_objects[obj->o_type].element_at))(obj, idx);
}

#ifdef __cplusplus
}
#endif