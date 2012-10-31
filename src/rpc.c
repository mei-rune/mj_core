#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "meijing/hashtable.h"
#include "meijing/rpc.h"


#ifdef __cplusplus
extern "C" 
{
#endif

static void _rpc_method_key_free(const char* key) {
}

static void _rpc_method_body_free(mj_rpc_method_t* method) {
	my_free(method);
}



HASH_DECLARE(static, mj_rpc_methods, const char*, mj_rpc_method_t*);

HASH_DEFINE(static
	, mj_rpc_methods
	, const char*
	, mj_rpc_method_t*
	, mj_hash_string
	, mj_cmp_string
	, _rpc_method_key_free
	, _rpc_method_body_free)    



struct mj_rpc_server_s
{
	mj_rpc_methods_t methods;
	mj_variables_t* eventBus;
	size_t eventLength;
};


static char* safe_tolower(char* str)
{
	char* p = str;
	if(0 == str)
		return 0;

	while(0 != *p)
	{
		*p = tolower(*p);
		++p;
	}
	return str;
}

#define safe_strdup(str) ((0 == str)?0:strdup(str))
#define safe_strlen(str) ((0 == str)?0:strlen(str))

/**
 * 创建一个 rpc 服务
 */
DLL_VARIABLE mj_rpc_server_t* mj_rpc_create()
{
	mj_rpc_server_t* server = (mj_rpc_server_t*)
		my_calloc(1, sizeof(mj_rpc_server_t));
	mj_rpc_methods_init(&(server->methods), 1000);
	return server;
}

/**
 * 销毁 rpc 服务
 */
DLL_VARIABLE void mj_rpc_free(mj_rpc_server_t* server)
{
	mj_rpc_methods_destroy(&(server->methods));
	mj_variables_destory(server->eventBus);
	my_free(server);
}


DLL_VARIABLE void mj_rpc_add_method(mj_rpc_server_t* server, const char* methodName
	, const char* descriptor, mj_rpc_callback_t callback,	void* usrContext)
{
	mj_rpc_method_t* method;
	size_t methodLen;
	size_t descriptorlen;

	assert(0 != methodName);
	assert(0 != callback);

	methodLen = safe_strlen(methodName);
	descriptorlen = safe_strlen(descriptor);
	
	method = (mj_rpc_method_t*)malloc(MEM_ALIGNMENT(sizeof(mj_rpc_method_t)) + MEM_ALIGNMENT(methodLen+4) + MEM_ALIGNMENT(descriptorlen+4));
	method->server = server;
	method->action.str = ((char*)method) + MEM_ALIGNMENT(sizeof(mj_rpc_method_t));
	method->action.len = methodLen;
	method->descriptor.str = ((char*)method) + MEM_ALIGNMENT(sizeof(mj_rpc_method_t)) + MEM_ALIGNMENT(methodLen+4);
	method->descriptor.len = descriptorlen;
	method->hook = callback;
	method->usrContext = usrContext;

	memcpy(method->action.str, methodName, methodLen + 1);

	if(0 != descriptor) {
		memcpy(method->descriptor.str, descriptor, descriptorlen + 1);
	} else {
		*(method->descriptor.str) = 0;
	}

	mj_rpc_methods_put_value(&(server->methods), method->action.str, method);
}

DLL_VARIABLE void mj_rpc_remove_method(mj_rpc_server_t* server, const char* methodName)
{
	mj_rpc_methods_del(&(server->methods), methodName);
}

DLL_VARIABLE void mj_rpc_system_list_methods(mj_rpc_server_t* server, 
	mj_variables_t *request, mj_variables_t *response)
{
	mj_variables_put_object(response, "result", mj_rpc_list_methods(server));
}

DLL_VARIABLE void mj_rpc_system_events(mj_rpc_server_t* server, 
	mj_variables_t *request, mj_variables_t *response)
{
	if(server->eventBus)
	{
		mj_variables_put_object(response, "result", server->eventBus);
		server->eventBus = 0;
	}
	else
	{
		mj_variables_put_object(response, "result", mj_variables_new_list());
	}
}

static cstring_t mj_rpc_request_method(mj_variables_t *request)
{
	mj_variables_t *value;

	value = mj_variables_get_object(request, "action");
	return mj_variables_to_string(value, "", 0);
}

DLL_VARIABLE mj_rpc_result_t mj_rpc_service(mj_rpc_server_t* server
	, mj_variables_t *request, mj_variables_t *response, void* ctx)
{
	cstring_t methodName = mj_rpc_request_method(request);
	return mj_rpc_call(server, methodName.str, methodName.len, request, response, ctx);
}

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
	, void* ctx) {
	mj_rpc_async_call(server, method_name, name_len, request, response, ctx, NULL);
}
DLL_VARIABLE mj_rpc_result_t mj_rpc_async_call(mj_rpc_server_t* server
	, const char* method_name
	, size_t name_len
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx
	, mj_rpc_complete_t on_complete) {

	if(strcasecmp(method_name, "system.listMethods") == 0)
	{
	   mj_rpc_system_list_methods(server, request, response);
	}
	else if(strcasecmp(method_name, "system.events") == 0)
	{
	   mj_rpc_system_events(server, request, response);
	}
	else
	{
		mj_rpc_method_t* method = mj_rpc_methods_get_value(&(server->methods), method_name, 0);
		if(method)
		{
			mj_rpc_context_t context;
			context.method = method;
			context.pipeline = 0;
			context.usrContext = ctx;

			mj_variables_put_object(response, "action", mj_variables_new_stringLen(method_name,name_len));
			return (*(method->hook))(request, response, &context);
		}
		else
		{
			char buf[1025];
			buf[1024] = 0;
			snprintf(buf, 1024, "can`t find such method - '%s'.", method_name);
			mj_rpc_constructException(response, "SystemExeception", buf);
		}
	}
	return PIPELINE_STATUS_OK;
}

DLL_VARIABLE void mj_rpc_set_event_list_size(mj_rpc_server_t* server, int size)
{
	server->eventLength = size;
}

DLL_VARIABLE void mj_rpc_add_event(mj_rpc_server_t* server, mj_variables_t *eventData)
{
	if(!server->eventBus)
	{
		server->eventBus = mj_variables_new_list(); 
	}
	else if(mj_variables_length(server->eventBus) >= server->eventLength)
	{
		mj_variables_element_del(server->eventBus, 0);
	}
	
	mj_variables_element_push(server->eventBus, eventData);
}

DLL_VARIABLE mj_variables_t* mj_rpc_list_methods(mj_rpc_server_t* server)
{
	mj_variables_t *methodList = mj_variables_new_list();

	{
		mj_variables_t* listEvents_desc = mj_variables_new_table();

		mj_variables_put_string(
			listEvents_desc, 
			"system.events", 
			"list all events.");

		mj_variables_element_push(
			methodList,  
			listEvents_desc);
	}

	{	
		mj_variables_t* listMethods_desc = mj_variables_new_table();

		mj_variables_put_string(
			listMethods_desc, 
			"system.listMethods", 
			"list all methods.");

		mj_variables_element_push(
			methodList,  
			listMethods_desc);
	}
	{
		size_t i;
		size_t methodLen;
		
		methodLen = mj_rpc_methods_count(&(server->methods));
		for(i = 0; i < methodLen; ++ i) {
			mj_variables_t* desc;
			mj_rpc_method_t* method;

			desc = mj_variables_new_table();
			method = mj_rpc_methods_at_value(&(server->methods), i, 0);

			mj_variables_put_stringLen(
				desc, 
				method->action.str, 
				method->descriptor.str,
				method->descriptor.len);

			mj_variables_element_push(
				methodList, 
				desc);
		}
	}
	return methodList;
}

DLL_VARIABLE void mj_rpc_constructException(mj_variables_t *response
    , const char* code
    , const char* err
    , ...)
{
    size_t len;
    char* buf;
	mj_variables_t* error;

    va_list argList;
    va_start(argList, err);
    len = vscprintf(err, argList);
    if(len <= 0)
        len = 3*strlen(err);
    buf = malloc(len + 10);
    memset(&buf[len], 0, 10);
    snprintf(buf, len, err, argList);
	va_end( argList );

	error = mj_variables_new_table();
	mj_variables_put_object(error, "errorCode", mj_variables_new_string(code));
	mj_variables_put_object(error, "message", mj_variables_new_string(buf));
	mj_variables_put_object(response, "error", error);
	free(buf);
}

struct _simple_regex
{
	const char* str;
	size_t len;
};

int match_startwith(const char* name, size_t len, void* regex)
{
	struct _simple_regex* pattern = (struct _simple_regex*)regex;
	return (0 == strncasecmp(name, pattern->str, pattern->len))?1:0;
}

int match_endwith(const char* name, size_t len, void* regex)
{
	struct _simple_regex* pattern = (struct _simple_regex*)regex;
	if(len < pattern->len)
		return 0;

	return (0 == strncasecmp(name + len - pattern->len, pattern->str, pattern->len))?1:0;
}


DLL_VARIABLE mj_rpc_result_t mj_rpc_invoke(mj_rpc_server_t* server
	, const char* pattern
    , mj_variables_t *request
    , mj_variables_t *response
	, void* ctx
	, boolean alway_continue)
{
	typedef int (*match_fn_t)(const char* name, size_t len, void* regex);
	
	struct _simple_regex simple_regex;
	void* regex;
	match_fn_t match_fn;


	if(0 == strncasecmp("start:", pattern, 6))
	{
		match_fn = &match_startwith;
		regex = &simple_regex;

		simple_regex.str = pattern + 6;
		simple_regex.len = strlen(simple_regex.str);
	}
	else if(0 == strncasecmp("end:", pattern, 4))
	{
		match_fn = &match_endwith;
		regex = &simple_regex;

		simple_regex.str = pattern + 4;
		simple_regex.len = strlen(simple_regex.str);
	}
	else 
	{
		mj_rpc_constructException(response, "SystemExeception", "unsupport pattern!");
		return PIPELINE_STATUS_OK;
	}

	{
		mj_rpc_result_t  result;
		mj_rpc_context_t context;
		size_t                methodLen;
		size_t                i;

		result = PIPELINE_STATUS_OK;
		methodLen = mj_rpc_methods_count(&(server->methods));
		for(i = 0; i < methodLen; ++ i) {
			mj_rpc_method_t* method;

			method = mj_rpc_methods_at_value(&(server->methods), i, 0);

			if(0 == (*match_fn)(method->action.str, method->action.len, regex))
				continue;


			context.method = method;
			context.usrContext = ctx;
			context.pipeline = 0;

			result = (*(method->hook))(request, response, &context);

			if(alway_continue)
				continue;

			if(PIPELINE_STATUS_OK == result)
				break;
		}
		return result;
	}
}

#ifdef __cplusplus
};
#endif

