#include <iostream>
#include <string>
#include <string.h>

#include "ngx_func.h"
#include "global.h"

//#define DEBUG

namespace myskill {
	using namespace std;

	void NgxInitSetProcTitle() {
		// 先计算出传入的指令长度
		size_t TitleLen = 0;
		for (int i = 0; i < ngx_argc; i++) {
			TitleLen += strlen(ngx_argv[i]) + 1;
		}

		// 将后面的环境变量参数搬移
#ifdef DEBUG
		cout << *(ngx_argv + (TitleLen * sizeof(char))) << endl;	//	windows有字对齐
#endif
		// 记录env起始地址，vs环境没有environ全局变量
		char** env = ngx_argv + (TitleLen * sizeof(char));
		for (int i = 0; env[i]; i++) {
			cout << env[i] << endl;
		}
	}

	void NgxSetTitle(string&& title) {
		
	}
}