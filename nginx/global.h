#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <signal.h> 

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

// ��������
typedef enum {
	NGX_PROCESS_MASTER,
	NGX_PROCESS_WORKER
};

extern int ngx_argc;				//	ָ���
extern char** ngx_argv;				//	���ε�ַ
extern size_t ngx_title_len;		//	���γ���
extern char* ngx_environ;			//	��������
extern size_t ngx_env_size;			//	������������

extern pid_t ngx_pid;				//	��ǰ���̵�pid
extern pid_t ngx_ppid;				//	�����̵�pid
extern sig_atomic_t ngx_reap;		//	ԭ�Ӳ�������¼�ӽ���״̬
extern unsigned char ngx_process;	//	��¼�ý�������
extern bool ngx_shutdown;			//	�����˳���־λ
#endif 