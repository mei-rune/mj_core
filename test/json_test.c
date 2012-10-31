#include "meijing/json.h"
#include "meijing/unittest.h"

const char simpleJson[] = "[ 12, -23, \"123\", true, false"
   ", [12, -23, \"123\", true, false, {\"a\": \"b\"}],"
   " {"
       "\"a1\":1, \"a2\":-2, \"a3\":true, \"a4\":false, \"a5\":\"str\","
	   " \"a6\":[12, -23, \"123\", true, false, {\"a\": \"b\"}], \"a11\":12 "
   "},"
   " {} ]"; 

TEST(json, simple)
{
	char error_buf[1024];
	mj_variables_t* ar;
	mj_variables_t* ar2;
	mj_variables_t* map;
	mj_variables_t* map2;
	mj_variables_t* ret = mj_json_parse_from_string(simpleJson, sizeof(simpleJson)-1, error_buf, 1024);
	ASSERT_EQ( true, mj_variables_typeof(ret, mj_variable_type_list));
	ASSERT_EQ( 8, mj_variables_length(ret));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ret, 0), mj_variable_type_number));
	ASSERT_EQ( 12, mj_variables_to_int(mj_variables_element_at(ret, 0), 0));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ret, 1), mj_variable_type_number));
	ASSERT_EQ( -23, mj_variables_to_int(mj_variables_element_at(ret, 1), 0));

	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ret, 2), mj_variable_type_string));
	ASSERT_STREQ( "123", mj_variables_to_string(mj_variables_element_at(ret, 2), 0, 0).str);
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ret, 3), mj_variable_type_boolean));
	ASSERT_EQ( true, mj_variables_to_boolean(mj_variables_element_at(ret, 3), false));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ret, 4), mj_variable_type_boolean));
	ASSERT_EQ( false, mj_variables_to_boolean(mj_variables_element_at(ret, 4), true));


	ar = mj_variables_element_at(ret, 5);
	ASSERT_EQ( true, mj_variables_typeof(ar, mj_variable_type_list));
	ASSERT_EQ( 6, mj_variables_length(ar));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar, 0), mj_variable_type_number));
	ASSERT_EQ( 12, mj_variables_to_int(mj_variables_element_at(ar, 0), 0));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar, 1), mj_variable_type_number));
	ASSERT_EQ( -23, mj_variables_to_int(mj_variables_element_at(ar, 1), 0));

	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar, 2), mj_variable_type_string));
	ASSERT_STREQ( "123", mj_variables_to_string(mj_variables_element_at(ar, 2), 0, 0).str);
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar, 3), mj_variable_type_boolean));
	ASSERT_EQ( true, mj_variables_to_boolean(mj_variables_element_at(ar, 3), false));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar, 4), mj_variable_type_boolean));
	ASSERT_EQ( false, mj_variables_to_boolean(mj_variables_element_at(ar, 4), true));


	
	map = mj_variables_element_at(ar, 5);
	ASSERT_EQ( true, mj_variables_typeof(map, mj_variable_type_table));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map, "a"), mj_variable_type_string));
	ASSERT_STREQ( "b", mj_variables_to_string(mj_variables_get_object(map, "a"),0,0).str);

	map2 = mj_variables_element_at(ret, 6);
	ASSERT_EQ( true, mj_variables_typeof(map2, mj_variable_type_table));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a1"), mj_variable_type_number));
	ASSERT_EQ( 1, mj_variables_to_int(mj_variables_get_object(map2, "a1"), 0));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a2"), mj_variable_type_number));
	ASSERT_EQ( -2, mj_variables_to_int(mj_variables_get_object(map2, "a2"), 0));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a3"), mj_variable_type_boolean));
	ASSERT_EQ( true, mj_variables_to_boolean(mj_variables_get_object(map2, "a3"), false));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a4"), mj_variable_type_boolean));
	ASSERT_EQ( false, mj_variables_to_boolean(mj_variables_get_object(map2, "a4"), true));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a5"), mj_variable_type_string));
	ASSERT_STREQ( "str",mj_variables_to_string(mj_variables_get_object(map2, "a5"),0,0).str);
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a11"), mj_variable_type_number));
	ASSERT_EQ( 12, mj_variables_to_int(mj_variables_get_object(map2, "a11"), 0));

	
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map2, "a6"), mj_variable_type_list));
	ar2 = mj_variables_get_object(map2, "a6");

	ASSERT_EQ( true, mj_variables_typeof(ar2, mj_variable_type_list));
	ASSERT_EQ( 6, mj_variables_length(ar2));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar2, 0), mj_variable_type_number));
	ASSERT_EQ( 12, mj_variables_to_int(mj_variables_element_at(ar2, 0), 0));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar2, 1), mj_variable_type_number));
	ASSERT_EQ( -23, mj_variables_to_int(mj_variables_element_at(ar2, 1), 0));

	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar2, 2), mj_variable_type_string));
	ASSERT_STREQ( "123", mj_variables_to_string(mj_variables_element_at(ar2, 2),0,0).str);
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar2, 3), mj_variable_type_boolean));
	ASSERT_EQ( true, mj_variables_to_boolean(mj_variables_element_at(ar2, 3), false));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_element_at(ar2, 4), mj_variable_type_boolean));
	ASSERT_EQ( false, mj_variables_to_boolean(mj_variables_element_at(ar2, 4), true));

	
	map2 = mj_variables_element_at(ar2, 5);
	ASSERT_EQ( true, mj_variables_typeof(map2, mj_variable_type_table));
	ASSERT_EQ( true, mj_variables_typeof(mj_variables_get_object(map, "a"), mj_variable_type_string));
	ASSERT_STREQ( "b", mj_variables_to_string(mj_variables_get_object(map, "a"),0,0).str);

}