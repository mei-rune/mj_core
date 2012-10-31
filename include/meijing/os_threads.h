
#ifndef _mj_os_threads_h_
#define _mj_os_threads_h_ 1

#ifdef _WIN32
#include "meijing/win32/pthread_windows.h"
#else
#include <pthread.h>
#endif 

#ifdef __cplusplus
extern "C" 
{
#endif

DLL_VARIABLE void _system_lock();
DLL_VARIABLE void _system_unlock();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class system_lock
{
public:
	system_lock()
	{
		_system_lock();
	}

	~system_lock()
	{
		_system_unlock();
	}
private:
	system_lock(const system_lock&);
	system_lock& operator=(const system_lock&);
};

#endif


#endif 

