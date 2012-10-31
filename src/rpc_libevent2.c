//
//
//#include "meijing/rpc.h"
//#include "evutil.h"
//#include "event.h"
//#include "evhttp.h"
//#include "evrpc.h"
//
//
//#ifdef __cplusplus
//extern "C" 
//{
//#endif
//
//struct mj_rpc_bridge_s {
//	mj_rpc_server_t* server;
//	struct evhttp_base* base;
//};
//
//
//typedef struct mj_invaction_s {
//	
//	mj_rpc_bridge_t *rpc;
//
//	struct evhttp_request* http_req;
//
//	mj_variables_t *request;
//
//	mj_variables_t *reply;
//
//} mj_invaction_t;
//
//static const char* to_http_command(evhttp_cmd_type type) {
//	switch(type) {
//	case EVHTTP_REQ_GET:
//		return "GET";
//	case EVHTTP_REQ_POST:
//		return "POST";
//	case EVHTTP_REQ_HEAD:
//		return "HEAD";
//	case EVHTTP_REQ_PUT:
//		return "PUT";
//	case EVHTTP_REQ_DELETE:
//		return "DELETE";
//	case EVHTTP_REQ_OPTIONS:
//		return "OPTIONS";
//	case EVHTTP_REQ_TRACE:
//		return "TRACE";
//	case EVHTTP_REQ_CONNECT:
//		return "CONNECT";
//	case EVHTTP_REQ_PATCH:
//		return "PATCH";
//	default:
//		return NULL;
//	}
//};
//
//static void mj_invaction_free(mj_invaction_t* invaction) {
//	mj_variables_destory(invaction->request);
//	mj_variables_destory(invaction->reply);
//	my_free(invaction);
//}
//
//static void mj_bridge_callback_closure(mj_invaction_t *arg, mj_rpc_result_t hook_res) {
//	mj_invaction_free(arg);
//}
//
//static void mj_bridge_callback(struct evhttp_request *req, void *arg)
//{
//	mj_rpc_bridge_t *rpc;
//	mj_invaction_t *rpc_state;
//	mj_string_t methodName;
//	const char* http_command;
//	size_t http_command_len;
//	const char* http_path;
//	size_t http_path_len;
//	mj_rpc_result_t hook_res;
//
//	rpc = (mj_rpc_bridge_t*)arg;
//	rpc_state = (mj_invaction_t*)my_malloc(sizeof(mj_invaction_t));
//	if (rpc_state == NULL)
//		goto error;
//
//	rpc_state->rpc = rpc;
//	rpc_state->http_req = req;
//	
//	rpc_state->request = mj_variables_new_table();
//	rpc_state->reply = mj_variables_new_table();
//	
//	http_command = to_http_command(req->type);
//	http_path = evhttp_uri_get_path(req->uri_elems);
//
//	http_command_len = strlen(http_command);
//	http_path_len = strlen(http_path);
//
//	methodName.len = http_command_len + 1 + http_path_len + 4;
//	methodName.str = (char*)my_malloc(methodName.len * sizeof(char));
//	memcpy(methodName.str, http_command, http_command_len);
//	*(methodName.str + http_command_len) = '/';
//	memcpy(methodName.str + http_command_len + 1, http_path, http_path_len);
//	*(methodName.str + http_command_len + 1 + http_path_len) = 0;
//	
//	hook_res = mj_rpc_call(rpc->server, methodName.str, methodName.len
//		, rpc_state->request, rpc_state->reply, NULL);
//	switch(hook_res) {
//		case PIPELINE_STATUS_CONTINUE:
//		break;
//		case PIPELINE_STATUS_OK:
//		break;
//	}
//
//	mj_bridge_callback_closure(rpc_state, PIPELINE_STATUS_OK);
//	return;
//
//error:
//	if (rpc_state != NULL)
//		mj_invaction_free(rpc_state);
//
//	evhttp_send_error(req, HTTP_SERVUNAVAIL, NULL);
//	return;
//}
//
//static void mj_rpc_bridge_callback(struct evrpc_req_generic *req, void * cbarg) {
//	mj_rpc_bridge_t* bridge;
//	const char* http_command;
//	const char* path;
//	mj_string_t methodName;
//	struct evhttp_request* http_req;
//	mj_variables_t* rpcReq;
//	mj_variables_t* rpcResp;
//
//	http_req = EVRPC_REQUEST_HTTP(req);
//	rpcReq = (mj_variables_t*)evrpc_get_request(req);
//	rpcResp = (mj_variables_t*)evrpc_get_reply(req);
//
//	http_command = to_http_command(http_req->type);
//	path = evhttp_uri_get_path(http_req->uri_elems);
//
//	methodName.len = strlen(http_command) + 1 + strlen(path) + 4;
//	methodName.str = (char*)my_malloc(methodName.len * sizeof(char));
//
//
//	bridge = (mj_rpc_bridge_t*)cbarg;
//	switch(mj_rpc_call(bridge->server, &methodName, rpcReq, rpcResp, NULL)) {
//	case PIPELINE_STATUS_CONTINUE:
//		break;
//	case PIPELINE_STATUS_OK:
//		break;
//	}
//
//}
//
//DLL_VARIABLE mj_rpc_bridge_t* mj_rpc_bridge_create(mj_rpc_server_t* server, void* base) {
//	struct evrpc_base* rpc_base;
//	mj_rpc_bridge_t* bridge;
//
//	rpc_base = (struct evrpc_base*)base;
//	bridge = (mj_rpc_bridge_t*)my_malloc(sizeof(mj_rpc_bridge_t));
//	bridge->server = server;
//	bridge->base = rpc_base;
//
//	evrpc_register_generic(rpc_base, "/",
//    void (*callback)(struct evrpc_req_generic *, void *), void *cbarg,
//    void *(*req_new)(void *), void *req_new_arg, void (*req_free)(void *),
//    int (*req_unmarshal)(void *, struct evbuffer *),
//    void *(*rpl_new)(void *), void *rpl_new_arg, void (*rpl_free)(void *),
//    int (*rpl_complete)(void *),
//    void (*rpl_marshal)(struct evbuffer *, void *))
//
//	EVRPC_REGISTER(rpc_base, /, request, reply, callback, cbarg)	\
//
//
//	return bridge;
//}
//
//DLL_VARIABLE void mj_rpc_bridge_create(mj_rpc_bridge_t* bridge) {
//	my_free(bridge);
//}
//
//#ifdef __cplusplus
//};
//#endif
//
