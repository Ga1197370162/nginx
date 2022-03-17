#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <signal.h> 

#include "ngx_c_log.h"
using namespace myskill;

// 日志等级
using Ngx_Log_Status = enum {
	NGX_LOG_STDERR = 0,		//	控制台
	NGX_LOG_EMERG,			//	紧急
	NGX_LOG_ALERT,			//	警戒
	NGX_LOG_CRIT,			//	严重
	NGX_LOG_ERR,			//	错误
	NGX_LOG_WRAN,			//	警告
	NGX_LOG_NOTICE,			//	注意
	NGX_LOG_INFO,			//	信息
	NGX_LOG_DEBUG,			//	调试
};

// 快捷日志记录方法
#define NGX_LOG(Level) ((ngx_log_level < Level) ? ((cout) << NgxLocalTimeWithTime() << NgxLogLevel(Level)) : ((*log) << NgxLocalTimeWithTime() << NgxLogLevel(Level))) 

extern class ngx_c_log* log;	//	日志对象
extern int ngx_log_level;		//	日志定义的最低等级，低于该等级就不会记录日志，采用输出的方式

// 进程类型
typedef enum {
	NGX_PROCESS_MASTER,
	NGX_PROCESS_WORKER
};

extern int ngx_argc;				//	指令传参
extern char** ngx_argv;				//	传参地址
extern size_t ngx_title_len;		//	传参长度
extern char* ngx_environ;			//	环境变量
extern size_t ngx_env_size;			//	环境变量长度

extern pid_t ngx_pid;				//	当前进程的pid
extern pid_t ngx_ppid;				//	父进程的pid
extern sig_atomic_t ngx_reap;		//	原子操作，记录子进程状态
extern unsigned char ngx_process;	//	记录该进程类型
extern bool ngx_shutdown;			//	进程退出标志位
#endif 