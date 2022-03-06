#include <iostream>
#include <string>
#include <string.h>

#include "ngx_func.h"
#include "global.h"

//#define DEBUG

namespace myskill {
	using namespace std;

	void NgxInitSetProcTitle() {
		// �ȼ���������ָ���
		size_t TitleLen = 0;
		for (int i = 0; i < ngx_argc; i++) {
			TitleLen += strlen(ngx_argv[i]) + 1;
		}

		// ������Ļ���������������
#ifdef DEBUG
		cout << *(ngx_argv + (TitleLen * sizeof(char))) << endl;	//	windows���ֶ���
#endif
		// ��¼env��ʼ��ַ��vs����û��environȫ�ֱ���
		char** env = ngx_argv + (TitleLen * sizeof(char));
		for (int i = 0; env[i]; i++) {
			cout << env[i] << endl;
		}
	}

	void NgxSetTitle(string&& title) {
		
	}
}