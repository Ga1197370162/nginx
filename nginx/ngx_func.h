#pragma once
#include <string>
#include <iostream>

#include "global.h"

namespace myskill {
	using namespace std;

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
	void NgxSetTitle(const string& title);

	// �źų�ʼ������
	bool ngx_init_signals();

	// �����̴�����
	void ngx_master_process_cycle();

	// �ӽ��̶�ʱ�¼�������
	void ngx_process_events_and_timers();

	// �ػ�����
	int ngx_daemon();
}