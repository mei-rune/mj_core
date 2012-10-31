
#include "meijing/platform.h"
#include "variables_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_VARIABLE boolean mj_variables_put_int(mj_variables_t *obj, const char *key, int64_t val)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_int(val));
}

DLL_VARIABLE boolean mj_variables_put_uint(mj_variables_t *obj, const char *key, uint64_t val)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_uint(val));
}

DLL_VARIABLE boolean mj_variables_put_double(mj_variables_t *obj, const char *key, double val)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_double(val));
}

DLL_VARIABLE boolean mj_variables_put_boolean(mj_variables_t *obj, const char *key, boolean val)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_boolean(val));
}

DLL_VARIABLE boolean mj_variables_put_string(mj_variables_t *obj, const char *key, const char* val)
{
	ENSURE(obj, mj_variable_type_table, false);
	return mj_variables_put_object(obj, key, mj_variables_new_string(val));
}

DLL_VARIABLE boolean mj_variables_put_stringLen(mj_variables_t *obj, const char *key, const char* val, size_t len)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_stringLen(val, len));
}

DLL_VARIABLE boolean mj_variables_put_stringN(mj_variables_t *obj, const char *key, char val, size_t n)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_stringN(val, n));
}

DLL_VARIABLE boolean mj_variables_put_string_zerocopy(mj_variables_t *obj, const char *key, char* val, size_t len)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_string_zerocopy(val, len));
}

DLL_VARIABLE boolean mj_variables_put_zerocopy(mj_variables_t *obj, const char *key, const mj_string_t* val)
{
	ENSURE(obj, mj_variable_type_table, false);

	return mj_variables_put_object(obj, key, mj_variables_new_zerocopy(val));
}

DLL_VARIABLE boolean mj_variables_get_boolean(mj_variables_t *obj, const char *key, boolean defaultValue)
{
	return mj_variables_to_boolean(mj_variables_get_object(obj, key), defaultValue);	
}

DLL_VARIABLE int64_t      mj_variables_get_int(mj_variables_t *obj, const char *key, int64_t defaultValue)
{
	return mj_variables_to_int(mj_variables_get_object(obj, key), defaultValue);	
}

DLL_VARIABLE uint64_t     mj_variables_get_uint(mj_variables_t *obj, const char *key, uint64_t defaultValue)
{
	return mj_variables_to_uint(mj_variables_get_object(obj, key), defaultValue);	
}

DLL_VARIABLE double     mj_variables_get_double(mj_variables_t *obj, const char *key, double defaultValue)
{
	return mj_variables_to_double(mj_variables_get_object(obj, key), defaultValue);	
}

DLL_VARIABLE cstring_t   mj_variables_get_string(mj_variables_t *obj, const char *key, const char* defaultValue, size_t len)
{
	return mj_variables_to_string(mj_variables_get_object(obj, key), defaultValue, len);	
}


DLL_VARIABLE boolean mj_variables_element_push_boolean(mj_variables_t *obj, boolean val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_boolean(val));
}

DLL_VARIABLE boolean mj_variables_element_push_int(mj_variables_t *obj, int64_t val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_int(val));
}

DLL_VARIABLE boolean mj_variables_element_push_uint(mj_variables_t *obj, uint64_t val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_uint(val));
}

DLL_VARIABLE boolean mj_variables_element_push_double(mj_variables_t *obj, double val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_double(val));
}

DLL_VARIABLE boolean mj_variables_element_push_string(mj_variables_t *obj, const char* val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_string(val));
}

DLL_VARIABLE boolean mj_variables_element_push_stringLen(mj_variables_t *obj, const char* val, size_t len)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_stringLen(val, len));
}

DLL_VARIABLE boolean mj_variables_element_push_stringN(mj_variables_t *obj, char val, size_t n)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_stringN(val, n));
}

DLL_VARIABLE boolean mj_variables_element_push_string_zerocopy(mj_variables_t *obj, char* val, size_t len)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_string_zerocopy(val, len));
}

DLL_VARIABLE boolean mj_variables_element_push_zerocopy(mj_variables_t *obj, const mj_string_t* val)
{
	ENSURE(obj, mj_variable_type_list, false);

	return mj_variables_element_push(obj, mj_variables_new_zerocopy(val));
}

DLL_VARIABLE boolean mj_variables_element_pop_boolean(mj_variables_t *obj, boolean defaultValue)
{
	mj_variables_t* ret;
	boolean val;
	if(!obj)
		return defaultValue;

	ret = mj_variables_element_pop(obj);
	if(0 == ret)
		return defaultValue;
	val = mj_variables_to_boolean(ret, defaultValue);
	mj_variables_destory(ret);
	return val;
}

DLL_VARIABLE int64_t      mj_variables_element_pop_int(mj_variables_t *obj, int64_t defaultValue)
{
	mj_variables_t* ret;
	int64_t val;
	if(!obj)
		return defaultValue;

	ret = mj_variables_element_pop(obj);
	if(0 == ret)
		return defaultValue;
	val = mj_variables_to_int(ret, defaultValue);
	mj_variables_destory(ret);
	return val;
}

DLL_VARIABLE uint64_t     mj_variables_element_pop_uint(mj_variables_t *obj, uint64_t defaultValue)
{
	mj_variables_t* ret;
	uint64_t val;
	if(!obj)
		return defaultValue;

	ret = mj_variables_element_pop(obj);
	if(0 == ret)
		return defaultValue;
	val = mj_variables_to_uint(ret, defaultValue);
	mj_variables_destory(ret);
	return val;
}

DLL_VARIABLE double     mj_variables_element_pop_double(mj_variables_t *obj, double defaultValue)
{
	mj_variables_t* ret;
	double val;
	if(!obj)
		return defaultValue;

	ret = mj_variables_element_pop(obj);
	if(0 == ret)
		return defaultValue;
	val = mj_variables_to_double(ret, defaultValue);
	mj_variables_destory(ret);
	return val;
}

DLL_VARIABLE mj_string_t   mj_variables_element_pop_string(mj_variables_t *obj)
{
	mj_string_t tmp;
	mj_variables_t* ret;
	mj_string_t val = STRING_DEFAULT;
	if(!obj)
		return val;

	ret = mj_variables_element_pop(obj);
	if(0 == ret)
		return val;

	
	mj_variables_to_string(ret, 0, 0);
	//string_swap(&obj->_buffer, &val);
	memcpy(&tmp, &obj->_buffer, sizeof(mj_string_t));
	memcpy(&obj->_buffer, &val, sizeof(mj_string_t));
	memcpy(&val, &tmp, sizeof(mj_string_t));

	mj_variables_destory(ret);
	return val;
}


DLL_VARIABLE boolean mj_variables_element_set_boolean(mj_variables_t *obj, size_t idx, boolean value)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_boolean(value));
}
DLL_VARIABLE boolean  mj_variables_element_set_int(mj_variables_t *obj, size_t idx, int64_t value)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_int(value));
}
DLL_VARIABLE boolean  mj_variables_element_set_uint(mj_variables_t *obj, size_t idx, uint64_t value)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_uint(value));
}
DLL_VARIABLE boolean  mj_variables_element_set_double(mj_variables_t *obj, size_t idx, double value)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_double(value));
}
DLL_VARIABLE boolean  mj_variables_element_set_string(mj_variables_t *obj, size_t idx, const char* str)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_string(str));
}
DLL_VARIABLE boolean  mj_variables_element_set_stringLen(mj_variables_t *obj, size_t idx, const char* str, size_t len)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_stringLen(str, len));
}
DLL_VARIABLE boolean  mj_variables_element_set_stringN(mj_variables_t *obj, size_t idx, char c, size_t n)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_stringN(c, n));
}
DLL_VARIABLE boolean  mj_variables_element_set_string_zerocopy(mj_variables_t *obj, size_t idx, char* str, size_t len)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_string_zerocopy(str, len));
}
DLL_VARIABLE boolean  mj_variables_element_set_zerocopy(mj_variables_t *obj, size_t idx, const mj_string_t* str)
{
	ENSURE(obj, mj_variable_type_list, false);
	return mj_variables_element_set(obj, idx, mj_variables_new_zerocopy(str));
}

DLL_VARIABLE int64_t      mj_variables_element_at_int(mj_variables_t *obj, size_t idx, int64_t defaultValue)
{
	return mj_variables_to_int(mj_variables_element_at(obj, idx), defaultValue);
}
DLL_VARIABLE uint64_t     mj_variables_element_at_uint(mj_variables_t *obj, size_t idx, uint64_t defaultValue)
{
	return mj_variables_to_uint(mj_variables_element_at(obj, idx), defaultValue);
}
DLL_VARIABLE double     mj_variables_element_at_double(mj_variables_t *obj, size_t idx, double defaultValue)
{
	return mj_variables_to_double(mj_variables_element_at(obj, idx), defaultValue);
}
DLL_VARIABLE boolean mj_variables_element_at_boolean(mj_variables_t *obj, size_t idx, boolean defaultValue)
{
	return mj_variables_to_boolean(mj_variables_element_at(obj, idx), defaultValue);
}
DLL_VARIABLE cstring_t  mj_variables_element_at_string(mj_variables_t *obj, size_t idx, const char* defaultStr, size_t len)
{
	return mj_variables_to_string(mj_variables_element_at(obj, idx), defaultStr, len);
}

#ifdef __cplusplus
}
#endif

