#include <iostream>
#include <string.h>
#include <unistd.h>

#include "ngx_c_config.h"
#include "ngx_func.h"
#include "global.h"
#include "memory_pool.h"

using namespace myskill;

// 记录传参地址，用于修改nginx标题
int ngx_argc = 0;					//	元素个数
char** ngx_argv = nullptr;			//	元素地址
size_t ngx_title_len = 0;			//	传参长度
char* ngx_environ = nullptr;		//	环境变量新地址
size_t ngx_env_size = 0;			//	环境变量长度

// 进程相关
pid_t ngx_pid;						//	当前进程的pid
pid_t ngx_ppid;						//	父进程的pid
sig_atomic_t ngx_reap = 0;			//	原子操作，记录子进程状态变化
unsigned char ngx_process = NGX_PROCESS_MASTER;		//	默认为master进程
bool ngx_shutdown = false;			//	进程退出标志位
bool ngx_daemon_flag = false;		//	是否使用了守护进程

// 获取config中设置的日志等级分割线
ngx_c_log* log = ngx_c_log::GetInstance();
int ngx_log_level = ngx_c_config::GetInstance()->GetConfigRowInt("LogLevel");

int main(int argc,char* argv[]) {
	// 记录传参指令信息
	ngx_argc = argc;
	ngx_argv = argv;

	// 记录进程信息
	ngx_pid = getpid();
	ngx_ppid = getppid();

	// 环境变量搬家
	NgxInitSetProcTitle();

	// 初始化信号
	if (!ngx_init_signals()) {
		NGX_LOG(1) << "ngx_init_signals() fail!!" << endl;
		exit(0);
	}

	// 守护进程初始化
	if (ngx_c_config::GetInstance()->GetConfigRowInt("Daemon") == 1) {
		int daemon_result = ngx_daemon();

		if (daemon_result == -1) {			//	error
			NGX_LOG(1) << "ngx_daemon() fork fail!" << endl;
			exit(0);
		}
		else if (daemon_result == 1) {		//	父进程
			exit(0);
		}

		ngx_daemon_flag = true;
	}

	// 主进程主要处理函数
	ngx_master_process_cycle();
	return 0;
}