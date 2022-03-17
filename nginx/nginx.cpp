#include <iostream>
#include <string.h>
#include <unistd.h>

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

// �������
pid_t ngx_pid;						//	��ǰ���̵�pid
pid_t ngx_ppid;						//	�����̵�pid
sig_atomic_t ngx_reap = 0;			//	ԭ�Ӳ�������¼�ӽ���״̬�仯
unsigned char ngx_process = NGX_PROCESS_MASTER;		//	Ĭ��Ϊmaster����
bool ngx_shutdown = false;			//	�����˳���־λ
bool ngx_daemon_flag = false;		//	�Ƿ�ʹ�����ػ�����

// ��ȡconfig�����õ���־�ȼ��ָ���
ngx_c_log* log = ngx_c_log::GetInstance();
int ngx_log_level = ngx_c_config::GetInstance()->GetConfigRowInt("LogLevel");

int main(int argc,char* argv[]) {
	// ��¼����ָ����Ϣ
	ngx_argc = argc;
	ngx_argv = argv;

	// ��¼������Ϣ
	ngx_pid = getpid();
	ngx_ppid = getppid();

	// �����������
	NgxInitSetProcTitle();

	// ��ʼ���ź�
	if (!ngx_init_signals()) {
		NGX_LOG(1) << "ngx_init_signals() fail!!" << endl;
		exit(0);
	}

	// �ػ����̳�ʼ��
	if (ngx_c_config::GetInstance()->GetConfigRowInt("Daemon") == 1) {
		int daemon_result = ngx_daemon();

		if (daemon_result == -1) {			//	error
			NGX_LOG(1) << "ngx_daemon() fork fail!" << endl;
			exit(0);
		}
		else if (daemon_result == 1) {		//	������
			exit(0);
		}

		ngx_daemon_flag = true;
	}

	// ��������Ҫ������
	ngx_master_process_cycle();
	return 0;
}