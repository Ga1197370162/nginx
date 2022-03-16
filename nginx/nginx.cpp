#include <iostream>
#include <string.h>

#include "ngx_c_config.h"
#include "ngx_func.h"
#include "global.h"
#include "memory_pool.h"
using namespace myskill;

// ��¼���ε�ַ�������޸�nginx����
int ngx_argc = 0;					//	Ԫ�ظ���
char** ngx_argv = nullptr;			//	Ԫ�ص�ַ
size_t ngx_title_len = 0;			//	���γ���
char* ngx_environ = nullptr;		//	���������µ�ַ
size_t ngx_env_size = 0;			//	������������

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