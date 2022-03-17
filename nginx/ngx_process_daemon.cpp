#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ngx_func.h"

namespace myskill {
	// �ӽ��� 0 ������ 1 error -1
	int ngx_daemon() {
		// ���� setsid ֻ���ӽ�����Ч������ͨ�����ַ�ʽʹ�������ȷ���һ�Σ��ٽ��������˳������ӽ��̴��������̼�������
		// ͬʱ�������Ѿ��˳��ˣ�ֻ���ӽ��̣������ն��ϲ������������ӽ�������ʧȥ���������Իᱻroot���̽ӹ�
		switch (fork()) {
		case -1:
			NGX_LOG(NGX_LOG_EMERG) << errno << " ngx_daemon()��fork()ʧ��!" << endl;
			break;
		case 0:
			//	�ӽ���
			break;
		default:
			//	�����̻ص�������ֱ���˳�����
			return 1;
		}

		// ԭ������ID���Ǹ����̵�ID�����Խ�������̵ĸ�ID����ngx_pid
		ngx_ppid = ngx_pid;
		ngx_pid = getpid();

		//	ʹ������̳�Ϊ�ỰID
		if (setsid() == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()��setsid()ʧ��!" << endl;
			return -1;
		}

		// �������ļ�Ȩ��
		umask(0);

		int fd = open("/dev/null",O_RDWR);
		if (fd == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()��open(\"/dev/null\")ʧ��!" << endl;
			return -1;
		}

		//	��������ָ��ڶ��ļ���������ʧЧ
		if (dup2(fd, STDIN_FILENO) == -1) {
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()��dup2(STDIN)ʧ��!" << endl;
			return -1;
		}

		//	��׼���
		if (dup2(fd, STDOUT_FILENO) == -1) 
		{
			NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()��dup2(STDOUT)ʧ��!" << endl;
			return -1;
		}

		if (fd > STDERR_FILENO)  
		{
			//	ȷ���ļ������ر�
			if (close(fd) == -1)  
			{
				NGX_LOG(NGX_LOG_EMERG) << errno << ": ngx_daemon()��close(fd)ʧ��!" << endl;
				return -1;
			}
		}

		return 0;
	}
	
}