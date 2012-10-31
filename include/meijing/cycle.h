
#ifndef _mj_cycle_h_
#define _mj_cycle_h_ 1

#include "meijing/platform.h"
#include <sys/types.h>
#include "meijing/variables.h"

typedef struct mj_cycle_s {
	mj_variables_t* variables;

} mj_cycle_;

void mj_cycle_init(mj_cycle_* cy);
void mj_cycle_destory(mj_cycle_* cy);

typedef struct mj_module_s {
    int               (*init_module)(mj_cycle_ *cycle);
    int               (*init_process)(mj_cycle_ *cycle);
    int               (*init_thread)(mj_cycle_ *cycle);
    void              (*exit_thread)(mj_cycle_ *cycle);
    void              (*exit_process)(mj_cycle_ *cycle);
    void              (*exit_module)(mj_cycle_ *cycle);
} mj_module_t;



#define MEIJING_HOOK_MAGIC    { '0','1','0','0' }
typedef mj_module_t* (*mj_hook_load_func_t) (int version
	, mj_cycle_* server
	, char* err
	, size_t len);

#define MEIJING_DECLARE_HOOK(name) mj_module_t* _##name##_internal(mj_cycle_* server                    \
																				, char* err, size_t len);      \
                                                                                                               \
HOOK_VARIABLE mj_module_t* name(int version, mj_cycle_* server, char* err, size_t len) {                \                                                                                                       \
    static char magic[] = MEIJING_HOOK_MAGIC;                                                                  \
    if(version != *(int*)magic) {                                                                              \
        snprintf(err, len, "version is error, excepted version is '%c.%c.%c.%c', "                             \
                                               "real version is '%c.%c.%c.%c'."                                \
                             , magic[0], magic[1], magic[2], magic[3]                                          \
                             , ((char*)&version)[0], ((char*)&version)[1]                                      \
                             , ((char*)&version)[2], ((char*)&version)[3]);                                    \
        return 0;                                                                                              \
    }                                                                                                          \
    return _##name##_internal(server, err, len);                                                               \
}                                                                                                              \
                                                                                                               \
mj_module_t* _##name##_internal(mj_cycle_* server, char* err, size_t len)

#endif /* _NGX_CYCLE_H_INCLUDED_ */
