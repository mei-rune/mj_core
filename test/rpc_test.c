#include <assert.h>
#include "meijing/rpc.h"
#include "meijing/unittest.h"

TEST(rpc, empty)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
    mj_variables_t *error;
	mj_rpc_result_t result;
	
	server = mj_rpc_create();

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	mj_variables_put_string(request, "action", "this_is_empty");
	result = mj_rpc_service(server, request, response, NULL);

	ASSERT_EQ(result, PIPELINE_STATUS_OK);

	error = mj_variables_get_object(response, "error");
	ASSERT_STREQ("SystemExeception", mj_variables_get_string(error, "errorCode", 0, 0).str);
	ASSERT_STREQ("can`t find such method - 'this_is_empty'.", mj_variables_get_string(error, "message", 0, 0).str);

	mj_variables_destory(request);
	mj_variables_destory(response);
	mj_rpc_free(server);
}

mj_rpc_result_t simple_test_hook(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value", "ok");
	assert(1 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_OK;
}

TEST(rpc, simple)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
	mj_rpc_result_t result;
	const char* ok;

	server = mj_rpc_create();
	mj_rpc_add_method (server, "simple", 0, &simple_test_hook, (void*)1);

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	mj_variables_put_string(request, "action", "simple");
	result = mj_rpc_service(server, request, response, (void*)2);

	ASSERT_EQ(result, PIPELINE_STATUS_OK);

	ok = mj_variables_get_string(response, "value", 0, 0).str;
	ASSERT_STREQ("ok", ok);

	mj_variables_destory(request);
	mj_variables_destory(response);
	mj_rpc_remove_method(server, "simple");
	mj_rpc_free(server);
}


TEST(rpc, remove)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
    mj_variables_t *error;
	mj_rpc_result_t result;
	
	server = mj_rpc_create();
	mj_rpc_add_method (server, "simple", 0, &simple_test_hook, (void*)1);
	mj_rpc_remove_method(server, "simple");

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	mj_variables_put_string(request, "action", "simple");
	result = mj_rpc_service(server, request, response, (void*)2);

	error = mj_variables_get_object(response, "error");
	ASSERT_STREQ("SystemExeception", mj_variables_get_string(error, "errorCode", 0, 0).str);
	ASSERT_STREQ("can`t find such method - 'simple'.", mj_variables_get_string(error, "message", 0, 0).str);


	ASSERT_EQ(result, PIPELINE_STATUS_OK);

	mj_variables_destory(request);
	mj_variables_destory(response);
	mj_rpc_free(server);
}

TEST(rpc, listMethods)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
    mj_variables_t *result2;
	mj_rpc_result_t result;
	const char* msg;
	
	server = mj_rpc_create();
	mj_rpc_add_method (server, "simple", "ss", &simple_test_hook, (void*)1);

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	mj_variables_put_string(request, "action", "system.listMethods");
	result = mj_rpc_service(server, request, response, NULL);

	ASSERT_EQ(result, PIPELINE_STATUS_OK);
	
	result2 = mj_variables_get_object(response, "result");
	msg = mj_variables_to_string(result2, "", 0).str;
	
	ASSERT_STREQ("[{\"system.events\":\"list all events.\"},"      \
	"{\"system.listMethods\":\"list all methods.\"},{\"simple\":\"ss\"}]", msg);

	mj_variables_destory(request);
	mj_variables_destory(response);
	mj_rpc_free(server);
}


mj_rpc_result_t start_hook1(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value1", "ok1");
	assert(1 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_CONTINUE;
}
mj_rpc_result_t start_hook2(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value2", "ok2");
	assert(2 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_CONTINUE;
}
mj_rpc_result_t hook1_stopat(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value1", "stop1");
	assert(1 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_CONTINUE;
}
mj_rpc_result_t hook2_stopat(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value2", "stop2");
	assert(2 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_CONTINUE;
}

TEST(rpc, pipeline1)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
	mj_rpc_result_t result;
	const char* ok;

	server = mj_rpc_create();
	mj_rpc_add_method (server, "start_hook1", 0, &start_hook1, (void*)1);
	mj_rpc_add_method (server, "start_hook2", 0, &start_hook2, (void*)2);
	mj_rpc_add_method (server, "hook1_stopat", 0, &hook1_stopat, (void*)1);
	mj_rpc_add_method (server, "hook2_stopat", 0, &hook2_stopat, (void*)2);

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	result = mj_rpc_invoke(server,"start:start_", request, response, (void*)2, false);

	ASSERT_EQ(result, PIPELINE_STATUS_CONTINUE);

	ok = mj_variables_get_string(response, "value1", 0, 0).str;
	ASSERT_STREQ("ok1", ok);
	ok = mj_variables_get_string(response, "value2", 0, 0).str;
	ASSERT_STREQ("ok2", ok);

	mj_variables_destory(request);
	mj_variables_destory(response);



	
	request = mj_variables_new_table();
	response = mj_variables_new_table();

	result = mj_rpc_invoke(server,"end:_stopat", request, response, (void*)2, false);

	ASSERT_EQ(result, PIPELINE_STATUS_CONTINUE);

	ok = mj_variables_get_string(response, "value1", 0, 0).str;
	ASSERT_STREQ("stop1", ok);
	ok = mj_variables_get_string(response, "value2", 0, 0).str;
	ASSERT_STREQ("stop2", ok);

	mj_variables_destory(request);
	mj_variables_destory(response);

	mj_rpc_free(server);
}



mj_rpc_result_t p2_hook1(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value1", "ok1");
	assert(1 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_OK;
}
mj_rpc_result_t p2_hook2(mj_variables_t *request
    , mj_variables_t *response, mj_rpc_context_t* usrContext)
{
	mj_variables_put_string(response, "value2", "ok2");
	assert(2 == ((int)usrContext->method->usrContext));
	assert(2 == ((int)usrContext->usrContext));
	return PIPELINE_STATUS_OK;
}

TEST(rpc, pipeline2)
{
	mj_rpc_server_t* server;
    mj_variables_t *request;
    mj_variables_t *response;
	mj_rpc_result_t result;
	const char* v1;
	const char* v2;

	server = mj_rpc_create();
	mj_rpc_add_method (server, "p2_hook1", 0, &p2_hook1, (void*)1);
	mj_rpc_add_method (server, "p2_hook2", 0, &p2_hook2, (void*)2);

	request = mj_variables_new_table();
	response = mj_variables_new_table();

	result = mj_rpc_invoke(server,"start:p2_", request, response, (void*)2, true);

	ASSERT_EQ(result, PIPELINE_STATUS_OK);

	v1 = mj_variables_get_string(response, "value1", 0, 0).str;
	v2 = mj_variables_get_string(response, "value2", 0, 0).str;
	ASSERT_STREQ("ok1", v1);
	ASSERT_STREQ("ok2", v2);

	mj_variables_destory(request);
	mj_variables_destory(response);


	mj_rpc_free(server);
}