#pragma once
#include <ctime>
#include <string.h>		// memset
#include <iostream>
#include <stdarg.h>

#include "ngx_func.h"

namespace myskill {
	using namespace std;

	// 获取当前时间
	string NgxLocalTimeWithTime() {
		time_t t = time(nullptr);
		struct tm *local_t;
		memset(&local_t, '\0', sizeof(local_t));
		local_t = localtime(&t);

		return string(to_string(local_t->tm_year + 1900) + '/' +	
							   (local_t->tm_mon + 1 < 10 ? '0' + to_string(local_t->tm_mon + 1) : to_string(local_t->tm_mon + 1)) + '/' +
							   (local_t->tm_mday < 10 ? '0' + to_string(local_t->tm_mon) : to_string(local_t->tm_mon)) + ' ' +
							   (local_t->tm_hour < 10 ? '0' + to_string(local_t->tm_hour) : to_string(local_t->tm_hour)) + ":" +
							   (local_t->tm_min < 10 ? '0' + to_string(local_t->tm_min) : to_string(local_t->tm_min)) + ":" + 
							   (local_t->tm_sec < 10 ? '0' + to_string(local_t->tm_sec) : to_string(local_t->tm_sec)) + " ");
	}

	// 获取日志等级
	string NgxLogLevel(int level) {
		switch (level) {
		case NGX_LOG_STDERR:
			return string("[STDERR] ");
			break;
		case NGX_LOG_EMERG:
			return string("[EMERG] ");
			break;
		case NGX_LOG_ALERT:
			return string("[ALERT] ");
			break;
		case NGX_LOG_CRIT:
			return string("[CRIT] ");
			break;
		case NGX_LOG_ERR:
			return string("[ERROR] ");
			break;
		case NGX_LOG_WRAN:
			return string("[WRANING] ");
			break;
		case NGX_LOG_NOTICE:
			return string("[NOTICE] ");
			break;
		case NGX_LOG_DEBUG:
			return string("[DEBUG] ");
			break;
		default:
			return " ";
			break;
		}
		return " ";
	}
}