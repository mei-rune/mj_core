
 
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "meijing/json.h"
#include <yajl/yajl_parse.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define STATUS_CONTINUE 1
#define STATUS_ABORT    0
	
typedef struct mj_json_stack_elem_s {
    mj_variables_t* key;
    mj_variables_t* value;
    struct mj_json_stack_elem_s *next;
} mj_json_stack_elem_t;

typedef struct mj_json_context_s {
    mj_json_stack_elem_t *stack;
    mj_variables_t* root;
    mj_string_t errbuf;
} mj_json_context_t;


#define RETURN_ERROR(ctx,...) RETURN_ERROR_WITH(ctx, false, __VA_ARGS__)
	
#define RETURN_ERROR_WITH(ctx, retval, ...) {                                \
        if ((ctx)->errbuf.str != NULL)                                       \
            (ctx)->errbuf.len = snprintf ((ctx)->errbuf.str                  \
                                      , (ctx)->errbuf.len, __VA_ARGS__);     \
        return (retval);                                                     \
    }

static boolean _mj_yajl_context_push(mj_json_context_t *ctx, mj_variables_t* v)
{
    mj_json_stack_elem_t *stack;

    stack = (mj_json_stack_elem_t*)my_malloc (sizeof (*stack));
    if (stack == NULL)
        RETURN_ERROR (ctx, "Out of memory");

    memset (stack, 0, sizeof (*stack));

    assert ((ctx->stack == NULL)
            || !mj_variables_is_primary (v));

    stack->value = v;
    stack->next = ctx->stack;
    ctx->stack = stack;

    return (true);
}

static mj_variables_t* _mj_yajl_context_pop(mj_json_context_t *ctx)
{
    mj_json_stack_elem_t *stack;
    mj_variables_t* v;

    if (ctx->stack == NULL)
        RETURN_ERROR (ctx, NULL, "_mj_yajl_context_pop: "
                      "Bottom of stack reached prematurely");

    stack = ctx->stack;
    ctx->stack = stack->next;

    v = stack->value;

    my_free (stack);

    return (v);
}


/*
 * Add a value to the value on top of the stack or the "root" member in the
 * context if the end of the parsing process is reached.
 */
static boolean context_add_value (mj_json_context_t *ctx, mj_variables_t* v)
{
    /* We're checking for NULL values in all the calling functions. */
    assert (ctx != NULL);
    assert (v != NULL);

    if (ctx->stack == NULL)
    {
        assert (ctx->root == NULL);
        ctx->root = v;
        return (true);
    }
    else if (mj_variables_typeof (ctx->stack->value, mj_variable_type_table))
    {
        if (ctx->stack->key == NULL)
        {
            if  (!mj_variables_typeof (v, mj_variable_type_string)) {
                RETURN_ERROR (ctx, "context_add_value: "
                              "Object key is not a string (%s)",
								mj_variables_get_type_name(v));
			}

			ctx->stack->key = v;
            return (true);
        }
        else /* if (ctx->key != NULL) */
        {
            const char * key;
			boolean      retval;

			key = mj_variables_to_string(ctx->stack->key, 0, 0).str;
            retval = (mj_variables_put_object(ctx->stack->value, key, v));
			
			mj_variables_destory(ctx->stack->key);
            ctx->stack->key = NULL;
			return retval;
        }
    }
    else if (mj_variables_typeof (ctx->stack->value, mj_variable_type_list))
    {
		return (mj_variables_element_push(ctx->stack->value, v));
    }
    else
    {
        RETURN_ERROR (ctx, "context_add_value: Cannot add value to "
                      "a value of type %s (not a composite type)",
                      mj_variables_get_type_name(ctx->stack->value));
    }
}

static int _mj_yajl_null(void * ctx)
{
    mj_json_context_t* ctx_ = (mj_json_context_t*) ctx;
    return ((context_add_value (ctx_, mj_variables_new_Nil())) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_boolean(void * ctx, int value)
{
    mj_json_context_t* ctx_ = (mj_json_context_t*) ctx;
    return ((context_add_value (ctx_, mj_variables_new_boolean(value))) ? STATUS_CONTINUE : STATUS_ABORT);
}
static int _mj_yajl_integer(void * ctx, long long integerVal) {
    mj_json_context_t* ctx_ = (mj_json_context_t*) ctx;
    return ((context_add_value (ctx_, mj_variables_new_int(integerVal))) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_double(void * ctx, double doubleVal) {
    mj_json_context_t* ctx_ = (mj_json_context_t*) ctx;
    return ((context_add_value (ctx_, mj_variables_new_double(doubleVal))) ? STATUS_CONTINUE : STATUS_ABORT);
}

//static int _mj_yajl_number(void * ctx, const char * s, size_t l) {
//    mj_json_context_t* ctx_ = (mj_json_context_t*) ctx;
//    return ((context_add_value (ctx_, mj_variables_new_stringLen(s, l))) ? STATUS_CONTINUE : STATUS_ABORT);
//}

static int _mj_yajl_string(void * ctx, const unsigned char * stringVal,
                           size_t stringLen) {
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = mj_variables_new_stringLen((const char*)stringVal, stringLen);

    return ((context_add_value (ctx_, v)) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_map_key(void * ctx, const unsigned char * stringVal,
                            size_t stringLen)
{
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = mj_variables_new_stringLen((const char*)stringVal, stringLen);
    return ((context_add_value (ctx_, v)) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_start_map(void * ctx) {
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = mj_variables_new_table();
    if (v == NULL)
        RETURN_ERROR_WITH (ctx_, STATUS_ABORT, "Out of memory");

    return (_mj_yajl_context_push (ctx_, v) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_end_map(void * ctx) {
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = _mj_yajl_context_pop (ctx_);

    if (v == NULL)
        return (STATUS_ABORT);

    return (context_add_value (ctx_, v) ? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_start_array(void * ctx) {
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = mj_variables_new_list();
    if (v == NULL)
        RETURN_ERROR_WITH (ctx_, STATUS_ABORT, "Out of memory");

    return (_mj_yajl_context_push (ctx_, v)? STATUS_CONTINUE : STATUS_ABORT);
}

static int _mj_yajl_end_array(void * ctx) {
    mj_json_context_t* ctx_;
	mj_variables_t* v;

	ctx_ = (mj_json_context_t*) ctx;
	v = _mj_yajl_context_pop (ctx_);

    if (v == NULL)
        return (STATUS_ABORT);

    return (context_add_value (ctx_, v)? STATUS_CONTINUE : STATUS_ABORT);
}

static yajl_callbacks _mj_yajl_callbacks = {
    _mj_yajl_null,
    _mj_yajl_boolean,
    _mj_yajl_integer,
    _mj_yajl_double,
    NULL, //_mj_yajl_number,
    _mj_yajl_string,
    _mj_yajl_start_map,
    _mj_yajl_map_key,
    _mj_yajl_end_map,
    _mj_yajl_start_array,
    _mj_yajl_end_array
};

DLL_VARIABLE mj_variables_t* mj_json_parse_from_string(const char* input, size_t len,
                          char *error_buffer, size_t error_buffer_size) {
    
    yajl_handle handle;
    yajl_status status;
	mj_json_context_t ctx = { NULL, NULL, {0, NULL} };

	ctx.errbuf.str = error_buffer;
	ctx.errbuf.len = error_buffer_size;

    if (error_buffer != NULL)
        memset (error_buffer, 0, error_buffer_size);

    handle = yajl_alloc (&_mj_yajl_callbacks, NULL, &ctx);
    yajl_config(handle, yajl_allow_comments, 1);

    status = yajl_parse(handle,
                        (const unsigned char *) input,
                        len);
    status = yajl_complete_parse (handle);
    if (status != yajl_status_ok) {
        if (error_buffer != NULL && error_buffer_size > 0) {
            snprintf(
                error_buffer, error_buffer_size, "%s",
                (char *) yajl_get_error(handle, 1,
                                        (const unsigned char *) input,
                                        len));
        }
        yajl_free (handle);
        return NULL;
    }

    yajl_free (handle);
    return (ctx.root);
}

#ifdef __cplusplus
}
#endif