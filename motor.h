/*************************************************************************
    > File Name: motor.h
    > 作者:YJK 
    > Mail: 745506980@qq.com 
    > Created Time: 2021年03月23日 星期二 12时32分44秒
 ************************************************************************/

#ifndef MOTOR__H 
#define MOTOR__H


#define MOTOR_PATH "/dev/step_motor_driver" //step_motor_driver

#define CMD_STEPMOTOR_A    _IOW('L', 0, unsigned long)
#define CMD_STEPMOTOR_B    _IOW('L', 1, unsigned long)
#define CMD_STEPMOTOR_C	   _IOW('L', 2, unsigned long)
#define CMD_STEPMOTOR_D    _IOW('L', 3, unsigned long)

#define HIGHT 1
#define LOW 0


extern int fd_moter;
// 电机相关

//打开电机
int open_motor(const char * path);

//将GPIO　拉低
void set_motor_off(void);
// speed 打开低电平的比例  0 0%  1 33%  2 50% 3 66%
int motor_on(const unsigned char direction, unsigned short feed_num, unsigned short speed);

#endif
/*防止头文件重复定义*/
