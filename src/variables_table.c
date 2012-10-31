#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "meijing/hashtable.h"
#include "variables_private.h"

#ifdef __cplusplus
extern "C" {
#endif


#define GETNEXT(el)       el->o.c_hash.c_un.c_kv->next
#define SETNEXT(el, n)    el->o.c_hash.c_un.c_kv->next = n

#define GETKEY(el)        el->o.c_hash.c_un.c_kv->key
#define SETKEY(el, k)     el->o.c_hash.c_un.c_kv->key = k

#define GETVALUE(el)      el->o.c_hash.c_un.c_kv->value
#define SETVALUE(el, val) el->o.c_hash.c_un.c_kv->value = val

	
#define GETKEY_STRREF(el) GETKEY(el).o.c_string
#define GETKEY_STR(el)    string_data(&(GETKEY_STRREF(el)))
#define GETKEY_LEN(el)    string_length(&(GETKEY_STRREF(el)))


static void _mj_variables_hashtable_resize(object_hashtable_t* hash);


static  mj_variables_t* _mj_variables_object_keyvalue_create(const char* key, size_t key_len, mj_variables_t* value)
{
	object_keyvalue_t* kv = 0;
	mj_variables_t* ret = (mj_variables_t*)my_calloc(1, sizeof(mj_variables_t));
	ret->o_type = mj_variable_type_table;
	kv = (object_keyvalue_t*)my_calloc(1, sizeof(object_keyvalue_t));
	
	kv->next = 0;
	kv->key.o_type = mj_variable_type_string;
	kv->key.o.c_string.len = key_len;
	kv->key.o.c_string.str = (char*)my_strndup(key, key_len);

	kv->value = value;

	ret->o.c_hash.is_kv = true;
	ret->o.c_hash.c_un.c_kv = kv;
	return ret;
}


static void _mj_variables_object_finialize(mj_variables_t *obj)
{
	if(0 == obj)
		return ;
	
    if(0 == mj_meta_objects[obj->o_type].finialize)
    {
        string_clear(&obj->_buffer);
    	return ;
    }

    (*(mj_meta_objects[obj->o_type].finialize))(obj);
}

static void _mj_variables_object_keyvalue_clear(mj_variables_t* value)
{
	mj_variables_destory(value->o.c_hash.c_un.c_kv->value);
	_mj_variables_object_finialize(&(value->o.c_hash.c_un.c_kv->key));
	my_free(value->o.c_hash.c_un.c_kv);
	value->o.c_hash.c_un.c_kv = 0;
}

static void _mj_variables_object_keyvalue_free(mj_variables_t* value)
{
	_mj_variables_object_keyvalue_clear(value);
	my_free(value);
}

object_hashtable_t* _mj_variables_hashtable_create(size_t size)
{
	object_hashtable_t* ret = (object_hashtable_t*)my_malloc(sizeof(object_hashtable_t));
	ret->capacity = size;
	ret->entries = (mj_variables_t**)my_malloc(sizeof(mj_variables_t*)*size);
	memset(ret->entries, 0, sizeof(mj_variables_t*)*size);
	ret->used = 0;
	ret->array = 0;
	return ret;
}

static boolean _mj_variables_hashtable_put(object_hashtable_t* hash, const char* key, size_t key_len, mj_variables_t* value, mj_variables_t* new_el)
{
	mj_variables_t* el = NULL;
	mj_variables_t* parent_el = NULL;
	
	size_t index = mj_mem_hash(key, key_len) % hash->capacity;


	/* 检测是否已存在 */
	el = hash->entries[index];
	while (el && mj_mem_cmp(GETKEY_STR(el)
		, GETKEY_LEN(el)
		, key, key_len))
	{
		parent_el = el;
		el = GETNEXT(el);
	}
	
	if(el)
	{
		/* 已存在, 则使用旧对象 */

		// 只有在_mj_variables_hashtable_resize调用时, new_el 才不为 null
		// 而 _mj_variables_hashtable_resize调用时, hash 一定是空的
		assert(0 == new_el);
		
		mj_variables_destory(GETVALUE(el));
		SETVALUE(el, value);
		return false;
	}

	if(0 == new_el)
		new_el = _mj_variables_object_keyvalue_create(key, key_len, value);
	
	SETNEXT(new_el, NULL);
	
	if (parent_el)
		SETNEXT(parent_el, new_el);
	else
		hash->entries[index] = new_el;
	
	hash->used++;
		
		/* 如果使用率大于 HASHTABLE_MAX_USAGE 时性能下将, 需要调整大小 */
	if ((double) hash->used / hash->capacity > HASHTABLE_MAX_USAGE)
		_mj_variables_hashtable_resize(hash);
	
	return true;
}



static void _mj_variables_hashtable_resize(object_hashtable_t* hash)
{
	
	size_t i;
	size_t old_size  = hash->capacity;
	mj_variables_t** old_entries = hash->entries;

	// 重新分配内存
	hash->capacity *= 2;
	hash->entries = (mj_variables_t**)my_calloc(hash->capacity, sizeof(mj_variables_t*));

	hash->used = 0;
	
	for (i = 0; i < old_size; i++)
	{
		mj_variables_t* el = old_entries[i];
		assert(el->o_type == mj_variable_type_table && true == el->o.c_hash.is_kv);
		while (el)
		{
			mj_variables_t* next = GETNEXT(el);
			mj_string_t* key = &(GETKEY_STRREF(el));
			_mj_variables_hashtable_put(hash, key->str, key->len
				, GETVALUE(el), el);
			el = next;
			
			assert(el->o_type == mj_variable_type_table && true == el->o.c_hash.is_kv);
		}
	}

	/* free old data */
	my_free(old_entries);
}

static boolean object_hashtable_del(object_hashtable_t* hash, const char* key, size_t key_len)
{
	mj_variables_t* el = NULL;
	mj_variables_t* parent_el = NULL;
	
	size_t index = mj_mem_hash(key, key_len) % hash->capacity;


	/* 检测是否已存在 */
	el = hash->entries[index];
	while (el && mj_mem_cmp(GETKEY_STR(el)
		, GETKEY_LEN(el)
		, key, key_len))
	{
		parent_el = el;
		el = GETNEXT(el);
	}

	if(!el)  /* 不存在, 则返回 */
		return false;

	if (parent_el)
		SETNEXT(parent_el, GETNEXT(el));
	else
		hash->entries[index] = GETNEXT(el);

	
	_mj_variables_object_keyvalue_free(el);

	if(0 != hash->array)
	{
		my_free(hash->array);
		hash->array = 0;
	}

	hash->used--;
	return true;
}

static void object_hashtable_put(object_hashtable_t* hash, const char* key, size_t key_len, mj_variables_t *val)
{
	if(0 == val)
	{
		object_hashtable_del(hash, key, key_len);
		return;
	}

	if(_mj_variables_hashtable_put(hash, key, key_len, val, NULL))
	{
		// 没有旧值, 说明结构发生变化
		if(0 != hash->array)
		{
			my_free(hash->array);
			hash->array = 0;
		}
	}
}

static mj_variables_t* object_hashtable_get(const object_hashtable_t* hash, const char* key, size_t key_len)
{
	mj_variables_t* el = hash->entries[mj_mem_hash(key, key_len) % hash->capacity];
	while (el) {
		if (mj_mem_cmp(GETKEY_STR(el)
		, GETKEY_LEN(el), key, key_len) == 0)
			return GETVALUE(el);

		el = GETNEXT(el);
	}

	return NULL;
}


static mj_variables_t* object_hashtable_elementAt(object_hashtable_t* hash, size_t idx)
{	
	size_t i;
	size_t count;
	if(0 == hash || idx >= hash->used)
		return 0;

	if(0 != hash->array)
		return hash->array[idx];


	if(hash->used > 20)
	{
		hash->array = (mj_variables_t**)my_malloc(sizeof(mj_variables_t*)*hash->used + 1);
		
		count = 0;
		for (i = 0; i < hash->capacity; i++) 
		{
			mj_variables_t* el = hash->entries[i];
			while (el) 
			{
				hash->array[count ++ ] = el;
				el = GETNEXT(el);
			}
		}

		return hash->array[idx];
	}

	count = 0;
	for (i = 0; i < hash->capacity; i++) 
	{
		mj_variables_t* el = hash->entries[i];
		while (el) 
		{
			if(idx == count ++)
				return el;

			el = GETNEXT(el);
		}
	}
	return 0;
}

static void object_hashtable_clear(object_hashtable_t* hash)
{
	size_t i;
	if(0 == hash)
		return;


	for (i = 0; i < hash->capacity; i++) 
	{
		mj_variables_t* el = hash->entries[i];
		hash->entries[i] = 0;
		while (el) 
		{
			mj_variables_t* next = GETNEXT(el);
			_mj_variables_object_keyvalue_free(el);
			el = next;
		}
	}

	if(0 == hash->array)
	{
		my_free(hash->array);
		hash->array = 0;
	}
	hash->used = 0;
}


static void object_hashtable_destroy(object_hashtable_t* hash)
{
	if(0 == hash)
		return;

	object_hashtable_clear(hash);
	my_free(hash);
}


/* table_object */
void _mj_variables_table_put(mj_variables_t* hash, const char* key, size_t key_len, mj_variables_t *val)
{
	if(hash->o.c_hash.is_kv)
		return;
	
	object_hashtable_put(hash->o.c_hash.c_un.c_object, key, key_len, val);
}


boolean _mj_variables_table_del(mj_variables_t* hash, const char* key, size_t key_len)
{
	if(hash->o.c_hash.is_kv)
		return false;

	return object_hashtable_del(hash->o.c_hash.c_un.c_object, key, key_len);
}

mj_variables_t* _mj_variables_table_get(const mj_variables_t* hash, const char* key, size_t key_len)
{
	if(hash->o.c_hash.is_kv)
	{
		if(0 == strcasecmp("key", key))
			return &(GETKEY(hash));
		else if(0 == strcasecmp("value", key))
			return GETVALUE(hash);
		else
			return 0;
	}
	return object_hashtable_get(hash->o.c_hash.c_un.c_object, key, key_len);
}

mj_variables_t* _mj_variables_table_elementAt(mj_variables_t* hash, size_t idx)
{
	if(hash->o.c_hash.is_kv)
	{
		switch(idx)
		{
		case 0:
			return &(GETKEY(hash));
		case 1:
			return GETVALUE(hash);
		}
		return 0;
	}

	return object_hashtable_elementAt(hash->o.c_hash.c_un.c_object, idx);
}

void _mj_variables_table_clear(mj_variables_t* hash)
{
	if(hash->o.c_hash.is_kv)
			return ;

	object_hashtable_clear(hash->o.c_hash.c_un.c_object);
}

void _mj_variables_table_destroy(mj_variables_t* hash)
{
	if(hash->o.c_hash.is_kv)
		_mj_variables_object_keyvalue_clear(hash);
	else
		object_hashtable_destroy(hash->o.c_hash.c_un.c_object);
}

size_t _mj_variables_table_length(mj_variables_t* hash)
{
	return (hash->o.c_hash.is_kv)?2:hash->o.c_hash.c_un.c_object->used;
}

void _mj_variables_table_print(mj_variables_t *obj) {
	mj_buffer_t buffer = STRING_BUFFER_DEFAULT;
	if(obj->o.c_hash.is_kv) {
		cstring_t key_str = _mj_variables_to_print_string(mj_variables_element_at(obj, 0));
		cstring_t value_str = _mj_variables_to_print_string(mj_variables_element_at(obj, 1));
		mj_buffer_appendLen(&buffer, string_data(&key_str), string_length(&key_str));
		mj_buffer_appendLen(&buffer, ":", 1);
		mj_buffer_appendLen(&buffer, string_data(&value_str), string_length(&value_str));
        mj_buffer_release(&buffer, &obj->_buffer);	
	} else {
		_mj_variables_collection_print(obj, "{", 1, "}", 1);
	}
}

#ifdef __cplusplus
}
#endif