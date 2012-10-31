

#ifndef _mj_rollingfile_h_
#define _mj_rollingfile_h_ 1

#include <stdarg.h>
#include "meijing/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * һ��ѭ����¼���ļ����
 */
struct mj_rollingfile_s;

typedef struct mj_rollingfile_s mj_rollingfile_t;

/**
 * ����һ������ѭ����¼���ļ����
 * @params[in] nm  �ļ���
 * @params[in] max_size  ѭ������ֵ�����ļ���С������ֵʱ����
 * ���Դ������ļ�
 * @params[in] remain_count �ļ��ı�������
 */
DLL_VARIABLE mj_rollingfile_t* mj_rollingfile_create(const char* nm, size_t max_size, size_t remain_count);

/**
 * �رտ���ѭ����¼���ļ����
 */
DLL_VARIABLE void mj_rollingfile_free(mj_rollingfile_t* rf);

/**
 * ���ļ���д�����ݣ�ע�⵱���ļ���С����ѭ����ֵʱ����ɾ����
 * ����ļ������������ļ���������д�����ļ���
 */
DLL_VARIABLE void mj_rollingfile_write(mj_rollingfile_t* rf, const char* data, size_t n);

/**
 * �������е���������д�����
 */
DLL_VARIABLE void mj_rollingfile_flush(mj_rollingfile_t* rf);

#ifdef __cplusplus
}
#endif

#endif
	

