#include "meijing/unittest.h"


TEST_DECLARE(ARRAY, func);
TEST_DECLARE(hashtable, simple);
TEST_DECLARE(hashtable, simple2);
TEST_DECLARE(json, simple);
TEST_DECLARE(object, simple);
TEST_DECLARE(object, array_push_pop);
TEST_DECLARE(object, array_set_and_del);
TEST_DECLARE(object, array_set_and_del2);
TEST_DECLARE(object, dict_put_and_get);
TEST_DECLARE(object, dict_element);
TEST_DECLARE(rpc, empty);
TEST_DECLARE(rpc, simple);
TEST_DECLARE(rpc, listMethods);
TEST_DECLARE(rpc, remove);
TEST_DECLARE(rpc, pipeline1);
TEST_DECLARE(rpc, pipeline2);

void out(const char* buf, size_t len)
{
	printf(buf);
}

int main()
{
	TEST_DEFINE(ARRAY, func);
	TEST_DEFINE(hashtable, simple);
	TEST_DEFINE(hashtable, simple2);
	TEST_DEFINE(json, simple);
	TEST_DEFINE(object, simple);
	TEST_DEFINE(object, array_push_pop);
	TEST_DEFINE(object, array_set_and_del);
	TEST_DEFINE(object, array_set_and_del2);
	TEST_DEFINE(object, dict_put_and_get);
	TEST_DEFINE(object, dict_element);
	TEST_DEFINE(rpc, empty);
	TEST_DEFINE(rpc, simple);
	TEST_DEFINE(rpc, listMethods);
	TEST_DEFINE(rpc, remove);
	TEST_DEFINE(rpc, pipeline1);
	TEST_DEFINE(rpc, pipeline2);

	RUN_ALL_TESTS(&out);
	RUN_ALL_TESTS(&out);
	return 0;
}