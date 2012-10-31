#include <string.h>
#include <stdlib.h>
#include "meijing/hooks.h"
#include "meijing/os_dlfcn.h"

#ifndef _MAX_PATH
#define _MAX_PATH   260 /* max. length of full pathname */
#endif

DLL_VARIABLE mj_module_t* mj_load_module(mj_cycle_* server
	, size_t argc
	, const char*const* args
	, char* err
	, size_t len) {
	static char magic[] = MEIJING_HOOK_MAGIC;
	void * symbol;
	void* module;
	const char* moduleName;
	const char* symbolName;
	mj_module_t* result;

	if(2 != argc)
	{
		snprintf(err, len, "parameter count is mismatch!");
		return 0;
	}
	
	moduleName = args[1];
	symbolName = args[0];

	module = dlopen(moduleName, RTLD_NOW);
	if(0 == module)
	{
		char modulePath[_MAX_PATH];
		const char* ptr = strchr(moduleName, '.');
		if(0 != ptr)
		{
			snprintf(err, len, "load module '%s' error[ %s ]", moduleName, dlerror());
			return 0;
		}

		strcpy(modulePath, moduleName);
		strcat(modulePath,".dll");

		module = dlopen(modulePath, RTLD_NOW);
		if(0 == module)
		{
			strcpy(modulePath, moduleName);
			strcat(modulePath,".so");
			module = dlopen(modulePath, RTLD_NOW);
			if(0 == module)
			{
				snprintf(err, len, "load module '%s' error[ %s ]", moduleName, dlerror());
				return 0;
			}
		}
	}

	symbol = dlsym(module, symbolName);
	if(0 == symbol)
	{
		snprintf(err, len, "load symbol %s at module '%s' [ %s ]", symbolName, moduleName, dlerror());
		dlclose(module);
		return 0;
	}

	result = (*(mj_hook_load_func_t)symbol)(*(int*)magic, server, err, len);

	if(0 == result)
	{
		dlclose(module);
		return 0;
	}
	return result;
}
