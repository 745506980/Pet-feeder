/*************************************************************************
    > File Name: uart.h
    > 作者:YJK 
    > Mail: 745506980@qq.com 
    > Created Time: 2021年03月17日 星期三 20时00分45秒
 ************************************************************************/

#ifndef UART__H 
#define UART__H

#define WIFI_UART_PATH "/dev/ttySAC0"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* 对串口进行配置　
 * 波特率 数据位　停止位　奇偶校验*/
int Set_Uart(int fd, int nSpeed, int nBits, int nStop, char nCheck);


extern int fd_wifi_uart; //　wifi 模块串口　
extern int fd_moter_uart;	//　电机模块串口

int open_uart(const char * path);

	
	
#endif
/*防止头文件重复定义*/
