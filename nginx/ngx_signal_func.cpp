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

    // �źż���ͨ���ü�����ע���ź�
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

    // �źų�ʼ������
    bool ngx_init_signals() {
        ngx_signal_t* sig;
        struct sigaction sa;        //  ����sigaction�ṹ�屣�����

        for (sig = signals; sig->signo != 0; sig += 1) {
            memset(&sa, '\0', sizeof(struct sigaction));

            //  ������źŴ����ص�����
            if (sig->handler) {         
                sa.sa_sigaction = sig->handler;     //  ���ô�������Ϊ�ص�����
                sa.sa_flags = SA_SIGINFO;           //  ʹ sa_sigaction ������Ч
            }
            else {
                sa.sa_handler = SIG_IGN;            //  Ĭ�ϵĶ��źź���
            }

            sigemptyset(&sa.sa_mask);               //  ���źż��ϵı�־λȫ������

            if (sigaction(sig->signo, &sa, NULL) == -1) {       //  ע���źŲ��� sigaction ����������ź���ȥ
                NGX_LOG(1) << errno << ": sigaction( " << sig->signame << ")failed" << endl;            //  ʧ�ܾ�д��־
                return false;
            }
            return true;
        }
    }

    // �źŻص�
    void ngx_signal_handler(int signo, siginfo_t* siginfo, void* ucontext)
    {
        ngx_signal_t* sig;
        string action = "";

        // ����Ѱ������ź�
        for (sig = signals; sig->signo != 0; sig += 1) {
            if (sig->signo == signo) break;
        }

        if (ngx_process == NGX_PROCESS_MASTER) {
            switch (signo) {
            case SIGCHLD:       //  �ӽ����˳�����յ�����Ϣ
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
        //  ���ڸý��̵�ID
        if (siginfo && siginfo->si_pid)  
        {
            NGX_LOG(NGX_LOG_NOTICE) << "signal " << signo << "(" << sig->signame << ") received from " << siginfo->si_pid << action << endl;
        }
        else
        {
            NGX_LOG(NGX_LOG_NOTICE) << "signal " << signo << "( " << sig->signame << " ) received " << action << endl;
        }


        //�ӽ���״̬�б仯��ͨ���������˳�
        if (signo == SIGCHLD)
        {
            //��ȡ�ӽ��̵Ľ���״̬
            ngx_process_get_status(); 
        }

        return;
    }

    // �����ӽ��̳�Ϊ��ʬ����
    static void ngx_process_get_status(void)
    {
        pid_t            pid;
        int              status;
        int              err;
        int              one = 0;   //  ��������źŽ��������Σ���ȷ���ӽ��̲����ɽ�ʬ����

        for (;; )
        {
            // ��һ��������-1 ��ʾ�ȴ��κ��ӽ��̣��ڶ��������������ӽ��̵�״̬��Ϣ,�������������ṩ����ѡ�WNOHANG��ʾ��Ҫ�����������waitpid()��������   
            pid = waitpid(-1, &status, WNOHANG);      

            if (pid == 0) 
            {
                return;
            } 
     
            if (pid == -1)
            {
                err = errno;
                if (err == EINTR)           //  ���ñ�ĳ���ź��ж�
                {
                    continue;
                }

                if (err == ECHILD && one)   //  û���ӽ���
                {
                    return;
                }

                if (err == ECHILD)          //  û���ӽ���
                {
                    NGX_LOG(7) << err << ": waitpid() failed!" << endl;
                    return;
                }

                NGX_LOG(2) << err << ": waitpid() failed!" << endl;
                return;
            }  
           
            one = 1;  
            if (WTERMSIG(status))   //  ��ȡʹ�ӽ�����ֹ���źű��
            {
                //  ��ȡʹ�ӽ�����ֹ���źű��
                NGX_LOG(2) << "pid = " << pid << "exited on signal " << WTERMSIG(status) << "!" << endl; 
            }
            else
            {
                //  WEXITSTATUS()��ȡ�ӽ��̴��ݸ�exit����_exit�����ĵͰ�λ
                NGX_LOG(6) << "pid = " << pid << "exited on signal " << WTERMSIG(status) << "!" << endl; 
            }
        }
        return;
    }
}