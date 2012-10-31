#include "meijing/hashtable.h"
#include "meijing/unittest.h"

#define hash_int(x) x
#define cmp_int(x,y) (x - y)


HASH_DECLARE(EMPTY, int2int, int, int);
HASH_DEFINE(EMPTY, int2int, int, int, hash_int, cmp_int, EMPTY_FUNC1, EMPTY_FUNC1);

typedef struct int_thunk_s {
	int x;
	int y;
	struct int_thunk_s* aa;
} int_thunk_t;

#define int_table_get_key(t) t->x
#define int_table_get_next(t) t->aa
#define int_table_set_next(t,v) t->aa = v
#define int_table_free(x) my_free(x)



HASH_DECLARE2(EMPTY, int_table, int, int_thunk_t);

HASH_DEFINE2(EMPTY, int_table
	, int
	, int_thunk_t
	, int_table_get_key
	, int_table_get_next
	, int_table_set_next
	, hash_int
	, cmp_int
	, int_table_free);


TEST(hashtable, simple) {
	int i;
	int2int_t h;
	int2int_init(&h, 3);
	int2int_put_value(&h, 1,1);

	for(i = 10; i <20; i++) {
		int2int_to_array(&h);
	    int2int_put_value(&h, i, i);
	}

	
    int2int_to_array(&h);
	
	ASSERT_EQ(1, int2int_get_value(&h, 1, 0));
	int2int_del(&h, 1);
	ASSERT_EQ(0, int2int_get_value(&h, 1, 0));
	
	for(i = 10; i <20; i++) {
	    ASSERT_EQ(i, int2int_get_value(&h, i, 0));
	}
	
	int2int_to_array(&h);


	
	for(i = 10; i <20; i++) {
		ASSERT_NOT_NULL(int2int_at(&h, i - 10));
	    ASSERT_EQ(i, int2int_get_value(&h, i, 0));
	}
	


	int2int_destroy(&h);
}


TEST(hashtable, simple2) {
	 int_table_t h;
	 int i;
	 int_thunk_t* e1 = (int_thunk_t*)my_malloc(sizeof(int_thunk_t));
	 int_thunk_t* e2 = (int_thunk_t*)my_malloc(sizeof(int_thunk_t));
	 e1->x = 1;
	 e1->y = 1;

	 e2->x = 2;
	 e2->y = 2;

	 int_table_init(&h, 3);
	 int_table_put(&h, e1);
	 int_table_put(&h, e2);

	 for(i = 10; i <20; i++) {
	     int_thunk_t* e = (int_thunk_t*)my_malloc(sizeof(int_thunk_t));
	     e->x = i;
	     e->y = i;
	     int_table_put(&h, e);
	 }

	 ASSERT_EQ(1, int_table_get(&h, 1)->y);
	 int_table_del(&h, 1);
	 ASSERT_EQ(NULL, int_table_get(&h, 1));

	 
	 for(i = 10; i <20; i++) {
	     int_thunk_t* e = int_table_get(&h, i);
		 ASSERT_EQ(i, e->y);
	 }

	 int_table_destroy(&h);
}