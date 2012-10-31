
#ifndef _mj_os_atomic_h
#define _mj_os_atomic_h 1

#include "meijing/platform.h"


#ifdef __cplusplus
extern "C" {
#endif

#define atomic_t volatile int
#define ATOMIC_INIT(i)  (i)

#ifdef _WIN32
#define atomic_add(val, mem)               (InterlockedExchangeAdd(mem, val) + val)
#define atomic_sub(val, mem)               (InterlockedExchangeAdd(mem , - val) - val)
#define atomic_dec(mem)                    (InterlockedDecrement(mem))
#define atomic_inc(mem)                    (InterlockedIncrement(mem))
#define atomic_read(mem)                   (*mem)
#define atomic_set(mem, val)               (InterlockedExchange(mem , val))
#define atomic_cvs(mem, new_val, old_val)  (InterlockedCompareExchange(mem,1, 0))
#else
#define atomic_add(val, mem)              __sync_add_and_fetch (mem, val)
#define atomic_sub(val, mem)              __sync_sub_and_fetch (mem, val)
#define atomic_dec(mem)                    __sync_sub_and_fetch(mem, 1)
#define atomic_inc(mem)                    __sync_add_and_fetch(mem, 1)
#define atomic_read(mem)                   (*mem)
#define atomic_set(mem, val)              __sync_lock_test_and_set(mem, val)
#define atomic_cvs(mem, new_val, old_val) __sync_val_compare_and_swap(mem,0, 1)
#endif



#ifdef __cplusplus
}
#endif

#endif    /** _OS_ATOMIC_H */
