#ifndef _mj_last_error_h_
#define _mj_last_error_h_ 1

#ifdef _WIN32
const char* mj_get_last_win_error();
const char* mj_get_last_win_error_with_code(unsigned long code);
#endif

const char* mj_get_last_crt_error();
const char* mj_get_last_crt_error_with_code(int code);

#endif // _last_error_h_