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
		// �ȼ���������ָ���
		ngx_title_len = 0;
		for (int i = 0; i < ngx_argc; i++) {
			ngx_title_len += strlen(ngx_argv[i]) + 1;
		}

		// ������Ļ���������������
#ifdef DEBUG
		cout << *(ngx_argv + (ngx_title_len * sizeof(char))) << endl;	//	windows���ֶ���
#endif
		ngx_env_size = 0;
		// ��¼env��ʼ��ַ��vs����û��environȫ�ֱ���
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
		// ���õ��±���̫����
		if (new_title_len >= all_len) {
			return;
		}

		default_head += title;
		ngx_argv[1] = nullptr;
		char* p = ngx_argv[0];
		strcpy(p, default_head.c_str());
		p += new_title_len;
		all_len -= new_title_len;
		// ����������в������
		memset(p, '\0', all_len);

		return;
	}
}