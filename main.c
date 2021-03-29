
#include "wifi.h"
#include <stdio.h>
#include "fork_signal.h"
int fd_wifi_uart = 0;
int fd_moter = 0;
int feedplan_fd = 0;
int cpid = 0;
int main()
{
	//打开与wifi模组通信的串口 ttySAC0
	fd_wifi_uart = open_uart(WIFI_UART_PATH);
	if (-1 == fd_wifi_uart)
	{
		fprintf(stderr, "open fd_wifi_uart is error!\n");
		return -1;
	}
	printf("uart open success!\n");

	//打开电机	
	fd_moter = open_motor(MOTOR_PATH);
	if (-1 == fd_moter)
	{
		fprintf(stderr, "open fd_motor is error!\n");
		return -1;
	}
	printf("motor open success!\n");

	feedplan_fd = open_feedplan_file(FEED_PLAN_PATH);
	if (-1 == feedplan_fd)
	{
		fprintf(stderr, "open feed_plan_file is error!\n");
		return -1;
	}
	printf("feed_plan_file open success!\n");
	
	// 对串口接收缓冲区进行初始化
	wifi_protocol_init();
	//设置音量
	set_speaker_voice(1);
	init();	
	cpid = fork();
	if (-1 == cpid)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (0 == cpid)  //子进程
	{	
		// 获取当前文件偏移量	如果非0，代表feedplan更新;
		int file_seek; //获取当前文件偏移量
		int i = 0;
		int ret = 1;
		get_feedplan();
		printf("child get_feedplan success!\n");
		if (signal(SIGALRM, sig_alrm) == SIG_ERR)  //捕捉SIGALRM信号
		{
			perror("signal");
			exit(EXIT_FAILURE);
		}		
		if (signal(SIGUSR1, sig_usr) == SIG_ERR)// 捕捉父进程发送给子进程的SIGUSR1信号
		{
			perror("signal");
			exit(EXIT_FAILURE);
		}
		while (1)
		{
			Alarm_clock();						
		}
		// 子进程获取喂食计划;		
	}
	else{		//父进程 
	
		int i = 0;
		time_t tim;
		struct tm * t = NULL;
		printf("cpid :%d\n", cpid);
		while (1)
		{
			//接收数据
			uart_receive();	
			wifi_uart_service();
			tim = time(NULL);
			t = localtime(&tim);
			if ((i % 30) == 0)
			{		
				if (121 > t->tm_year)  //当前时间未同步
				{
					mcu_get_wifi_connect_status();
				}
				else{
					printf("time is update success!\n");
					break;
				}
			}
			i++;
		}
		while (1)
		{
			uart_receive();
			wifi_uart_service();
		}
		wait(NULL);
		}
	return 0;	
}


