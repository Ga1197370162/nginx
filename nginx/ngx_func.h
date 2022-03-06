#pragma once
#include <string>

#include "global.h"

namespace myskill {
	//	�ַ�������
	#define	ngx_strcpy(str1,str2)	strcpy(str1,str2)
	#define ngx_memset(str1)		memset(str1,'\0',sizeof(str1))

	// ��ȡ��ǰʱ��
	static std::string NgxLocalTimeWithTime();

	// ��ȡ��־�ȼ�
	static std::string NgxLogLevel(int level = -1);

	// nginx�����ʼ��
	void NgxInitSetProcTitle();
}