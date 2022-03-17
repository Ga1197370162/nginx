#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#include "ngx_func.h"
#include "global.h"

namespace myskill {
    using namespace std;
    
    typedef struct {
        int signo;
        const char* signame;
        void (*handler)(int signo, siginfo_t* siginfo, void* ucontext);
    } ngx_signal_t;

    static void ngx_signal_handler(int signo, siginfo_t* siginfo, void* ucontext);
    static void ngx_process_get_status(void);

    // 信号集，通过该集合来注册信号
    ngx_signal_t  signals[] = {
        { SIGHUP,    "SIGHUP",           ngx_signal_handler },  
        { SIGINT,    "SIGINT",           ngx_signal_handler },   
        { SIGTERM,   "SIGTERM",          ngx_signal_handler },     
        { SIGCHLD,   "SIGCHLD",          ngx_signal_handler },       
        { SIGQUIT,   "SIGQUIT",          ngx_signal_handler },    
        { SIGIO,     "SIGIO",            ngx_signal_handler },       
        { SIGSYS,    "SIGSYS, SIG_IGN",  NULL               },       
        { 0,         NULL,               NULL               }        
    };

    // 信号初始化函数
    bool ngx_init_signals() {
        ngx_signal_t* sig;
        struct sigaction sa;        //  创建sigaction结构体保存参数

        for (sig = signals; sig->signo != 0; sig += 1) {
            memset(&sa, '\0', sizeof(struct sigaction));

            //  如果有信号触发回调函数
            if (sig->handler) {         
                sa.sa_sigaction = sig->handler;     //  设置触发函数为回调函数
                sa.sa_flags = SA_SIGINFO;           //  使 sa_sigaction 配置有效
            }
            else {
                sa.sa_handler = SIG_IGN;            //  默认的对信号忽略
            }

            sigemptyset(&sa.sa_mask);               //  将信号集上的标志位全部清零

            if (sigaction(sig->signo, &sa, NULL) == -1) {       //  注册信号并将 sigaction 配置载入该信号中去
                NGX_LOG(1) << errno << ": sigaction( " << sig->signame << ")failed" << endl;            //  失败就写日志
                return false;
            }
            return true;
        }
    }

    // 信号回调
    void ngx_signal_handler(int signo, siginfo_t* siginfo, void* ucontext)
    {
        ngx_signal_t* sig;
        string action = "";

        // 遍历寻找这个信号
        for (sig = signals; sig->signo != 0; sig += 1) {
            if (sig->signo == signo) break;
        }

        if (ngx_process == NGX_PROCESS_MASTER) {
            switch (signo) {
            case SIGCHLD:       //  子进程退出会接收到该信息
                ngx_reap = 1;
                break;
            default:
                break;
            }
        }
        else if (ngx_process == NGX_PROCESS_WORKER) {

        }
        else {

        }

        //  si_pid = sending process ID
        //  存在该进程的ID
        if (siginfo && siginfo->si_pid)  
        {
            NGX_LOG(NGX_LOG_NOTICE) << "signal " << signo << "(" << sig->signame << ") received from " << siginfo->si_pid << action << endl;
        }
        else
        {
            NGX_LOG(NGX_LOG_NOTICE) << "signal " << signo << "( " << sig->signame << " ) received " << action << endl;
        }


        //子进程状态有变化，通常是意外退出
        if (signo == SIGCHLD)
        {
            //获取子进程的结束状态
            ngx_process_get_status(); 
        }

        return;
    }

    // 避免子进程成为僵尸进程
    static void ngx_process_get_status(void)
    {
        pid_t            pid;
        int              status;
        int              err;
        int              one = 0;   //  代表这个信号将处理两次，来确保子进程不会变成僵尸进程

        for (;; )
        {
            // 第一个参数，-1 表示等待任何子进程，第二个参数：保存子进程的状态信息,第三个参数：提供额外选项，WNOHANG表示不要阻塞，让这个waitpid()立即返回   
            pid = waitpid(-1, &status, WNOHANG);      

            if (pid == 0) 
            {
                return;
            } 
     
            if (pid == -1)
            {
                err = errno;
                if (err == EINTR)           //  调用被某个信号中断
                {
                    continue;
                }

                if (err == ECHILD && one)   //  没有子进程
                {
                    return;
                }

                if (err == ECHILD)          //  没有子进程
                {
                    NGX_LOG(7) << err << ": waitpid() failed!" << endl;
                    return;
                }

                NGX_LOG(2) << err << ": waitpid() failed!" << endl;
                return;
            }  
           
            one = 1;  
            if (WTERMSIG(status))   //  获取使子进程终止的信号编号
            {
                //  获取使子进程终止的信号编号
                NGX_LOG(2) << "pid = " << pid << "exited on signal " << WTERMSIG(status) << "!" << endl; 
            }
            else
            {
                //  WEXITSTATUS()获取子进程传递给exit或者_exit参数的低八位
                NGX_LOG(6) << "pid = " << pid << "exited on signal " << WTERMSIG(status) << "!" << endl; 
            }
        }
        return;
    }
}