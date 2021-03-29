/*************************************************************************
    > File Name: fork_signal.c
    > 作者:YJK 
    > Mail: 745506980@qq.com 
    > Created Time: 2021年03月23日 星期二 09时43分55秒
 ************************************************************************/

#include <fcntl.h>
#include<stdio.h>
#include "protocol.h"
#include "wifi.h"

#include <sys/types.h>

#include <unistd.h>

#include "fork_signal.h"
/*SIGALRM信号到达时，执行喂食*/\
		
int feedindex = 0;   //当前是哪个闹钟
Feed_Plan feedplan[10];
void sig_alrm(int sig)
{
	//执行喂食任务
	printf("feed plan -----\n");
	motor_on('R', 10 * feedplan[feedindex].weight, 1);

}
void sig_usr(int sig)
{
	get_feedplan();		
}

unsigned int get_mintime(void) 
{
	time_t tm = time(NULL); //获取当前时间 
	struct tm *t = localtime(&tm); 	
	//获取与当前时间作比较最小的时钟
	int i = 0;
	unsigned int min_time = 0xFFFF;
	unsigned int now_time = (t->tm_hour * 60 + t->tm_min) * 60 + t->tm_sec; 
	unsigned int feed_time = 0;
	unsigned int tim = 0;
	unsigned int tomorrow_lock = 0;
	while (i < 10)   //获取距离闹钟最小的秒数
	{
		//首先判断开关是否打开
		if ((feedplan[i].swch == 1) && (feedplan[i].weekday[t->tm_wday] == 1))
		{
			// 获取距离当前的时间
			feed_time = (feedplan[i].hour * 60 + feedplan[i].minute) * 60;  //获取当前距离00:00:00经过的秒数
			tim = feed_time - now_time;
			printf("time :%d\n", tim);			
			if (tim > 0)  //说明未到达定时时间
			{
				if (min_time > tim)
				{
					min_time = tim;
					feedindex = i;
				}
			}
		}		
		if (min_time == 0xFFFF && i < 9)
		{
			unsigned char week_day;
			if (t->tm_wday < 6)
				week_day = t->tm_wday + 1;
			else 
				week_day = 0;				
			if (feedplan[i].swch == 1 && feedplan[i].weekday[week_day] == 1)
			{
				tomorrow_lock = (feedplan[i].hour * 60 + feedplan[i].minute) * 60 + 86400;
				tim = tomorrow_lock - now_time;
				if (min_time > tim)
				{
					min_time = tim;
					feedindex = i + 1;
				}
			}
		}
		i++;
	}
//	printf("min_time%d\n", min_time);	
	return min_time;
}

void Alarm_clock(void)
{

	unsigned int mintime; 
	printf("get time\n");
	mintime = get_mintime();
	printf("Alarm_clock time :%ds\n",mintime);
	alarm(mintime);
	pause();
}

int get_feedplan(void)
{
	lseek(feedplan_fd, 0 , SEEK_SET);	
	bzero(&feed_plan, sizeof(feed_plan));
	int ret = 1;
	int i = 0;
	while(ret)
	{		
		do{
			ret = read(feedplan_fd, &feedplan[i], sizeof(Feed_Plan));
		}while(ret == -1 && errno==EINTR);
		if (-1 == ret)
		{
			fprintf(stderr,"read feed_plan_file is error!\n");
			return -1;
		}
		if (ret == 0)
			break;
		printf("getplan喂食计划----%d\n", i);
		printf("Monday		: %d\n", feedplan[i].weekday[0]);
		printf("Tuesday		: %d\n", feedplan[i].weekday[1]);
		printf("Wednesday	: %d\n", feedplan[i].weekday[2]);
		printf("Thursday	: %d\n", feedplan[i].weekday[3]);
		printf("Friday		: %d\n", feedplan[i].weekday[4]);
		printf("Satureday	: %d\n", feedplan[i].weekday[5]);
		printf("Sunday		: %d\n", feedplan[i].weekday[6]);
		printf("hour		: %d\n", feedplan[i].hour);
		printf("minute		: %d\n", feedplan[i].minute);
		printf("weight		: %d\n", feedplan[i].weight);
		printf("swch		: %d\n", feedplan[i].swch);
		i++;
	}
	//读取完毕，文件偏移量到达末尾，更新偏移量
	printf("get_feedplan success!\n");
	return 0;	
}
int open_feedplan_file(const char * path)
{
	int fd = open(path, O_RDWR | O_CREAT ,0777);
	if (fd == -1)
		perror("open");
	return fd;
}

