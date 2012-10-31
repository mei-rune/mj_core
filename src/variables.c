
#include <string.h>
#include "internal.h"
#include "variables_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* #define REFCOUNT_DEBUG 1 */

const char *object_number_chars = "0123456789.+-eE";
const char *object_hex_chars = "0123456789abcdef";



static void _null_empty(mj_variables_t* obj){
}

static boolean _is_primary_true(mj_variables_t* obj) {
	return true;
}


static boolean _is_primary_false(mj_variables_t* obj) {
	return false;
}


static void _object_number_print(mj_variables_t* obj)
{
	ENSURE(obj, mj_variable_type_number, EMPTY);

	switch(obj->o.c_number.number_type)  {
	case NUMBER_SIGNED_INTEGER: {
 			mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
			mj_buffer_sprintf(&buffer, "%I64d", obj->o.c_number.value.c_int);
			mj_buffer_release(&buffer, &obj->_buffer);
			break;
		}
	case NUMBER_UNSIGNED_INTEGER: {
 			mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
			mj_buffer_sprintf(&buffer, "%I64u", obj->o.c_number.value.c_uint);
			mj_buffer_release(&buffer, &obj->_buffer);
			break;
		}
	case NUMBER_DOUBLE: {
 			mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
			mj_buffer_sprintf(&buffer, "%lg", obj->o.c_number.value.c_double);
			mj_buffer_release(&buffer, &obj->_buffer);
			break;
		}
	}
}

mj_variables_type_fn_t mj_meta_objects[] = {
  { //mj_variable_type_nil 
	{4, "null"},
    0, //void (*destroy)(mj_variables_t *obj);
    0, //void (*finialize)(mj_variables_t *obj);
    0, //void (*element_set(mj_variables_t *obj, size_t idx, mj_variables_t *val);
    0, //boolean (*element_del)(mj_variables_t *obj, size_t idx);
    0, //mj_variables_t* (*element_at)(mj_variables_t *obj, size_t idx);
    0, //size_t (*clear)(mj_variables_t *obj);
    0, //size_t (*length)(mj_variables_t *obj);
    0, //void (*to_string)(mj_variables_t *obj);
  },
  {//mj_variable_type_boolean
	{7, "boolean"},
	0,
    0,
    0,
    0,
    0,
    0,
	0,
    0
  },
  { //object_type_number
	{6, "number"},
	&_mj_variables_default_destroy,
  	0,
    0,
    0,
    0,
    0,
    0, 
    &_object_number_print
  },
  { //mj_variable_type_string
	{6, "string"},
  	&_mj_variables_default_destroy,
    &_mj_variables_string_destroy,
    &_mj_variables_string_clear,
    0,
    0,
    0,
    &_mj_variables_string_length,
    &_mj_variables_string_print
  },
  { //mj_variable_type_table
	{5, "table"},
	&_mj_variables_default_destroy,
    &_mj_variables_table_destroy,
    &_mj_variables_table_clear,
    0,
    0,
	&_mj_variables_table_elementAt,
    &_mj_variables_table_length,
    &_mj_variables_table_print
  },
  { //mj_variable_type_list
	{4, "list"},
	&_mj_variables_default_destroy,
    &_mj_variables_array_destroy,
    &_mj_variables_array_clear,
    &_mj_variables_array_set,
    &_mj_variables_array_del,
    &_mj_variables_array_at,
    &_mj_variables_array_length,
    &_mj_variables_array_print
  }
};




DLL_VARIABLE boolean mj_variables_is_primary(mj_variables_t *obj)
{
	if(0 == obj) return false;

	return (mj_variable_type_boolean == obj->o_type 
		 || mj_variable_type_number == obj->o_type)?true:false;
}

DLL_VARIABLE boolean mj_variables_typeof(mj_variables_t *obj, enum mj_variable_type type)
{
	if(0 == obj)
		return false;

	return (type == obj->o_type)?true:false;
}

DLL_VARIABLE enum mj_variable_type mj_variables_get_type(mj_variables_t *obj)
{
	if(0 == obj) 
		return mj_variable_type_nil;

	return obj->o_type;
}

DLL_VARIABLE const char* mj_variables_get_type_name(mj_variables_t *obj)
{
	if(0 == obj) 
		return "null";

	return mj_meta_objects[obj->o_type].type.str;
}


DLL_VARIABLE boolean  mj_variables_is_NaN(mj_variables_t *obj)
{
	ENSURE(obj, mj_variable_type_number, false);
	return NUMBER_NAN == obj->o.c_number.number_type;
}

DLL_VARIABLE boolean  mj_variables_is_Nil(mj_variables_t *obj)
{
	if(0 == obj) return true;
	return (mj_variable_type_nil == obj->o_type)?true:false;
}

DLL_VARIABLE boolean  mj_variables_is_minusInfinity(mj_variables_t *obj)
{
	ENSURE(obj, mj_variable_type_number, false);
	return NUMBER_MINUSINFINITY == obj->o.c_number.number_type;
}

DLL_VARIABLE boolean  mj_variables_is_positiveInfinity(mj_variables_t *obj)
{
	ENSURE(obj, mj_variable_type_number, false);
	return NUMBER_POSITIVEINFINITY == obj->o.c_number.number_type;
}

DLL_VARIABLE size_t mj_variables_length(mj_variables_t *obj)
{
	if(0 == obj)
		return 0;

    if(0 == mj_meta_objects[obj->o_type].length)
    	return 0;

    return (*(mj_meta_objects[obj->o_type].length))(obj);
}

DLL_VARIABLE void mj_variables_clear(mj_variables_t *obj) {
	
	if(0 == obj)
		return ;

    if(0 == mj_meta_objects[obj->o_type].clear)
    	return ;

    (*(mj_meta_objects[obj->o_type].clear))(obj);
}

DLL_VARIABLE boolean mj_variables_put_object(mj_variables_t* obj
							, const char *key
							, mj_variables_t *val)
{
	ENSURE(obj, mj_variable_type_table, false);
	if(0 == key) return false;
	if(0 == val) return false;

	string_clear(&obj->_buffer);
	_mj_variables_table_put(obj, key, strlen(key), val);
	return true;
}

DLL_VARIABLE mj_variables_t* mj_variables_get_object(mj_variables_t* obj, const char *key)
{
	ENSURE(obj, mj_variable_type_table, 0);
	if(0 == key) return 0;

	return _mj_variables_table_get(obj, key, strlen(key));
}

DLL_VARIABLE boolean mj_variables_del_object(mj_variables_t* obj, const char *key)
{
	ENSURE(obj, mj_variable_type_table, false);
	if(0 == key) return false;

	string_clear(&obj->_buffer);
	return _mj_variables_table_del(obj, key, strlen(key));
}

#ifdef __cplusplus
}
#endif
