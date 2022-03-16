#pragma once
#include <string>

#include "global.h"

namespace myskill {
	//	�ַ�������
	#define	ngx_strcpy(str1,str2)	strcpy(str1,str2)
	#define ngx_memset(str1)		memset(str1,'\0',sizeof(str1))

	// ��ȡ��ǰʱ��
	std::string NgxLocalTimeWithTime();

	// ��ȡ��־�ȼ�
	std::string NgxLogLevel(int level = -1);

	// nginx�����ʼ��
	void NgxInitSetProcTitle();

	// nginx��������
	void NgxSetTitle(string&& title);
}