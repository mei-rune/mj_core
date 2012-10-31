#ifndef _mj_variables_h_
#define _mj_variables_h_ 1


#include <stddef.h>
#include "meijing/platform.h"
#include "meijing/string_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JSON_OBJECT_DEF_HASH_ENTRIES 16

typedef struct mj_variables_s mj_variables_t;
typedef void (*format_fn_t)(const void* buf, size_t len, void* ctxt);

/* 支持的对象类型 */

typedef enum mj_variable_type {
  mj_variable_type_nil,
  mj_variable_type_boolean,
  mj_variable_type_number,
  mj_variable_type_string,
  mj_variable_type_table,
  mj_variable_type_list,
} mj_variable_type;


/**
 * 检测对象是不是指定的类型
 */
DLL_VARIABLE boolean mj_variables_typeof(mj_variables_t *obj, enum mj_variable_type type);

/**
 * 检测对象是不是原生类型(即非用户自定义类型)
 */
DLL_VARIABLE boolean mj_variables_is_primary(mj_variables_t *obj);

/**
 * 返回对象的类型
 */
DLL_VARIABLE enum mj_variable_type mj_variables_get_type(mj_variables_t *obj);

/**
 * 返回对象的类型
 */
DLL_VARIABLE const char* mj_variables_get_type_name(mj_variables_t *obj);

/**
 * 格式化输出对象
 */
DLL_VARIABLE void mj_variables_format(mj_variables_t* obj, format_fn_t cb, void* ctxt);

/**
 * @defgroup new 对象创建系列函数
 * 
 * 创建一个对象, 对象的类型根据 mj_variables_* 名的后缀决定
 * 
 * @{
 * 
 * @param[in] b 为对象的布尔值
 * @param[in] i 为对象的整型值
 * @param[in] ui 为对象的无符号整型值
 * @param[in] d 为对象的 double 值
 * @param[in] s 为对象的字符串值
 * @param[in] len 字符串的长度
 * @returns 返回类型为 mj_variable_type_string 的对象实例
 */
DLL_VARIABLE mj_variables_t* mj_variables_new_NaN();
DLL_VARIABLE mj_variables_t* mj_variables_new_Nil();;
DLL_VARIABLE mj_variables_t* mj_variables_new_minusInfinity();
DLL_VARIABLE mj_variables_t* mj_variables_new_positiveInfinity();
DLL_VARIABLE mj_variables_t* mj_variables_new_table(void);
DLL_VARIABLE mj_variables_t* mj_variables_new_list(void);
DLL_VARIABLE mj_variables_t* mj_variables_new_boolean(boolean b);
DLL_VARIABLE mj_variables_t* mj_variables_new_int(int64_t i);
DLL_VARIABLE mj_variables_t* mj_variables_new_uint(uint64_t ui);
DLL_VARIABLE mj_variables_t* mj_variables_new_double(double d);
DLL_VARIABLE mj_variables_t* mj_variables_new_string(const char *s);
DLL_VARIABLE mj_variables_t* mj_variables_new_stringLen(const char *s, size_t len);
DLL_VARIABLE mj_variables_t* mj_variables_new_stringN(char c, size_t n);
DLL_VARIABLE mj_variables_t* mj_variables_new_string_zerocopy(char *s, size_t len);
DLL_VARIABLE mj_variables_t* mj_variables_new_zerocopy(const mj_string_t* str);
/**
 * 删除对象
 */
DLL_VARIABLE void mj_variables_destory(mj_variables_t *obj);

/** @} */

/**
 * @defgroup to 对象的值转换函数
 * 
 * @{
 */
DLL_VARIABLE boolean    mj_variables_is_NaN(mj_variables_t *obj);
DLL_VARIABLE boolean    mj_variables_is_Nil(mj_variables_t *obj);
DLL_VARIABLE boolean    mj_variables_is_minusInfinity(mj_variables_t *obj);
DLL_VARIABLE boolean    mj_variables_is_positiveInfinity(mj_variables_t *obj);
DLL_VARIABLE boolean    mj_variables_to_boolean(mj_variables_t *obj, boolean defaultValue);
DLL_VARIABLE int64_t      mj_variables_to_int(mj_variables_t *obj, int64_t defaultValue);
DLL_VARIABLE uint64_t     mj_variables_to_uint(mj_variables_t *obj, uint64_t defaultValue);
DLL_VARIABLE double     mj_variables_to_double(mj_variables_t *obj, double defaultValue);
DLL_VARIABLE cstring_t  mj_variables_to_string(mj_variables_t *obj, const char* defaultValue, size_t len);

/** @} */


/**
 * @defgroup put put 系列函数
 * 
 * 向 map 类型的对象添加一个字段
 * 
 * @{
 *
 * 注意: 这个操作后 val 对象的所有者变为 obj
 *
 * @param[in] obj map 类型对象的实例
 * @param[in] key 新字段的字段名, 注意如果已经有的话会覆盖旧值
 * @param[in] val 新字段的值
 */
DLL_VARIABLE boolean mj_variables_put_object(mj_variables_t* obj, const char *key, mj_variables_t *val);
DLL_VARIABLE boolean mj_variables_put_int(mj_variables_t *obj, const char *key, int64_t val);
DLL_VARIABLE boolean mj_variables_put_uint(mj_variables_t *obj, const char *key, uint64_t val);
DLL_VARIABLE boolean mj_variables_put_double(mj_variables_t *obj, const char *key, double val);
DLL_VARIABLE boolean mj_variables_put_boolean(mj_variables_t *obj, const char *key, boolean val);
DLL_VARIABLE boolean mj_variables_put_string(mj_variables_t *obj, const char *key, const char* val);
DLL_VARIABLE boolean mj_variables_put_stringLen(mj_variables_t *obj, const char *key, const char* val, size_t len);
DLL_VARIABLE boolean mj_variables_put_stringN(mj_variables_t *obj, const char *key, char val, size_t n);
DLL_VARIABLE boolean mj_variables_put_string_zerocopy(mj_variables_t *obj, const char *key, char* val, size_t len);
DLL_VARIABLE boolean mj_variables_put_zerocopy(mj_variables_t *obj, const char *key, const mj_string_t* val);

/** @} */

/**
 * @defgroup get get 系列函数
 * 
 * 获取对象实例中指定的字段
 * 
 * @{
 *
 * @param[in] obj 对象的实例
 * @param[in] key 字段名
 * @returns 返回指定字段的值
 */
DLL_VARIABLE mj_variables_t*  mj_variables_get_object(mj_variables_t* obj, const char *key);
DLL_VARIABLE boolean mj_variables_get_boolean(mj_variables_t *obj, const char *key, boolean defaultValue);
DLL_VARIABLE int64_t      mj_variables_get_int(mj_variables_t *obj, const char *key, int64_t defaultValue);
DLL_VARIABLE uint64_t     mj_variables_get_uint(mj_variables_t *obj, const char *key, uint64_t defaultValue);
DLL_VARIABLE double     mj_variables_get_double(mj_variables_t *obj, const char *key, double defaultValue);
DLL_VARIABLE cstring_t  mj_variables_get_string(mj_variables_t *obj, const char *key, const char* defaultValue, size_t len);

/** @} */

/** 
 * 删除对象实例中指定的字段
 * @param[in] obj 对象的实例
 * @param[in] key 字段名
 * @returns 删除总是成功的, 返回值为 true 表示找到并删除
 * , 为 false 时表示没有找到这个字段
 */
DLL_VARIABLE boolean mj_variables_del_object(mj_variables_t* obj, const char *key);


/** 
 * 获取对象内部子对象的个数
 * 
 * 注意, 只有 object_type_object 和 mj_variable_type_list 两种类型的对象
 * 有 length 属性,其它类型的对象返回 0
 * 
 * @param[in] obj 对象的实例
 */
DLL_VARIABLE size_t mj_variables_length(mj_variables_t *obj);

/** 
 * 删除对象内部所有子对象或字段
 * 
 * @param[in] obj 对象的实例
 */
DLL_VARIABLE void mj_variables_clear(mj_variables_t *obj);

/**
 * @defgroup push push 系列函数
 * 
 * 在类型为 mj_variable_type_list 的对象实例的尾部增加一个子对象
 * 
 * @{
 *
 * @param[in] obj 对象的实例
 * @param[in] val 被添加的子对象
 */
DLL_VARIABLE boolean mj_variables_element_push(mj_variables_t *obj, mj_variables_t *val);
DLL_VARIABLE boolean mj_variables_element_push_boolean(mj_variables_t *obj, boolean val);
DLL_VARIABLE boolean mj_variables_element_push_int(mj_variables_t *obj, int64_t val);
DLL_VARIABLE boolean mj_variables_element_push_uint(mj_variables_t *obj, uint64_t val);
DLL_VARIABLE boolean mj_variables_element_push_double(mj_variables_t *obj, double val);
DLL_VARIABLE boolean mj_variables_element_push_string(mj_variables_t *obj, const char* val);
DLL_VARIABLE boolean mj_variables_element_push_stringLen(mj_variables_t *obj, const char* val, size_t len);
DLL_VARIABLE boolean mj_variables_element_push_stringN(mj_variables_t *obj, char val, size_t n);
DLL_VARIABLE boolean mj_variables_element_push_string_zerocopy(mj_variables_t *obj, char* val, size_t len);
DLL_VARIABLE boolean mj_variables_element_push_zerocopy(mj_variables_t *obj, const mj_string_t* val);


/** @} */

/** 
 * @defgroup pop pop 系列函数
 * 
 * 从类型为 mj_variable_type_list 的对象实例的头部移出一个子对象
 *
 * 注意, 当 pop 出一个对象时, 在你使用完成后, 需要你调用 mj_variables_destory 清理它
 *
 * @{
 *
 * @param[in] obj 对象的实例
 * @param[in] val 被添加的子对象
 */
DLL_VARIABLE mj_variables_t*  mj_variables_element_pop(mj_variables_t *obj);
DLL_VARIABLE boolean               mj_variables_element_pop_boolean(mj_variables_t *obj, boolean defaultValue);
DLL_VARIABLE int64_t                 mj_variables_element_pop_int(mj_variables_t *obj, int64_t defaultValue);
DLL_VARIABLE uint64_t                mj_variables_element_pop_uint(mj_variables_t *obj, uint64_t defaultValue);
DLL_VARIABLE double                mj_variables_element_pop_double(mj_variables_t *obj, double defaultValue);
DLL_VARIABLE mj_string_t              mj_variables_element_pop_string(mj_variables_t *obj);

/** @} */

/** 
 *  @defgroup set set 系列函数
 * 
 * 将类型为 mj_variable_type_list 的对象实例的第 idx 个对象更改为指定的子对象
 *
 * 注意如果原位置有子对象时,会自动清理
 *
 * @{
 *
 * @param[in] obj 对象的实例
 * @param[in] idx 位置
 * @param[in] val 被添加的子对象
 */
DLL_VARIABLE boolean  mj_variables_element_set(mj_variables_t *obj, size_t idx, mj_variables_t *val);
DLL_VARIABLE boolean  mj_variables_element_set_boolean(mj_variables_t *obj, size_t idx, boolean value);
DLL_VARIABLE boolean  mj_variables_element_set_int(mj_variables_t *obj, size_t idx, int64_t value);
DLL_VARIABLE boolean  mj_variables_element_set_uint(mj_variables_t *obj, size_t idx, uint64_t value);
DLL_VARIABLE boolean  mj_variables_element_set_double(mj_variables_t *obj, size_t idx, double value);
DLL_VARIABLE boolean  mj_variables_element_set_string(mj_variables_t *obj, size_t idx, const char* str);
DLL_VARIABLE boolean  mj_variables_element_set_stringLen(mj_variables_t *obj, size_t idx, const char* str, size_t len);
DLL_VARIABLE boolean  mj_variables_element_set_stringN(mj_variables_t *obj, size_t idx, char c, size_t n);
DLL_VARIABLE boolean  mj_variables_element_set_string_zerocopy(mj_variables_t *obj, size_t idx, char* str, size_t len);
DLL_VARIABLE boolean  mj_variables_element_set_zerocopy(mj_variables_t *obj, size_t idx, const mj_string_t* str);

/** @} */

/** 
 * @defgroup at at 系列函数
 * 
 * 获取类型为 mj_variable_type_list 或 object_type_object 的对象实例中第 idx 个对象
 *
 * 注意如果对象为 object_type_object 时, 返回的是一个 key/value 对
 *
 * @{
 *
 * @param[in] obj 对象的实例
 * @param[in] idx 位置
 */
DLL_VARIABLE mj_variables_t*  mj_variables_element_at(mj_variables_t *obj, size_t idx);
DLL_VARIABLE int64_t          mj_variables_element_at_int(mj_variables_t *obj, size_t idx, int64_t defaultValue);
DLL_VARIABLE uint64_t         mj_variables_element_at_uint(mj_variables_t *obj, size_t idx, uint64_t defaultValue);
DLL_VARIABLE double           mj_variables_element_at_double(mj_variables_t *obj, size_t idx, double defaultValue);
DLL_VARIABLE boolean          mj_variables_element_at_boolean(mj_variables_t *obj, size_t idx, boolean defaultValue);
DLL_VARIABLE cstring_t        mj_variables_element_at_string(mj_variables_t *obj, size_t idx, const char* val, size_t len);

/** @} */

/**
 * 删除类型为 mj_variable_type_list 的对象实例的第 idx 个对象
 *
 * 注意如果原位置有子对象时,会自动清理
 *
 * @param[in] obj 对象的实例
 * @param[in] idx 位置
 */
DLL_VARIABLE boolean mj_variables_element_del(mj_variables_t *obj, size_t idx);


#ifdef __cplusplus
}
#endif

#endif
