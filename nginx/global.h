#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "ngx_c_log.h"
using namespace myskill;

// ��־�ȼ�
using Ngx_Log_Status = enum {
	NGX_LOG_STDERR = 0,		//	����̨
	NGX_LOG_EMERG,			//	����
	NGX_LOG_ALERT,			//	����
	NGX_LOG_CRIT,			//	����
	NGX_LOG_ERR,			//	����
	NGX_LOG_WRAN,			//	����
	NGX_LOG_NOTICE,			//	ע��
	NGX_LOG_INFO,			//	��Ϣ
	NGX_LOG_DEBUG,			//	����
};

// �����־��¼����
#define NGX_LOG(Level) ((ngx_log_level < Level) ? ((cout) << NgxLocalTimeWithTime() << NgxLogLevel(Level)) : ((*log) << NgxLocalTimeWithTime() << NgxLogLevel(Level))) 

extern class ngx_c_log* log;	//	��־����
extern int ngx_log_level;		//	��־�������͵ȼ������ڸõȼ��Ͳ����¼��־����������ķ�ʽ

extern int ngx_argc;			//	ָ���
extern char** ngx_argv;			//	���ε�ַ
extern size_t ngx_title_len;	//	���γ���
extern char* ngx_environ;		//	��������
extern size_t ngx_env_size;		//	������������

#endif 