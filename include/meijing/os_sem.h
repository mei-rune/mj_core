
#ifndef _mj_os_sem_h_
#define _mj_os_sem_h_ 1


#include "meijing/platform.h"

#ifdef _WIN32
#include <time.h>
#include "meijing/os_threads.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __MINGW32__

struct timespec
{
	time_t tv_sec;
	long tv_nsec;
};

#endif

typedef struct _sem_st
{
	HANDLE sem ;
	int value ;
} sem_t;


DLL_VARIABLE int sem_init(sem_t *sem , int shared , int value) ;

DLL_VARIABLE int sem_destroy(sem_t *sem) ;

DLL_VARIABLE int sem_wait(sem_t *sem) ;

DLL_VARIABLE int sem_trywait(sem_t *sem);

DLL_VARIABLE int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

DLL_VARIABLE int sem_post(sem_t *sem) ;


#ifdef __cplusplus
}
#endif

#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <semaphore.h>
	#include <time.h>
#endif


#endif
