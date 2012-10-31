
#ifndef _mj_rpc_h_
#define _mj_rpc_h_ 1

#include <stdarg.h>
#include "meijing/platform.h"
#include "meijing/variables.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mj_rpc_server_s   mj_rpc_server_t;
typedef struct mj_rpc_context_s  mj_rpc_context_t;
typedef struct mj_rpc_method_s   mj_rpc_method_t;


typedef enum mj_rpc_result_e {
	PIPELINE_STATUS_CONTINUE       = 0,
	PIPELINE_STATUS_OK           = 1,
} mj_rpc_result_t;


typedef void (*mj_rpc_complete_t)(mj_variables_t *, mj_variables_t *
	, mj_rpc_context_t*, mj_rpc_result_t result);

typedef mj_rpc_result_t (*mj_rpc_callback_t)(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext);


struct mj_rpc_method_s
{
	mj_rpc_server_t*  server;
	mj_string_t       action;
	mj_string_t       descriptor;
	mj_rpc_callback_t hook;
	void*                  usrContext;
};

struct mj_rpc_context_s
{
	mj_rpc_method_t*  method;
	mj_rpc_context_t* pipeline;
	void*                  usrContext;
};


/**
 * 创建一个 rpc 服务
 */
DLL_VARIABLE mj_rpc_server_t* mj_rpc_create();

/**
 * 销毁 rpc 服务
 */
DLL_VARIABLE void mj_rpc_free(mj_rpc_server_t* server);

/**
 * 注册一个方法. 
 * @param methodName 方法名
 * @param descriptor 方法的描述
 * @param method 方法指针
 * @param usrContext 方法上下文
 */
DLL_VARIABLE void mj_rpc_add_method (mj_rpc_server_t* server
	, const char* methodName
	, const char* descriptor
	, mj_rpc_callback_t callback
	, void* usrContext);
/**
 * 注销一个方法. 
 * @param methodName 方法名
 */
DLL_VARIABLE void mj_rpc_remove_method(mj_rpc_server_t* server, const char* methodName);

/**
 * 设置系统内部事件缓冲区的大小.
 */
DLL_VARIABLE void mj_rpc_set_event_list_size(mj_rpc_server_t* server, int size);

/**
 * 增加一个内部事件 
 * @param eventType 事件类型.
 * @param eventData 事件的数据.
 *        
 */
DLL_VARIABLE void mj_rpc_add_event(mj_rpc_server_t* server
    , mj_variables_t *eventData);

/*
 * 调用 json 服务
 * @param request 一个含有方法名和方法参数的 json 对象
 * @param response 含有服务返回值的 json 对象
 */
DLL_VARIABLE mj_rpc_result_t mj_rpc_service(mj_rpc_server_t* server
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx);

/*
 * 以流水线方式依次调用所有匹配 pattern 的 json 服务
 * @param request 一个含有方法名和方法参数的 json 对象
 * @param response 含有服务返回值的 json 对象
 */
DLL_VARIABLE mj_rpc_result_t mj_rpc_call(mj_rpc_server_t* server
	, const char* method_name
	, size_t name_len
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx);

/*
 * 以流水线方式依次调用所有匹配 pattern 的 json 服务
 * @param request 一个含有方法名和方法参数的 json 对象
 * @param response 含有服务返回值的 json 对象
 */
DLL_VARIABLE mj_rpc_result_t mj_rpc_async_call(mj_rpc_server_t* server
	, const char* method_name
	, size_t name_len
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx
	, mj_rpc_complete_t on_complete);

/*
 * 以流水线方式依次调用所有匹配 pattern 的 json 服务
 * @param request 一个含有方法名和方法参数的 json 对象
 * @param response 含有服务返回值的 json 对象
 */
DLL_VARIABLE mj_rpc_result_t mj_rpc_invoke(mj_rpc_server_t* server
	, const char* pattern
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx
	, boolean alway_continue);

/**
 * 列出所有方法. 
 */
DLL_VARIABLE mj_variables_t* mj_rpc_list_methods(mj_rpc_server_t* server);

/**
 * 在指定的 json 对象中构造一个异常
 * @param response  json 对象
 * @param code  异常代码
 * @param err  异常描述
 */
DLL_VARIABLE void mj_rpc_constructException(mj_variables_t *response
	, const char* code
	, const char* err
	, ...);


#define throw_exception1(response, exception, err) mj_rpc_constructException(response, exception, err)
#define throw_exception2(response, exception, fmt, a1) mj_rpc_constructException(response, exception, fmt, a1)
#define throw_exception3(response, exception, fmt, a1, a2) mj_rpc_constructException(response, exception, fmt, a1, a2)
#define throw_exception4(response, exception, fmt, a1, a2, a3) mj_rpc_constructException(response, exception, fmt, a1, a2, a3)
#define throw_exception5(response, exception, fmt, a1, a2, a3, a4) mj_rpc_constructException(response, exception, fmt, a1, a2, a3, a4)


#ifdef __cplusplus
};
#endif

#endif
	
