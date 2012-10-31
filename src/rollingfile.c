#ifdef _WIN32
#include <io.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "meijing/rollingfile.h"
#include "meijing/string_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif


struct mj_rollingfile_s
{
	char* nm;
	size_t max_size;
	size_t remain_count;

	size_t current_size;

	FILE* out;
};

void debugPrintf(const char* src, const char* dst)
{
	printf("roll file %s to %s - %s\n", src, dst, strerror(errno));
//#ifdef _WIN32
//	{
//		char buf[1024];
//		snprintf(buf, 1024, "roll file %s to %s - %s\n", src, dst, strerror(errno));
//		OutputDebugString(buf);
//	}
//#endif
}

void ensure_file_size(mj_rollingfile_t* rf)
{
	char bakstr1[MAX_PATH + 8];
	char bakstr2[MAX_PATH + 8];
	char* bak1 = bakstr1;
	char* bak2 = bakstr2;

	size_t n;
	if(rf->current_size < rf->max_size)	 {
		if(0 == rf->out) {
			rf->out = fopen(rf->nm, "a+");
			
			if(0 == rf->out)
				printf("打开日志文件 '%s' 失败 - %s", rf->nm, strerror(errno));
		}
		return;
	}

	if(0 != rf->out) {
		fclose(rf->out);
		rf->out = 0;
	}

	snprintf(bak2, MAX_PATH, "%s.%u",  rf->nm, rf->remain_count);
	remove(bak2);

	for(n = rf->remain_count-1; n > 0; --n) {
		char* tmp;
		snprintf(bak1, MAX_PATH, "%s.%u",  rf->nm, n);

		if(0 != rename(bak1, bak2))
			debugPrintf(bak1, bak2);

		tmp = bak1;
		bak1 = bak2;
		bak2 = tmp;
	}

	if(0 != rename(rf->nm, bak2)) {
		debugPrintf(rf->nm, bak2);
	} else {
		rf->current_size = 0;
		rf->out = fopen(rf->nm, "a+");
		
		if(0 == rf->out)
			printf("open rolling file '%s' failed - %s", rf->nm, strerror(errno));
	}
}

DLL_VARIABLE mj_rollingfile_t* mj_rollingfile_create(const char* nm, size_t max_size, size_t remain_count)
{
	mj_rollingfile_t* ret;
	if(0 == nm)
		return 0;

	ret = (mj_rollingfile_t*)my_malloc(sizeof(mj_rollingfile_t));
	ret->nm = strdup(nm);
	ret->max_size = max_size;
	ret->remain_count = remain_count;
	ret->current_size = max_size + 10;
	ret->out = fopen(nm, "a+");
	if(0 == ret->out)
	{
		my_free(ret->nm);
		my_free(ret);
		return 0;
	}	

	return ret;	
}

DLL_VARIABLE void mj_rollingfile_free(mj_rollingfile_t* rf)
{
	if(0 == rf)
		return;

	if(0 != rf->out)
		fclose(rf->out);
	if(0 != rf->nm)
		my_free(rf->nm);
	my_free(rf);
}


DLL_VARIABLE void mj_rollingfile_write(mj_rollingfile_t* rf, const char* data, size_t n)
{
	if(0 == rf)
		return;

	ensure_file_size(rf);

	if(0 == rf->out)
		return;

	fwrite(data, sizeof(char), n, rf->out);
	rf->current_size += n;
}

DLL_VARIABLE void mj_rollingfile_flush(mj_rollingfile_t* rf)
{
	if(0 == rf || 0 == rf->out)
		return;

	fflush(rf->out);
}

#ifdef __cplusplus
}
#endif
