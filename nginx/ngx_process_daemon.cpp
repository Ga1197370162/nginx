#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ngx_func.h"

namespace myskill {
	// 子进程 0 父进程 1 error -1
	int ngx_daemon() {
		// 由于 setsid 只对子进程有效，所以通过这种方式使主进程先分裂一次，再将主进程退出程序，子进程代替主进程继续工作
		// 同时主进程已经退出了，只有子进程，所以终端上不会阻塞，而子进程由于失去父进程所以会被root进程接管
		switch (fork()) {
		case -1:
			NGX_LOG(NGX_LOG_EMERG) << errno << " ngx_daemon()中fork()失败!" << endl;
			break;
		case 0:
			//	子进程
			break;
		default:
			//	父进程回到主进程直接退出程序
			return 1;
		}

		// 原来进程ID就是父进程的ID，所以将这个进程的父ID就是ngx_pid
		ngx_ppid = ngx_pid;
		ngx_pid = getpid();

		//	使这个进程成为会话ID
		if (setsid() == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()中setsid()失败!" << endl;
			return -1;
		}

		// 不限制文件权限
		umask(0);

		int fd = open("/dev/null",O_RDWR);
		if (fd == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()中open(\"/dev/null\")失败!" << endl;
			return -1;
		}

		//	将输入流指向黑洞文件，即输入失效
		if (dup2(fd, STDIN_FILENO) == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()中dup2(STDIN)失败!" << endl;
			return -1;
		}

		//	标准输出
		if (dup2(fd, STDOUT_FILENO) == -1) 
		{
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()中dup2(STDOUT)失败!" << endl;
			return -1;
		}

		if (fd > STDERR_FILENO)  
		{
			//	确保文件流被关闭
			if (close(fd) == -1)  
			{
				NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()中close(fd)失败!" << endl;
				return -1;
			}
		}

		return 0;
	}
	
}