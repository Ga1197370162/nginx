#include <iostream>
#include <string.h>

#include "ngx_c_config.h"
#include "ngx_func.h"
#include "global.h"
#include "memory_pool.h"


using namespace myskill;

// 记录传参地址，用于修改nginx标题
int ngx_argc;			//	元素个数
char** ngx_argv;		//	元素地址

ngx_c_log* log = ngx_c_log::GetInstance();
int ngx_log_level = ngx_c_config::GetInstance()->GetConfigRowInt("LogLevel");

int main(int argc,char* argv[]) {
	ngx_argc = argc;
	ngx_argv = argv;

	NgxInitSetProcTitle();

	p_ngx_memory_pool p = memeory_c_pool::GetInstance();
	char*temp = p->make_AllocMemory<char>(60);

	std::cin.get();
	return 0;
}