#include <signal.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>

#include "ngx_c_config.h"
#include "ngx_func.h"
#include "global.h"

namespace myskill {
    static void ngx_start_worker_processes(int workprocess);                        //  �ӽ�����ת��������
    static int ngx_spawn_process(int threadnum, const char* process_name);         //  ���ӽ��̴���������
    static void ngx_worker_process_cycle(int threadnum, const char* process_name);  //  �ӽ��̹��ܺ�����ֻ���ӽ����ܵ�����
    static void ngx_worker_process_init(int threadnum);                             //  �ӽ��̳�ʼ������

	void ngx_master_process_cycle() {
		sigset_t set;			//	�źż�
		sigemptyset(&set);		//	����źż�

        // ������źż��е���Щ�ź�λ��1����ȷ����������Щ�źŽ���Ӱ�����
        sigaddset(&set, SIGCHLD);     //�ӽ���״̬�ı�
        sigaddset(&set, SIGALRM);     //��ʱ����ʱ
        sigaddset(&set, SIGIO);       //�첽I/O
        sigaddset(&set, SIGINT);      //�ն��жϷ�
        sigaddset(&set, SIGHUP);      //���ӶϿ�
        sigaddset(&set, SIGUSR1);     //�û������ź�
        sigaddset(&set, SIGUSR2);     //�û������ź�
        sigaddset(&set, SIGWINCH);    //�ն˴��ڴ�С�ı�
        sigaddset(&set, SIGTERM);     //��ֹ
        sigaddset(&set, SIGQUIT);     //�ն��˳���

        // SIG_BLOCK:   ��ǰ�ź������ֺ�set�źż��Ĳ���
        // SIG_UNBLOCK: ��ǰ�ź������ֺ�set�źż��Ľ���
        // SIG_SETMASK: ����������Ϊset�źż�
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) 
        {
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_master_process_cycle()��sigprocmask()ʧ��!" << endl;
        }

        // ����nginx����
        string ngx_process_name = "master process ";
        for (int i = 0; i < ngx_argc; i += 1) {
            ngx_process_name += ngx_argv[i];
        }
        NgxSetTitle(ngx_process_name);

        ngx_c_config* config = ngx_c_config::GetInstance();
        int workprocess = config->GetConfigRowInt("WorkerProcesses");
        ngx_start_worker_processes(workprocess);

        sigemptyset(&set);      //  ���������������Ѿ���ϣ�����źż��������ܹ��������յ��ź�

        for (;;) {
            sigsuspend(&set);
            sleep(1);
        }

        return;
	}

    void ngx_start_worker_processes(int workprocess) {
        for (int i = 0; i < workprocess; i += 1) {
            ngx_spawn_process(i, "worker process");
        }
        return;
    }

    int ngx_spawn_process(int threadnum, const char* process_name) {
        pid_t  pid;

        pid = fork(); 
        switch (pid)  
        {
        case -1:    //  forkʧ��
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_spawn_process() fork �����ӽ���,num = " << threadnum << ",procname = " << process_name << "ʧ��!" << endl;
            return -1;
        case 0:     //  �ӽ���
            ngx_ppid = ngx_pid;             
            ngx_pid = getpid();               
            ngx_worker_process_cycle(threadnum, process_name);      //  �����ӽ��̵Ĵ��������ڲ��Ǹ���ѭ�������ϴ�������¼�
            break;
        default:    //  �����̷��ص��Լ��Ĵ�������
            break;
        }

        return pid;
    }

    static void ngx_worker_process_cycle(int threadnum, const char* process_name) {
        //  ���ý��̵ı�־�޸�Ϊ�ӽ���
        ngx_process = NGX_PROCESS_WORKER;

        //  �ӽ��̳�ʼ���������������̣߳���ʼ��epoll��
        ngx_worker_process_init(threadnum);
        NgxSetTitle(process_name);

        for (;;) {
            ngx_process_events_and_timers();
        }

        return;
    }

    void ngx_worker_process_init(int threadnum){
        sigset_t set;
        sigemptyset(&set);

        if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_worker_process_init()��sigprocmask()ʧ��!" << endl;
        }

        // �������߳�
    }
}