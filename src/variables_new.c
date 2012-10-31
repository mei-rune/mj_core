
#include <string.h>
#include "variables_private.h"

#ifdef __cplusplus
extern "C"
{
#endif


char _nil_str[] = "null";
char _nan_str[] = "NaN";
char _minusInfinity[] = "-Infinity";
char _positiveInfinity[] = "Infinity";
char _true_str[] = "true";
char _false_str[] = "false";



mj_variables_t  _Nil = {
	mj_variable_type_nil,
	{4, _nil_str},
	{0}
};

mj_variables_t _NaN = {
	mj_variable_type_number,
	{3, _nan_str},
	{{NUMBER_NAN, {0}}}
};

mj_variables_t  _MinusInfinity = {
	mj_variable_type_number,
	{9, _minusInfinity},
	{{NUMBER_MINUSINFINITY, {0}}}
};

mj_variables_t  _PositiveInfinity = {
	mj_variable_type_number,
	{8, _positiveInfinity},
	{{NUMBER_POSITIVEINFINITY, {0}}}
};

mj_variables_t  _trueObject = {
	mj_variable_type_boolean,
	{4, _true_str},
	{true}
};

mj_variables_t  _falseObject = {
	mj_variable_type_boolean,
	{5, _false_str},
	{false}
};

void _mj_variables_default_destroy(mj_variables_t *obj)
{
	if(0 == obj)
		return;

	if(mj_variable_type_number == obj->o_type) {
		if(NUMBER_NAN == obj->o.c_number.number_type
			|| NUMBER_MINUSINFINITY == obj->o.c_number.number_type
			|| NUMBER_POSITIVEINFINITY == obj->o.c_number.number_type)
			return;
	}

	my_free(obj->_buffer.str);
	my_free(obj);
}


DLL_VARIABLE void mj_variables_destory(mj_variables_t *obj)
{
	if(0 == obj)
		return;

    if(0 != mj_meta_objects[obj->o_type].finialize)
        (*(mj_meta_objects[obj->o_type].finialize))(obj);
	
    if(0 != mj_meta_objects[obj->o_type].destroy)
        (*(mj_meta_objects[obj->o_type].destroy))(obj);
}



DLL_VARIABLE mj_variables_t*  mj_variables_new_NaN()
{
	return &_NaN;
}

DLL_VARIABLE mj_variables_t*  mj_variables_new_Nil()
{
	return &_Nil;
}

DLL_VARIABLE mj_variables_t*  mj_variables_new_minusInfinity()
{
	return &_MinusInfinity;
}

DLL_VARIABLE mj_variables_t*  mj_variables_new_positiveInfinity()
{
	return &_PositiveInfinity;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_boolean(boolean b)
{
	return (true == b)?&_trueObject:&_falseObject;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_uint(uint64_t i)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_number;
	obj->o.c_number.number_type = NUMBER_UNSIGNED_INTEGER;
	obj->o.c_number.value.c_uint = i;
	string_init(&obj->_buffer, 0, 0);
	return obj;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_int(int64_t i)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_number;
	obj->o.c_number.number_type = NUMBER_SIGNED_INTEGER;
	obj->o.c_number.value.c_int = i;
	string_init(&obj->_buffer, 0, 0);
	return obj;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_double(double d)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_number;
	obj->o.c_number.number_type = NUMBER_DOUBLE;
	obj->o.c_number.value.c_double = d;
	string_init(&obj->_buffer, 0, 0);
	return obj;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_string_zerocopy(char *s, size_t len)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_string;
	obj->o.c_string.str = s;
	obj->o.c_string.len = (-1 == len)?strlen(s):len;
	string_init(&obj->_buffer, 0, 0);
	return obj;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_zerocopy(const mj_string_t* str)
{
	return mj_variables_new_string_zerocopy(string_data(str), string_length(str));
}

DLL_VARIABLE mj_variables_t* mj_variables_new_stringLen(const char *s, size_t len)
{
	if(0 == s)
	    return mj_variables_new_string_zerocopy(0, 0);

	if(-1 == len)
		len = strlen(s);

	return mj_variables_new_string_zerocopy(my_strndup(s, len), len);
}

DLL_VARIABLE mj_variables_t* mj_variables_new_stringN(char c, size_t n)
{
	char* s = (char*)my_malloc(n + 1);
	memset(s, c, n);
	s[n] = 0;
	return mj_variables_new_string_zerocopy(s, n);
}

DLL_VARIABLE mj_variables_t* mj_variables_new_string(const char *s)
{
	return mj_variables_new_stringLen(s, strlen(s));
}

DLL_VARIABLE mj_variables_t* mj_variables_new_table(void)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_table;
	obj->o.c_hash.is_kv = false;
	obj->o.c_hash.c_un.c_object = _mj_variables_hashtable_create(JSON_OBJECT_DEF_HASH_ENTRIES);
	string_init(&obj->_buffer, 0, 0);
	return obj;
}

DLL_VARIABLE mj_variables_t* mj_variables_new_list(void)
{
	mj_variables_t *obj = (mj_variables_t*)my_calloc(sizeof(mj_variables_t), 1);
	obj->o_type = mj_variable_type_list;
	ARRAY_INIT(mj_variables_t*, &obj->o.c_array, 32);
	string_init(&obj->_buffer, 0, 0);
	return obj;
}
	
#ifdef __cplusplus
}
#endif