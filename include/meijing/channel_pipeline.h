
#ifndef _mj_rpc_libevent2_h_
#define _mj_rpc_libevent2_h_ 1

#include <stdarg.h>
#include "meijing/platform.h"
#include "meijing/array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mj_chain_              mj_chain_t;
typedef struct mj_chain_context_s     mj_pipeline_context_t;
typedef struct mj_pipeline_callback_s mj_pipeline_callback_t;


typedef void (*mj_chain_callback_t)(mj_chain_t* , int idx, mj_pipeline_context_t*, void *);

struct mj_chain_context_s {
	void *ctx;
	void (*cb)(void *);
};

struct mj_chain_command_s {
	void *ctx;
	void (*cb)(mj_chain_t* , int idx, mj_pipeline_context_t*, void *);
};

struct mj_chain_s {
	ARRAY_DECLARE(mj_chain_callback_t);
};

void mj_pipeline_send(mj_pipeline_t* chain, mj_pipeline_context_t* ctx);
void mj_pipeline_addLast(mj_pipeline_t* , mj_chain_callback_t cb, void* ctx);
void mj_pipeline_removeLast(mj_pipeline_t* , mj_chain_callback_t cb, void* ctx);

#ifdef __cplusplus
};
#endif

#endif
	
