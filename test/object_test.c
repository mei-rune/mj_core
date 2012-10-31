#include "meijing/variables.h"
#include "meijing/unittest.h"

TEST(object, simple)
{

 mj_variables_t* nan = mj_variables_new_NaN();
 mj_variables_t* nil = mj_variables_new_Nil();
 mj_variables_t* minusInfinity = mj_variables_new_minusInfinity();
 mj_variables_t* positiveInfinity = mj_variables_new_positiveInfinity();

 mj_variables_t* botrue = mj_variables_new_boolean(true);
 mj_variables_t* bofalse = mj_variables_new_boolean(false);
 mj_variables_t* oi = mj_variables_new_int(-4);
 mj_variables_t* oui = mj_variables_new_uint(4);
 mj_variables_t* od = mj_variables_new_double(0.9);
 mj_variables_t* os1 = mj_variables_new_string("aaa");
 mj_variables_t* os2 = mj_variables_new_stringLen("aaaaaaa", 3);
 mj_variables_t* os3 = mj_variables_new_stringN('a', 3);

 
 ASSERT_STREQ("NaN", mj_variables_to_string(nan,0,0).str);
 ASSERT_EQ(0, mj_variables_to_string(nil,0,0).str);

 ASSERT_STREQ("-Infinity", mj_variables_to_string(minusInfinity,0,0).str);
 ASSERT_STREQ("Infinity", mj_variables_to_string(positiveInfinity,0,0).str);
 
 ASSERT_STREQ("true", mj_variables_to_string(botrue,0,0).str);
 ASSERT_STREQ("false", mj_variables_to_string(bofalse,0,0).str);

 ASSERT_STREQ("-4", mj_variables_to_string(oi,0,0).str);
 ASSERT_STREQ("4", mj_variables_to_string(oui,0,0).str);
 ASSERT_STREQ("0.9", mj_variables_to_string(od,0,0).str);

 ASSERT_STREQ("aaa", mj_variables_to_string(os1,0,0).str);
 ASSERT_STREQ("aaa", mj_variables_to_string(os2,0,0).str);
 ASSERT_STREQ("aaa", mj_variables_to_string(os3,0,0).str);

 mj_variables_destory(nan);
 mj_variables_destory(nil);
 mj_variables_destory(minusInfinity);
 mj_variables_destory(positiveInfinity);
 mj_variables_destory(botrue);
 mj_variables_destory(bofalse);
 mj_variables_destory(oi);
 mj_variables_destory(oui);
 mj_variables_destory(od);
 mj_variables_destory(os1);
 mj_variables_destory(os2);
 mj_variables_destory(os3);
}

TEST(object, array_push_pop)
{
    mj_variables_t* obj;

    mj_variables_t* ar = mj_variables_new_list();
    ASSERT_EQ(0, mj_variables_length(ar));
    mj_variables_element_push_boolean(ar,  true);
    mj_variables_element_push_int(ar,  -23);
    mj_variables_element_push_uint(ar,  23);
    mj_variables_element_push_double(ar,  2.34);
    mj_variables_element_push_string(ar,  "23.34");
    mj_variables_element_push_stringLen(ar,  "23.34", 2);
    mj_variables_element_push_stringLen(ar,  "23.34dfs", -1);
    mj_variables_element_push_stringN(ar,  'a' , 10);

    ASSERT_EQ(8, mj_variables_length(ar));

    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(mj_variables_element_at(ar, 0)));
    ASSERT_TRUE(mj_variables_to_boolean(mj_variables_element_at(ar, 0), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 1)));
    ASSERT_TRUE(-23 == mj_variables_to_int(mj_variables_element_at(ar, 1), -1));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 2)));
    ASSERT_TRUE(23 == mj_variables_to_uint(mj_variables_element_at(ar, 2), -1));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 3)));
    //ASSERT_TRUE(23.34 == mj_variables_to_double(mj_variables_element_at(ar, 3)));

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 4)));
    ASSERT_STREQ("23.34", mj_variables_to_string(mj_variables_element_at(ar, 4), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 5)));
    ASSERT_STREQ("23", mj_variables_to_string(mj_variables_element_at(ar, 5), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 6)));
    ASSERT_STREQ("23.34dfs", mj_variables_to_string(mj_variables_element_at(ar, 6), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 7)));
    ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(mj_variables_element_at(ar, 7), 0, 0).str);

    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 3437));


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(obj));
    ASSERT_TRUE(mj_variables_to_boolean(obj, true));
    mj_variables_destory(obj);


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    ASSERT_TRUE(-23 == mj_variables_to_int(obj, 0));
    mj_variables_destory(obj);


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    ASSERT_TRUE(23 == mj_variables_to_uint(obj, -1));
    mj_variables_destory(obj);

    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    //ASSERT_TRUE(23.34 == mj_variables_to_double(obj));
    mj_variables_destory(obj);


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23.34", mj_variables_to_string(obj, 0, 0).str);
    mj_variables_destory(obj);


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23", mj_variables_to_string(obj, 0, 0).str);
    mj_variables_destory(obj);

    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23.34dfs", mj_variables_to_string(obj, 0, 0).str);
    mj_variables_destory(obj);


    obj = mj_variables_element_pop(ar);
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(obj, 0, 0).str);
    mj_variables_destory(obj);

    ASSERT_EQ(0, mj_variables_length(ar));

    mj_variables_destory(ar);
}

TEST(object, array_set_and_del)
{
    mj_variables_t* ar = mj_variables_new_list();
    ASSERT_EQ(0, mj_variables_length(ar));
    mj_variables_element_set_boolean(ar,   0, true);
    mj_variables_element_set_int(ar,       1, -23);
    mj_variables_element_set_uint(ar,      2, 23);
    mj_variables_element_set_double(ar,    3, 23.34);
    mj_variables_element_set_string(ar,    4, "23.34");
    mj_variables_element_set_stringLen(ar, 5, "23.34", 2);
    mj_variables_element_set_stringLen(ar, 6, "23.34dfs", -1);
    mj_variables_element_set_stringN(ar,   7, 'a' , 10);

    ASSERT_EQ(8, mj_variables_length(ar));

    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(mj_variables_element_at(ar, 0)));
    ASSERT_TRUE(mj_variables_to_boolean(mj_variables_element_at(ar, 0), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 1)));
    ASSERT_TRUE(-23 == mj_variables_to_int(mj_variables_element_at(ar, 1), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 2)));
    ASSERT_TRUE(23 == mj_variables_to_uint(mj_variables_element_at(ar, 2), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 3)));
    //ASSERT_TRUE(23.34 == mj_variables_to_double(mj_variables_element_at(ar, 3)));

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 4)));
    ASSERT_STREQ("23.34", mj_variables_to_string(mj_variables_element_at(ar, 4), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 5)));
    ASSERT_STREQ("23", mj_variables_to_string(mj_variables_element_at(ar, 5), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 6)));
    ASSERT_STREQ("23.34dfs", mj_variables_to_string(mj_variables_element_at(ar, 6), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 7)));
    ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(mj_variables_element_at(ar, 7), 0, 0).str);

    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 3437));


    mj_variables_element_del(ar, 1);
    ASSERT_EQ(7, mj_variables_length(ar));

    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(mj_variables_element_at(ar, 0)));
    ASSERT_TRUE(mj_variables_to_boolean(mj_variables_element_at(ar, 0), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 1)));
    ASSERT_TRUE(23 == mj_variables_to_uint(mj_variables_element_at(ar, 1), 0));

    mj_variables_element_del(ar, 6);
    mj_variables_element_del(ar, 5);
    mj_variables_element_del(ar, 4);
    mj_variables_element_del(ar, 3);
    mj_variables_element_del(ar, 2);
    mj_variables_element_del(ar, 1);
    mj_variables_element_del(ar, 0);

    ASSERT_EQ(0, mj_variables_length(ar));

    mj_variables_destory(ar);
}


TEST(object, array_set_and_del2)
{
    mj_variables_t* ar = mj_variables_new_list();
    ASSERT_EQ(0, mj_variables_length(ar));
    mj_variables_element_set_stringN(ar,   7, 'a' , 10);


    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 6));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 5));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 4));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 3));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 2));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 1));
    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 0));

    mj_variables_element_set_stringLen(ar, 6, "23.34dfs", -1);
    mj_variables_element_set_stringLen(ar, 5, "23.34", 2);
    mj_variables_element_set_string(ar,    4, "23.34");
    mj_variables_element_set_double(ar,    3, 2.34);
    mj_variables_element_set_uint(ar,      2, 23);
    mj_variables_element_set_int(ar,       1, -23);
    mj_variables_element_set_boolean(ar,   0, true);

    ASSERT_EQ(8, mj_variables_length(ar));

    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(mj_variables_element_at(ar, 0)));
    ASSERT_TRUE(mj_variables_to_boolean(mj_variables_element_at(ar, 0), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 1)));
    ASSERT_TRUE(-23 == mj_variables_to_int(mj_variables_element_at(ar, 1), 0));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 2)));
    ASSERT_TRUE(23 == mj_variables_to_uint(mj_variables_element_at(ar, 2), 0));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 3)));
    //ASSERT_TRUE(23.34 == mj_variables_to_double(mj_variables_element_at(ar, 3)));

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 4)));
    ASSERT_STREQ("23.34", mj_variables_to_string(mj_variables_element_at(ar, 4), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 5)));
    ASSERT_STREQ("23", mj_variables_to_string(mj_variables_element_at(ar, 5), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 6)));
    ASSERT_STREQ("23.34dfs", mj_variables_to_string(mj_variables_element_at(ar, 6), 0, 0).str);

    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(mj_variables_element_at(ar, 7)));
    ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(mj_variables_element_at(ar, 7), 0, 0).str);

    ASSERT_TRUE(NULL == mj_variables_element_at(ar, 3437));


    mj_variables_element_del(ar, 1);
    ASSERT_EQ(7, mj_variables_length(ar));

    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(mj_variables_element_at(ar, 0)));
    ASSERT_TRUE(mj_variables_to_boolean(mj_variables_element_at(ar, 0), true));

    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(mj_variables_element_at(ar, 1)));
    ASSERT_TRUE(23 == mj_variables_to_uint(mj_variables_element_at(ar, 1), 0));

    mj_variables_element_del(ar, 6);
    mj_variables_element_del(ar, 5);
    mj_variables_element_del(ar, 4);
    mj_variables_element_del(ar, 3);
    mj_variables_element_del(ar, 2);
    mj_variables_element_del(ar, 1);
    mj_variables_element_del(ar, 0);

    ASSERT_EQ(0, mj_variables_length(ar));

    mj_variables_destory(ar);
}



TEST(object, dict_put_and_get)
{
    mj_variables_t* obj;

    mj_variables_t* ar = mj_variables_new_table();
    ASSERT_EQ(0, mj_variables_length(ar));
    mj_variables_put_boolean(ar,   "a0", true);
    mj_variables_put_int(ar,       "a1", -23);
    mj_variables_put_uint(ar,      "a2", 23);
    mj_variables_put_double(ar,    "a3", 2.34);
    mj_variables_put_string(ar,    "a4", "23.34");
    mj_variables_put_stringLen(ar, "a5", "23.34", 2);
    mj_variables_put_stringLen(ar, "a6", "23.34dfs", -1);
    mj_variables_put_stringN(ar,   "a7", 'a' , 10);

    ASSERT_EQ(8, mj_variables_length(ar));

    obj = mj_variables_get_object(ar, "a0");
    ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(obj));
    ASSERT_TRUE(mj_variables_get_boolean(ar, "a0", true));


    obj = mj_variables_get_object(ar, "a1");
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    ASSERT_TRUE(-23 == mj_variables_get_int(ar, "a1", 0));

    obj = mj_variables_get_object(ar, "a2");
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    ASSERT_TRUE(23 == mj_variables_get_uint(ar, "a2", 0));


    obj = mj_variables_get_object(ar, "a3");
    ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
    //ASSERT_TRUE(23.34 == mj_variables_get_double(ar, "a3")));


    obj = mj_variables_get_object(ar, "a4");
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23.34", mj_variables_get_string(ar, "a4", 0, 0).str);


    obj = mj_variables_get_object(ar, "a5");
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23", mj_variables_get_string(ar, "a5", 0, 0).str);



    obj = mj_variables_get_object(ar, "a6");
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("23.34dfs", mj_variables_get_string(ar, "a6", 0, 0).str);


    obj = mj_variables_get_object(ar, "a7");
    ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
    ASSERT_STREQ("aaaaaaaaaa", mj_variables_get_string(ar, "a7", 0, 0).str);


    mj_variables_del_object(ar, "a3");

    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a2");
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a7");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a6");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a5");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a4");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a1");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL != mj_variables_get_object(ar, "a0"));

    mj_variables_del_object(ar, "a0");
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a7"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a6"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a5"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a4"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a3"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a2"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a1"));
    ASSERT_TRUE(NULL == mj_variables_get_object(ar, "a0"));

    ASSERT_EQ(0, mj_variables_length(ar));

    mj_variables_destory(ar);
}



TEST(object, dict_element)
{
    mj_variables_t* obj;
	size_t i;
    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    int a4 = 0;
    int a5 = 0;
    int a6 = 0;
    int a7 = 0;

    mj_variables_t* ar = mj_variables_new_table();
    ASSERT_EQ(0, mj_variables_length(ar));
    mj_variables_put_boolean(ar,   "a0", true);
    mj_variables_put_int(ar,       "a1", -23);
    mj_variables_put_uint(ar,      "a2", 23);
    mj_variables_put_double(ar,    "a3", 2.34);
    mj_variables_put_string(ar,    "a4", "23.34");
    mj_variables_put_stringLen(ar, "a5", "23.34", 2);
    mj_variables_put_stringLen(ar, "a6", "23.34dfs", -1);
    mj_variables_put_stringN(ar,   "a7", 'a' , 10);

    ASSERT_EQ(8, mj_variables_length(ar));


    for( i = 0; i < 8; ++ i)
    {
		const char* key;

        obj = mj_variables_element_at(ar, i);
		key = mj_variables_element_at_string(obj, 0, 0, 0).str;
        if(0 == strcmp("a0",  key))
        {
            a0 = 1;
            obj = mj_variables_element_at(obj, 1);

            ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(obj));
            ASSERT_TRUE(mj_variables_to_boolean(obj, true));
        }
        else if(0 == strcmp("a1",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a1 = 1;
            obj = mj_variables_element_at(obj, 1);

            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            ASSERT_TRUE(-23 == mj_variables_to_int(obj, 0));

        }
        else if(0 == strcmp("a2",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a2 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            ASSERT_TRUE(23 == mj_variables_to_uint(obj, 0));

        }
        else if(0 == strcmp("a3",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a3 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            //ASSERT_TRUE(23.34 == mj_variables_to_double(obj)));

        }
        else if(0 == strcmp("a4",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a4 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23.34", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a5",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a5 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a6",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a6 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23.34dfs", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a7",  mj_variables_element_at_string(obj, 0, 0, 0).str))
        {
            a7 = 1;
            obj = mj_variables_element_at(obj, 1);
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(obj, 0, 0).str);
        }
        else
        {
            CHECK_MESSAGE(false,"AAAA");
        }
    }



    ASSERT_EQ(a0 , 1);
    ASSERT_EQ(a1 , 1);
    ASSERT_EQ(a2 , 1);
    ASSERT_EQ(a3 , 1);
    ASSERT_EQ(a4 , 1);
    ASSERT_EQ(a5 , 1);
    ASSERT_EQ(a6 , 1);
    ASSERT_EQ(a7 , 1);

    a0 = 0;
    a1 = 0;
    a2 = 0;
    a3 = 0;
    a4 = 0;
    a5 = 0;
    a6 = 0;
    a7 = 0;


    for( i = 0; i < 8; ++ i)
    {
        obj = mj_variables_element_at(ar, i);
        if(0 == strcmp("a0", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a0 = 1;
            obj = mj_variables_get_object(obj, "value");

            ASSERT_EQ(mj_variable_type_boolean, mj_variables_get_type(obj));
            ASSERT_TRUE(mj_variables_to_boolean(obj, true));
        }
        else if(0 == strcmp("a1", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a1 = 1;
            obj = mj_variables_get_object(obj, "value");

            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            ASSERT_TRUE(-23 == mj_variables_to_int(obj, 0));

        }
        else if(0 == strcmp("a2", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a2 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            ASSERT_TRUE(23 == mj_variables_to_uint(obj,0));

        }
        else if(0 == strcmp("a3", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a3 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_number, mj_variables_get_type(obj));
            //ASSERT_TRUE(23.34 == mj_variables_to_double(obj)));

        }
        else if(0 == strcmp("a4", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a4 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23.34", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a5", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a5 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a6", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a6 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("23.34dfs", mj_variables_to_string(obj, 0, 0).str);
        }
        else if(0 == strcmp("a7", mj_variables_get_string(obj, "key", 0, 0).str))
        {
            a7 = 1;
            obj = mj_variables_get_object(obj, "value");
            ASSERT_EQ(mj_variable_type_string, mj_variables_get_type(obj));
            ASSERT_STREQ("aaaaaaaaaa", mj_variables_to_string(obj, 0, 0).str);
        }
        else
        {
            CHECK_MESSAGE(true,"AAAA");
        }
    }

    
    ASSERT_EQ(a0 , 1);
    ASSERT_EQ(a1 , 1);
    ASSERT_EQ(a2 , 1);
    ASSERT_EQ(a3 , 1);
    ASSERT_EQ(a4 , 1);
    ASSERT_EQ(a5 , 1);
    ASSERT_EQ(a6 , 1);
    ASSERT_EQ(a7 , 1);

    mj_variables_destory(ar);
}