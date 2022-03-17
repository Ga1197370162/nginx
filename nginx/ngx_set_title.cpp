#include <iostream>
#include <string>
#include <string.h>

#include "memory_pool.h"
#include "ngx_func.h"
#include "global.h"

//#define DEBUG

namespace myskill {
	using namespace std;

	void NgxInitSetProcTitle() {
		// 先计算出传入的指令长度
		ngx_title_len = 0;
		for (int i = 0; i < ngx_argc; i++) {
			ngx_title_len += strlen(ngx_argv[i]) + 1;
		}

		// 将后面的环境变量参数搬移
#ifdef DEBUG
		cout << *(ngx_argv + (ngx_title_len * sizeof(char))) << endl;	//	windows有字对齐
#endif
		ngx_env_size = 0;
		// 记录env起始地址，vs环境没有environ全局变量
		char** env = ngx_argv + (ngx_title_len * sizeof(char));
		for (int i = 0; env[i]; i++) {
			ngx_env_size += strlen(env[i]) + 1;
		}

		ngx_environ = make_memory<char>(ngx_env_size);
		char* p = ngx_environ;
		for (int i = 0; env[i]; i++) {
			size_t size = strlen(env[i]) + 1;
			strcpy(p, env[i]);
			env[i] = p;
			p += size;
		}

#ifdef DEBUG
		p = ngx_environ;
		for (int i = 0; env[i]; i++) {
			size_t size = strlen(env[i]) + 1;
			cout << p << endl;
			p += size;

		}
#endif
		return;
	}

	void NgxSetTitle(const string& title) {
		string default_head = "nginx: ";

		size_t new_title_len = default_head.length() + title.length();
		size_t all_len = ngx_title_len + ngx_env_size;
		// 设置的新标题太长了
		if (new_title_len >= all_len) {
			return;
		}

		default_head += title;
		ngx_argv[1] = nullptr;
		char* p = ngx_argv[0];
		strcpy(p, default_head.c_str());
		p += new_title_len;
		all_len -= new_title_len;
		// 将后面的所有参数清空
		memset(p, '\0', all_len);

		return;
	}
}