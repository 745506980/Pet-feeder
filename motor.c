/*************************************************************************
    > File Name: motor.c
    > 作者:YJK 
    > Mail: 745506980@qq.com 
    > Created Time: 2021年03月23日 星期二 15时36分53秒
 ************************************************************************/

#include "motor.h"
#include "protocol.h"
#include"wifi.h"
#include <sys/ioctl.h>
int open_motor(const char * path)
{
	int fd = open(path, O_RDWR | O_NDELAY);
	if (-1 == fd)
	{
		perror("motor open");
		return -1;
	}
	else 
		return fd;
}
void set_motor_off(void)
{

	ioctl(fd_moter, CMD_STEPMOTOR_C, LOW);   //GPIO拉低
	ioctl(fd_moter, CMD_STEPMOTOR_D, LOW);

}

int motor_on(const unsigned char direction, unsigned short feed_num, unsigned short speed)
{
	feed_status.feed_report = feed_num / 10;
	if(mcu_dp_value_update(DPID_FEED_REPORT,feed_status.feed_report) == SUCCESS)  //喂食结果上报
		printf ("feed_report:%ld\n", feed_status.feed_report);
	
	if(direction == 'R')
	{
		printf ("slow_feed :%d\n", feed_status.slow_feed);
		mcu_dp_enum_update(DPID_FEED_STATE,feeding);
		while (feed_num)
		{
			ioctl(fd_moter, CMD_STEPMOTOR_C, HIGHT);
			ioctl(fd_moter, CMD_STEPMOTOR_D, LOW);
			usleep(3000);
			ioctl(fd_moter, CMD_STEPMOTOR_C, LOW);
			ioctl(fd_moter, CMD_STEPMOTOR_D, LOW);
			if (feed_status.slow_feed == TRUE)
				usleep(6000);
			else 
				usleep(3000);
			feed_num--;	
		}
		mcu_dp_enum_update(DPID_FEED_STATE,done);
		
	}
	set_motor_off();
}

