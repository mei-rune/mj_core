
#include "variables_private.h"
#include "meijing/itostr.h"


#ifdef __cplusplus
extern "C"
{
#endif

DLL_VARIABLE void mj_variables_format(mj_variables_t* obj, format_fn_t cb, void* ctxt)
{
	switch(obj->o_type) {
		case mj_variable_type_string: {
			cstring_t str = mj_variables_to_string(obj, "", 0);
			(*cb)( "\"", 1, ctxt);
			(*cb)(string_data(&str), string_length(&str), ctxt);
			(*cb)( "\"", 1, ctxt);
			break;
		}
		case mj_variable_type_table: {
			size_t idx;
			(*cb)("{", 3, ctxt);
			for( idx = 0; idx < mj_variables_length(obj); ++ idx) {
				if(0 != idx)
					(*cb)(",", 1, ctxt);

				mj_variables_format(mj_variables_element_at(obj, idx), cb, ctxt);
			}
			(*cb)("}", 1, ctxt);
			break;
		}
		case mj_variable_type_list: {
			size_t idx;
			(*cb)("[", 1, ctxt);
			for( idx = 0; idx < mj_variables_length(obj); ++ idx) {
				if(0 != idx)
					(*cb)(",", 1, ctxt);

				mj_variables_format(mj_variables_element_at(obj, idx), cb, ctxt);
			}
			(*cb)("]", 1, ctxt);
			break;
		}
		default: {
			cstring_t s = mj_variables_to_string(obj, NULL, 0);
			(*cb)(s.str, s.len, ctxt);
		}
	}
}

#ifdef __cplusplus
}
#endif

