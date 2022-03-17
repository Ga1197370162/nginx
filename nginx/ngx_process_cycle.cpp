#include <signal.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>

#include "ngx_c_config.h"
#include "ngx_func.h"
#include "global.h"

namespace myskill {
    static void ngx_start_worker_processes(int workprocess);                        //  子进程轮转创建函数
    static int ngx_spawn_process(int threadnum, const char* process_name);         //  单子进程创建处理函数
    static void ngx_worker_process_cycle(int threadnum, const char* process_name);  //  子进程功能函数，只有子进程能到这里
    static void ngx_worker_process_init(int threadnum);                             //  子进程初始化工作

	void ngx_master_process_cycle() {
		sigset_t set;			//	信号集
		sigemptyset(&set);		//	清空信号集

        // 将这个信号集中的这些信号位置1，来确保不会有这些信号进入影响进程
        sigaddset(&set, SIGCHLD);     //子进程状态改变
        sigaddset(&set, SIGALRM);     //定时器超时
        sigaddset(&set, SIGIO);       //异步I/O
        sigaddset(&set, SIGINT);      //终端中断符
        sigaddset(&set, SIGHUP);      //连接断开
        sigaddset(&set, SIGUSR1);     //用户定义信号
        sigaddset(&set, SIGUSR2);     //用户定义信号
        sigaddset(&set, SIGWINCH);    //终端窗口大小改变
        sigaddset(&set, SIGTERM);     //终止
        sigaddset(&set, SIGQUIT);     //终端退出符

        // SIG_BLOCK:   当前信号屏蔽字和set信号集的并集
        // SIG_UNBLOCK: 当前信号屏蔽字和set信号集的交集
        // SIG_SETMASK: 设置屏蔽字为set信号集
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) 
        {
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_master_process_cycle()中sigprocmask()失败!" << endl;
        }

        // 重设nginx标题
        string ngx_process_name = "master process ";
        for (int i = 0; i < ngx_argc; i += 1) {
            ngx_process_name += ngx_argv[i];
        }
        NgxSetTitle(ngx_process_name);

        ngx_c_config* config = ngx_c_config::GetInstance();
        int workprocess = config->GetConfigRowInt("WorkerProcesses");
        ngx_start_worker_processes(workprocess);

        sigemptyset(&set);      //  基本主进程事情已经完毕，清空信号集，让他能够继续接收到信号

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
        case -1:    //  fork失败
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_spawn_process() fork 产生子进程,num = " << threadnum << ",procname = " << process_name << "失败!" << endl;
            return -1;
        case 0:     //  子进程
            ngx_ppid = ngx_pid;             
            ngx_pid = getpid();               
            ngx_worker_process_cycle(threadnum, process_name);      //  进入子进程的处理函数，内部是个死循环，不断处理相关事件
            break;
        default:    //  父进程返回到自己的处理函数中
            break;
        }

        return pid;
    }

    static void ngx_worker_process_cycle(int threadnum, const char* process_name) {
        //  将该进程的标志修改为子进程
        ngx_process = NGX_PROCESS_WORKER;

        //  子进程初始化函数，创建多线程，初始化epoll等
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
            NGX_LOG(NGX_LOG_ALERT) << errno << ": ngx_worker_process_init()中sigprocmask()失败!" << endl;
        }

        // 创建多线程
    }
}