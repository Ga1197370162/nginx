#pragma once
#include <string>
#include <iostream>

#include "global.h"

namespace myskill {
	using namespace std;

	//	字符串操作
	#define	ngx_strcpy(str1,str2)	strcpy(str1,str2)
	#define ngx_memset(str1)		memset(str1,'\0',sizeof(str1))

	// 获取当前时间
	std::string NgxLocalTimeWithTime();

	// 获取日志等级
	std::string NgxLogLevel(int level = -1);

	// nginx标题初始化
	void NgxInitSetProcTitle();

	// nginx标题设置
	void NgxSetTitle(const string& title);

	// 信号初始化函数
	bool ngx_init_signals();

	// 主进程处理函数
	void ngx_master_process_cycle();

	// 子进程定时事件处理函数
	void ngx_process_events_and_timers();

	// 守护进程
	int ngx_daemon();
}