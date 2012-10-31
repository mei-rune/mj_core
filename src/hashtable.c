#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "meijing/hashtable.h"

#ifdef __cplusplus
extern "C" {
#endif

	
DLL_VARIABLE int mj_cmp_string(const char* key1, const char* key2)
{
	
	size_t len1;
	size_t len2;

	len1 = strlen(key1);
	len2 = strlen(key2);

	if(len1 == len2)
		return memcmp(key1, key2, len1);
	else if(len1 > len2)
		return 1;
	else 
		return -1;
}

DLL_VARIABLE size_t mj_hash_string(const char *key)
{
	size_t hash = 5381;
	size_t i;
	size_t len;

	len = strlen(key);

	for (i = 0; i < len; i++)
		hash = ((hash << 5) + hash) + key[i]; /* hash * 33 + char */

	return hash;
}

DLL_VARIABLE int mj_mem_cmp(const void* key1, size_t len1, const void* key2, size_t len2)
{
	if(len1 == len2)
		return memcmp(key1, key2, len1);
	else if(len1 > len2)
		return 1;
	else 
		return -1;
}

DLL_VARIABLE size_t mj_mem_hash(const void *vkey, size_t len)
{
	size_t hash = 5381;
	size_t i;
	const char *key = (const char *) vkey;

	for (i = 0; i < len; i++)
		hash = ((hash << 5) + hash) + key[i]; /* hash * 33 + char */

	return hash;
}

#ifdef __cplusplus
}
#endif

