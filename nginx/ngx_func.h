#pragma once
#include <string>

#include "global.h"

namespace myskill {
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
	void NgxSetTitle(string&& title);
}