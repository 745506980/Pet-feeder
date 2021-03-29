#########################################################################
# File Name: Makefile
# 作者:YJK 
# mail: 745506980@qq.comc
# Created Time: 2021年03月20日 星期六 09时49分30秒
#########################################################################
#    $^ 当前规则中的所有依赖
#    $< 当前规则中的第一个依赖
#    $@ 当前规则中触发命令生成的目标
#    @ 不把执行的信息打印到显示屏上

CC = arm-linux-gcc

TARGET = main

OBJS += main.o
OBJS += mcu_api.o
OBJS += protocol.o
OBJS += system.o
OBJS += uart.o
OBJS += motor.o
OBJS += fork_signal.o


CFLAGS = -Wall -g

$(TARGET) : $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

%.o : %.c
	$(CC) -c $< -o $@


clean:
	rm -rf $(TARGET) $(OBJS)
install:
	cp main /home/nfs
