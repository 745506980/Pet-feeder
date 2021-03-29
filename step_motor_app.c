#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define CMD_STEPMOTOR_A    _IOW('L', 0, unsigned long)
#define CMD_STEPMOTOR_B    _IOW('L', 1, unsigned long)
#define CMD_STEPMOTOR_C	   _IOW('L', 2, unsigned long)
#define CMD_STEPMOTOR_D    _IOW('L', 3, unsigned long)

#define HIGHT 1
#define LOW 0

//#define STEPMOTOR_DEBUG
#ifdef  STEPMOTOR_DEBUG
#define DPRINTK(x...) printf("STEPMOTOR_DEBUG DEBUG:" x)
#else
#define DPRINTK(x...)
#endif

int fd;
void step_turn(unsigned long a , unsigned long delay);
void step_motor_num(char rl,unsigned long num,unsigned long speed);
void step_motor_off(void);

struct stepmotor_par{
            char  direction;
            unsigned long  num;
            unsigned long  speed;
};


int main(int argc,char *argv[])
{
	
	char *step_motor = "/dev/step_motor_driver";
	struct stepmotor_par step;

	//printf("argv[1] = %s \r\n",argv[1]);
	//printf("argv[2] = %s \r\n",argv[2]);
	//printf("argv[3] = %s \r\n",argv[3]);
	//printf("argv[4] = %s \n",argv[4]);

    if(argc<4)
    {
    	printf("Please enter the following parameters\n\r");
    	printf("Direction：R or L\n\r");
    	printf("num:10-4096\n\r");
    	printf("speed:3*1000-20*1000\n\r");
	    printf("Give an example :Turn right a circle\n\r\t./step_motor_app R 4076 3000\n\r");
		return 1;
    }

	if((fd = open(step_motor,O_RDWR|O_NDELAY))<0)
	{
		printf("APP open %s failed\r\n",step_motor);
		return -1;
	}
	else{
		printf("APP open %s success\r\n",step_motor);
	}

	memset(&step,0,sizeof(struct stepmotor_par));

	step.direction=atoi(argv[1]);
	step.num=atoi(argv[2]);
	step.speed=atoi(argv[3]);

   // printf("step.direction = %d \r\n",step.direction);
	//printf("step.num = %d \r\n",step.num);
	//printf("step.speed = %d \r\n",step.speed);


	if(argv[1][0]=='R')
	{
		step.direction=1;
		//printf("step.direction = %d \r\n",step.direction);

		step_motor_num(step.direction,step.num,step.speed);
		printf("APP enter R success\r\n");
	}
	

	if(argv[1][0]=='L')
	{
		step.direction=0;
		//printf("step.direction = %d \r\n",step.direction);
         step_motor_num(step.direction,step.num,step.speed);
		 printf("APP enter L success\r\n");
	}
	close(fd);
}


void step_motor_off(void)
{
	ioctl(fd,CMD_STEPMOTOR_A,LOW);
	ioctl(fd,CMD_STEPMOTOR_B,LOW);
	ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	ioctl(fd,CMD_STEPMOTOR_D,LOW); 
}


void step_turn(unsigned long a , unsigned long delay)
{
    DPRINTK("step_motor_start\n");
    switch(a)
    {
    	case 0:
		DPRINTK("step_motor_start A\n");

		ioctl(fd,CMD_STEPMOTOR_A,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_B,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_D,LOW); 
		usleep(delay);
		break;
		case 1:
		DPRINTK("step_motor_start B\n");
		ioctl(fd,CMD_STEPMOTOR_A,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_B,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_D,LOW); 
		usleep(delay);
		break;
		case 2:
		DPRINTK("step_motor_start C\n");
		ioctl(fd,CMD_STEPMOTOR_A,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_B,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_D,LOW); 
		usleep(delay);
		break;
		case 3:
		DPRINTK("step_motor_start D\n");
		ioctl(fd,CMD_STEPMOTOR_A,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_B,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_C,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_D,LOW); 
		usleep(delay);
		break;
		case 4:
		DPRINTK("step_motor_start E\n");
		ioctl(fd,CMD_STEPMOTOR_A,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_B,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_C,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_D,LOW); 
		usleep(delay);
		break;
		case 5:
	    DPRINTK("step_motor_start F\n");
	    ioctl(fd,CMD_STEPMOTOR_A,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_B,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_C,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_D,HIGHT);  
		usleep(delay);
		break;
		case 6:
		DPRINTK("step_motor_start G\n");
		ioctl(fd,CMD_STEPMOTOR_A,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_B,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_D,HIGHT); 
		usleep(delay);
		break;
		case 7:
		DPRINTK("step_motor_start H\n");
		ioctl(fd,CMD_STEPMOTOR_A,HIGHT); 
	    ioctl(fd,CMD_STEPMOTOR_B,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_C,LOW); 
	    ioctl(fd,CMD_STEPMOTOR_D,HIGHT); 
	    usleep(delay);
		DPRINTK("step_motor_start end\n");
		default:
		break;
    }
      	step_motor_off();	
}

void step_motor_num(char rl,unsigned long num,unsigned long speed){
	int i;
	char STEP;
	
	for(i=0;i<num;i++){	
		if(rl==1){ 
			STEP++;
			if(STEP>7)STEP=0;
		}else{
			if(STEP==0)STEP=8;
			STEP--;
		}
		printf("----%d\n",STEP);		
		step_turn(STEP,speed);
	}
}
