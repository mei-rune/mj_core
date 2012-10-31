

#ifndef _mj_hashtable_h_
#define _mj_hashtable_h_ 1

#include "meijing/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/** instance表的最大使用率 */
#define HASHTABLE_MAX_USAGE 0.60

	
#define HASH_DECLARE_TABLE(name, key_type, value_type)                                            \
typedef struct name##_s {                                                                          \
	/** hash表内块大小 */                                                                          \
	size_t capacity;                                                                               \
	/** hash表内当前对象数 */                                                                      \
	size_t used;                                                                                   \
	/** hash表内当前对象块指针 */                                                                  \
	value_type** entries;                                                                          \
	/** 用于 toArray() 函数保存临时数据的 */                                                       \
	value_type** ar;                                                                               \
} name##_t;
	
	
#define HASH_DECLARE_FUNC2(HASH_VARIABLE, name, key_type, value_type)                              \
HASH_VARIABLE void name##_init(name##_t* instance, size_t default_size);                           \
HASH_VARIABLE void name##_destory(name##_t* instance);                                             \
HASH_VARIABLE void name##_clear(name##_t* instance);                                               \
HASH_VARIABLE value_type* name##_at(const name##_t* instance, size_t idx);                         \
HASH_VARIABLE value_type** name##_to_array(name##_t* instance);                                    \
HASH_VARIABLE value_type** name##_copy_to(const name##_t* instance, value_type** copy, size_t len);   \
HASH_VARIABLE value_type* name##_get(const name##_t* instance, key_type key);                      \
HASH_VARIABLE void name##_put(name##_t* instance, value_type* val);                                \
HASH_VARIABLE boolean name##_del(name##_t* instance, key_type key);                                \
HASH_VARIABLE size_t name##_count(name##_t* instance);

	


#define HASH_DECLARE2(HASH_VARIABLE, name, key_type, value_type)                                   \
	HASH_DECLARE_TABLE(name, key_type, value_type)                                                \
	HASH_DECLARE_FUNC2(HASH_VARIABLE, name, key_type, value_type)


#define HASH_ELEMENT_FOREACH_BEGIN(value_type, entries, size)                                      \
	for (i = 0; i < size; i++) {                                                                   \
		value_type* el = entries[i];                                                               \
		while (el)

#define HASH_ELEMENT_FOREACH_END() }
	

#define HASH_DECLARE_THUNK(name, key_type, value_type)                                             \
typedef struct name##_entry_s {                                                                    \
	key_type key;                                                                                  \
	value_type value;                                                                              \
} name##_entry_t;                                                                                  \
	                                                                                               \
                                                                                                   \
typedef struct name##_thunk_s {                                                                    \
	                                                                                               \
	name##_entry_t  ref;                                                                           \
	                                                                                               \
	struct name##_thunk_s* next;                                                                   \
	                                                                                               \
} name##_thunk_t;

#define HASH_DECLARE_FUNC(HASH_VARIABLE, name, key_type, value_type)                               \
HASH_DECLARE_FUNC2(HASH_VARIABLE, name, key_type, name##_thunk_t)                                  \
HASH_VARIABLE value_type name##_get_value(const name##_t* instance                                 \
	                   , key_type key, value_type defaultValue);                                   \
HASH_VARIABLE void name##_put_value(name##_t* instance, key_type key, value_type val);             \
HASH_VARIABLE value_type name##_at_value(const name##_t* instance, size_t idx                      \
	                                          , value_type defaultValue);


#define HASH_DECLARE(HASH_VARIABLE, name, key_type, value_type)                                    \
	HASH_DECLARE_THUNK(name, key_type, value_type)                                                 \
	HASH_DECLARE_TABLE(name, key_type, name##_thunk_t)                                            \
	HASH_DECLARE_FUNC(HASH_VARIABLE, name, key_type, value_type)


#define HASH_FIND_VALUE(value_type                                                                 \
        , key                                                                                      \
		, get_key                                                                                  \
		, get_next                                                                                 \
		, hash                                                                                     \
		, cmp)                                                                                     \
	value_type* el = NULL;                                                                         \
	value_type* parent_el = NULL;                                                                  \
                                                                                                   \
    size_t  index = hash(key) % instance->capacity;                                                \
                                                                                                   \
	el = instance->entries[index];                                                                 \
	while (el && cmp(get_key(el), key)) {                                                          \
		parent_el = el;                                                                            \
		el = get_next(el);                                                                         \
	}


#define HASH_SET_NEW_VALUE(new_el, set_next)                                                       \
        if (parent_el) {                                                                           \
            set_next(parent_el, new_el);                                                           \
        } else  {                                                                                  \
            instance->entries[index] = new_el;                                                     \
        }                                                                                          \




#define HASH_DEFINE2(HASH_VARIABLE                                                                 \
        , name                                                                                     \
		, key_type                                                                                 \
		, value_type                                                                               \
		, get_key                                                                                  \
		, get_next                                                                                 \
		, set_next                                                                                 \
		, hash                                                                                     \
		, cmp                                                                                      \
        , del)                                                                                     \
	                                                                                               \
void _##name##_resize(name##_t* instance);                                                         \
	                                                                                               \
HASH_VARIABLE void name##_init(name##_t* instance, size_t default_size) {                          \
	instance->capacity = default_size;                                                             \
	instance->used = 0;                                                                            \
                                                                                                   \
	instance->entries = (value_type**)my_calloc(instance->capacity, sizeof(value_type*));          \
	instance->ar = 0;                                                                              \
}                                                                                                  \
	                                                                                               \
HASH_VARIABLE void name##_destroy(name##_t* instance) {                                            \
	if(0 == instance) {                                                                            \
		return;                                                                                    \
	}                                                                                              \
	name##_clear(instance);                                                                        \
	my_free(instance->entries);                                                                    \
}                                                                                                  \
                                                                                                   \
void name##_put(name##_t* instance, value_type* new_el) {                                          \
                                                                                                   \
	key_type key = get_key(new_el);                                                                \
	HASH_FIND_VALUE(value_type, key, get_key, get_next, hash, cmp)                                 \
                                                                                                   \
	if(el) {                                                                                       \
		set_next(new_el, get_next(el));                                                            \
        HASH_SET_NEW_VALUE(new_el, set_next);                                                      \
		del(el);                                                                                   \
		return ;                                                                                   \
	}                                                                                              \
                                                                                                   \
	set_next(new_el, NULL);                                                                        \
	                                                                                               \
    HASH_SET_NEW_VALUE(new_el, set_next);                                                          \
                                                                                                   \
	if(0 != instance->ar) {                                                                        \
		my_free(instance->ar);                                                                     \
		instance->ar = 0;                                                                          \
	}                                                                                              \
                                                                                                   \
	instance->used++;                                                                              \
	if ((double) instance->used / instance->capacity > HASHTABLE_MAX_USAGE) {                      \
		_##name##_resize(instance);                                                                \
	}	                                                                                           \
	return;	                                                                                       \
}	                                                                                               \
	                                                                                               \
void _##name##_resize(name##_t* instance) {                                                        \
                                                                                                   \
	size_t old_size  = instance->capacity;                                                         \
	value_type** old_entries = instance->entries;                                                  \
                                                                                                   \
	instance->capacity *= 2;                                                                       \
	instance->entries = (value_type**)my_calloc(instance->capacity, sizeof(value_type*));          \
                                                                                                   \
	instance->used = 0;                                                                            \
                                                                                                   \
	{	                                                                                           \
		size_t i;                                                                                  \
		HASH_ELEMENT_FOREACH_BEGIN(value_type, old_entries, old_size) {                            \
				value_type* next = get_next(el);                                                   \
				name##_put(instance, el);                                                          \
				el = next;                                                                         \
       }                                                                                           \
       HASH_ELEMENT_FOREACH_END()                                                                  \
	}                                                                                              \
                                                                                                   \
	my_free(old_entries);                                                                          \
}                                                                                                  \
	                                                                                               \
HASH_VARIABLE boolean name##_del(name##_t* instance, key_type key) {                               \
	                                                                                               \
	HASH_FIND_VALUE(value_type, key, get_key, get_next, hash, cmp)                                 \
	                                                                                               \
	if(!el) {  /* 不存在, 则返回 */                                                                \
		return false;                                                                              \
	}                                                                                              \
	                                                                                               \
	HASH_SET_NEW_VALUE(get_next(el), set_next);                                                    \
                                                                                                   \
	del(el);                                                                                       \
                                                                                                   \
	if(0 != instance->ar) {                                                                        \
		my_free(instance->ar);                                                                     \
		instance->ar = 0;                                                                          \
	}                                                                                              \
                                                                                                   \
	instance->used--;                                                                              \
	return true;                                                                                   \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE value_type* name##_get(const name##_t* instance, key_type key) {                     \
                                                                                                   \
	size_t  index = hash(key) % instance->capacity;                                                \
	value_type* el = instance->entries[index];                                                     \
	while (el) {                                                                                   \
		if (cmp(get_key(el), key) == 0) {                                                          \
			return el;                                                                             \
		}                                                                                          \
		el = get_next(el);                                                                         \
	}                                                                                              \
                                                                                                   \
	return NULL;                                                                                   \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE value_type* name##_at(const name##_t* instance, size_t idx) {                        \
                                                                                                   \
	size_t i;                                                                                      \
	size_t count;                                                                                  \
	if(NULL == instance) {                                                                         \
		return NULL;                                                                               \
	}                                                                                              \
                                                                                                   \
	if(0 != instance->ar) {                                                                        \
		return (idx >= instance->used)? NULL:instance->ar[idx];                                    \
	}                                                                                              \
                                                                                                   \
	count = 0;                                                                                     \
	HASH_ELEMENT_FOREACH_BEGIN(value_type, instance->entries, instance->capacity) {                \
		if(idx == count ++) {                                                                      \
			return el;                                                                             \
		}                                                                                          \
		el = get_next(el);                                                                         \
    }                                                                                              \
    HASH_ELEMENT_FOREACH_END()                                                                     \
	return NULL;                                                                                   \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE value_type** name##_copy_to(const name##_t* instance                                 \
                                              , value_type** copy, size_t len) {                   \
	size_t i;                                                                                      \
	HASH_ELEMENT_FOREACH_BEGIN(value_type, instance->entries, instance->capacity) {                \
		copy[-- len] = el;                                                                         \
		if(0 == len) break;                                                                        \
		el = get_next(el);                                                                         \
    } HASH_ELEMENT_FOREACH_END()                                                                   \
	return copy;                                                                    	           \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE value_type** name##_to_array(name##_t* instance) {                                   \
	if(0 != instance->ar) {                                                                        \
	    return instance->ar;                                                                       \
	}                                                                                              \
	instance->ar = (value_type**)my_malloc(sizeof(value_type*)*(instance->used + 1)); 	           \
	name##_copy_to(instance, instance->ar, instance->used);                                        \
    instance->ar[instance->used] = 0;                                                              \
	return instance->ar;                                                                           \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE void name##_clear(name##_t* instance) {                                              \
	size_t i;                                                                                      \
	if(0 == instance) {                                                                            \
		return;                                                                                    \
	}                                                                                              \
                                                                                                   \
	HASH_ELEMENT_FOREACH_BEGIN(value_type, instance->entries, instance->capacity) {                \
		value_type* next = get_next(el);                                             	           \
		del(el);                                                                     	           \
		el = next;                                                                   	           \
    } HASH_ELEMENT_FOREACH_END()                                                                   \
	memset(instance->entries, 0, instance->capacity * sizeof(value_type*));                        \
                                                                                                   \
	if(0 != instance->ar) {                                                                        \
		my_free(instance->ar);                                                                     \
		instance->ar = 0;                                                                          \
	}                                                                                              \
	instance->used = 0;                                                                            \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE size_t name##_count(name##_t* instance) {                                            \
	return instance->used;                                                                         \
}


#define default_get_key(x)  x->ref.key
#define default_set_key(x, k)  x->ref.key = k
#define default_get_value(x)  x->ref.value
#define default_set_value(x, v)  x->ref.value = v
#define default_get_next(x)  x->next
#define default_set_next(x, n)  x->next = n



#define HASH_DEFINE_FULL(HASH_VARIABLE                                                             \
        , name                                                                                     \
		, key_type                                                                                 \
		, value_type                                                                               \
		, thunk_type                                                                               \
		, get_key                                                                                  \
		, set_key                                                                                  \
		, get_value                                                                                \
		, set_value                                                                                \
		, get_next                                                                                 \
		, set_next                                                                                 \
		, hash                                                                                     \
		, cmp                                                                                      \
        , del_key                                                                                  \
        , del_value                                                                                \
        , thunk_new                                                                                \
        , thunk_del)                                                                               \
HASH_DEFINE2(HASH_VARIABLE, name, key_type, thunk_type, get_key, get_next                          \
        , set_next, hash, cmp, _##name##hash_trunk_del);                                           \
                                                                                                   \
HASH_VARIABLE value_type name##_at_value(const name##_t* instance                                  \
                                       , size_t idx, value_type defaultValue) {                    \
    thunk_type* thunk = name##_at(instance, idx);                                                  \
    return (NULL == thunk)?defaultValue:get_value(thunk);                                          \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE value_type name##_get_value(const name##_t* instance                                 \
                       , key_type key, value_type defaultValue) {                                  \
    thunk_type* thunk = name##_get(instance, key);                                                 \
    return (NULL == thunk)?defaultValue:get_value(thunk);                                          \
}                                                                                                  \
                                                                                                   \
HASH_VARIABLE void name##_put_value(name##_t* instance, key_type key, value_type val) {            \
                                                                                                   \
    thunk_type* new_el;                                                                            \
    HASH_FIND_VALUE(thunk_type, key, get_key, get_next, hash, cmp)                                 \
                                                                                                   \
    if(el) {                                                                                       \
        del_key(get_key(el));                                                                      \
        del_value(get_value(el));                                                                  \
        set_key(el, key);                                                                          \
        set_value(el, val);                                                                        \
        return;                                                                                    \
    }                                                                                              \
                                                                                                   \
    new_el = thunk_new();                                                                          \
    set_key(new_el, key);                                                                          \
    set_value(new_el, val);                                                                        \
    set_next(new_el, NULL);                                                                        \
                                                                                                   \
    HASH_SET_NEW_VALUE(new_el, set_next);                                                          \
	                                                                                               \
	if(0 != instance->ar) {                                                                        \
		my_free(instance->ar);                                                                     \
		instance->ar = 0;                                                                          \
	}                                                                                              \
                                                                                                   \
    instance->used++;                                                                              \
                                                                                                   \
    if ((double) instance->used / instance->capacity > HASHTABLE_MAX_USAGE) {                      \
        _##name##_resize(instance);                                                                \
    }                                                                                              \
    return;                                                                                        \
}




#define default_get_key(x)  x->ref.key
#define default_set_key(x, k)  x->ref.key = k
#define default_get_value(x)  x->ref.value
#define default_set_value(x, v)  x->ref.value = v
#define default_get_next(x)  x->next
#define default_set_next(x, n)  x->next = n

#define HASH_DEFINE(HASH_VARIABLE                                                                  \
        , name                                                                                     \
		, key_type                                                                                 \
		, value_type                                                                               \
		, hash                                                                                     \
		, cmp                                                                                      \
        , del_key                                                                                  \
        , del_value)                                                                               \
static void _##name##hash_trunk_del(name##_thunk_t* thunk) {                                       \
    del_key(default_get_key(thunk));                                                               \
    del_value(default_get_value(thunk));                                                           \
    my_free(thunk);                                                                                \
}                                                                                                  \
static name##_thunk_t* _##name##hash_trunk_new( ) {                                                \
    return (name##_thunk_t*)my_malloc(sizeof(name##_thunk_t));                                     \
}                                                                                                  \
HASH_DEFINE_FULL(HASH_VARIABLE                                                                     \
        , name                                                                                     \
		, key_type                                                                                 \
		, value_type                                                                               \
		, name##_thunk_t                                                                           \
		, default_get_key                                                                          \
		, default_set_key                                                                          \
		, default_get_value                                                                        \
		, default_set_value                                                                        \
		, default_get_next                                                                         \
		, default_set_next                                                                         \
		, hash                                                                                     \
		, cmp                                                                                      \
        , del_key                                                                                  \
        , del_value                                                                                \
        , _##name##hash_trunk_new                                                                  \
        , _##name##hash_trunk_del)




DLL_VARIABLE int mj_cmp_string(const char* key1, const char* key2);
DLL_VARIABLE size_t mj_hash_string(const char *key);
DLL_VARIABLE int mj_mem_cmp(const void* key1, size_t len1, const void* key2, size_t len2);
DLL_VARIABLE size_t mj_mem_hash(const void *vkey, size_t len);

#ifdef __cplusplus
}
#endif

#endif