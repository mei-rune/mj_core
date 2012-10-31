#ifndef _variables_private_h_
#define _variables_private_h_ 1

#include "meijing/platform.h"
#include "meijing/array.h"
#include "meijing/variables.h"
#include "internal.h"

#ifdef __cplusplus
extern "C" {
#endif
	
extern const char *object_number_chars;
extern const char *object_hex_chars;

typedef struct object_keyvalue_s object_keyvalue_t;

typedef struct object_hashtable_s {

	/** hash表内块大小 */
	size_t capacity;
	
	/** hash表内当前对象数. */
	size_t used;

	/** hash表内当前对象块指针 */
	mj_variables_t** entries;

	/** 用于 toArray() 函数保存临时数据的 */
	mj_variables_t** array;

} object_hashtable_t;



typedef struct object_hash_s
{
  boolean is_kv;
  union 
  {
    object_hashtable_t* c_object;
    object_keyvalue_t* c_kv;
  } c_un;
} object_hash_t;


typedef struct object_array_s
{
  ARRAY_DECLARE(mj_variables_t*);
} object_array_t;

struct mj_variables_s
{
  enum mj_variable_type o_type;
  mj_string_t _buffer;

  union data {
	struct {
		int number_type;
#define NUMBER_NAN              0
#define NUMBER_SIGNED_INTEGER   1
#define NUMBER_UNSIGNED_INTEGER 2
#define NUMBER_DOUBLE           3
#define NUMBER_MINUSINFINITY    4
#define NUMBER_POSITIVEINFINITY 5
		union {
			double c_double;
			int64_t c_int;
			uint64_t c_uint;
		} value;
	} c_number;
	boolean c_boolean;
    mj_string_t c_string;
    object_hash_t c_hash;
    object_array_t c_array;   
  } o;
};

struct object_keyvalue_s
{
  mj_variables_t key;
  mj_variables_t* value;
  
  mj_variables_t* next;
};




#define ENSURE(o, t, r) if(0 == o || o->o_type != t) return r



typedef struct mj_variables_type_fn_s
{
  cstring_t type;
  void (*destroy)(mj_variables_t *obj);
  void (*finialize)(mj_variables_t *obj);
  void (*clear)(mj_variables_t *obj);

  boolean (*element_set)(mj_variables_t *obj, size_t idx, mj_variables_t *val);
  boolean (*element_del)(mj_variables_t *obj, size_t idx);
  mj_variables_t* (*element_at)(mj_variables_t *obj, size_t idx);
  size_t (*length)(mj_variables_t *obj);
  void (*print)(mj_variables_t *obj);
	
} mj_variables_type_fn_t;


extern mj_variables_type_fn_t mj_meta_objects[];

void _mj_variables_default_destroy(mj_variables_t *obj);
cstring_t _mj_variables_to_print_string(mj_variables_t *obj);

object_hashtable_t* _mj_variables_hashtable_create(size_t size);
void _mj_variables_table_put(mj_variables_t* hash, const char* key, size_t key_len, mj_variables_t *val);
boolean _mj_variables_table_del(mj_variables_t* hash, const char* key, size_t key_len);
mj_variables_t*  _mj_variables_table_get(const mj_variables_t* hash, const char* key, size_t key_len);
mj_variables_t*  _mj_variables_table_elementAt(mj_variables_t* hash, size_t idx);
void _mj_variables_table_clear(mj_variables_t* hash);
void _mj_variables_table_destroy(mj_variables_t* hash);
size_t _mj_variables_table_length(mj_variables_t* hash);
void _mj_variables_table_print(mj_variables_t *obj);

void _mj_variables_array_destroy(mj_variables_t *obj);
size_t _mj_variables_array_length(mj_variables_t *obj);
void _mj_variables_array_clear(mj_variables_t *obj);
boolean _mj_variables_array_set(mj_variables_t *obj, size_t idx, mj_variables_t* val);
boolean _mj_variables_array_del(mj_variables_t *obj, size_t idx);
mj_variables_t* _mj_variables_array_at(mj_variables_t *obj, size_t idx);
void _mj_variables_array_print(mj_variables_t *obj);
void _mj_variables_collection_print(mj_variables_t *obj
	, const char* startTag, size_t startLen
	, const char* stopTag, size_t stopLen);

void _mj_variables_string_destroy(mj_variables_t* obj);
void _mj_variables_string_clear(mj_variables_t* obj);
size_t _mj_variables_string_length(mj_variables_t* obj);
void _mj_variables_string_print(mj_variables_t *obj);

#ifdef __cplusplus
}
#endif

#endif
