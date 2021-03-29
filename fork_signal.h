/*************************************************************************
    > File Name: fork_signal.h
    > 作者:YJK 
    > Mail: 745506980@qq.com 
    > Created Time: 2021年03月23日 星期二 09时44分01秒
 ************************************************************************/

#ifndef	FORK__SIGNAL__H 
#define FORK__SIGNAL__H
		

/* １ 父子进程通过管道进行通信
 * 2  子进程负责定时器操作，当喂食计划时间到达时，给父进程发送信号SIGUSR1信号,父进程开始喂食
 * 3  防止子进程变成孤儿进程，这里父进程是接收到
 * */
#define FEED_PLAN_PATH "feed_plan_file"
extern int feedplan_fd;
extern int cpid;
void sig_alrm(int sig);  //当子进程的闹钟到达时，内核发送SIGALRM信号，子进程执行定时喂食任务

void Alarm_clock(void);

unsigned int get_mintime(void) ;

void sig_usr(int);
int get_feedplan(void);

int open_feedplan_file(const char * path);

#endif
/*防止头文件重复定义*/
