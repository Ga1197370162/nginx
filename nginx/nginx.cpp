#include <iostream>
#include <string.h>

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

ngx_c_log* log = ngx_c_log::GetInstance();
int ngx_log_level = ngx_c_config::GetInstance()->GetConfigRowInt("LogLevel");

int main(int argc,char* argv[]) {
	ngx_argc = argc;
	ngx_argv = argv;

	NgxInitSetProcTitle();

	char* p = make_memory<char>(10);
	strcpy(p, "asdasd");
	NGX_LOG(8) << p << endl;

	std::cin.get();
	return 0;
}