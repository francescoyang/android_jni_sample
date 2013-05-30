#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <jni.h>
#include <android/log.h>

static const char *TAG="Acanoe";
int readcount= 0;
int readleng = 0;


pthread_mutex_t mutex;
pthread_cond_t  c1cond;
pthread_cond_t  c2cond;
pthread_cond_t  c3cond;

int times = 0, ret = 0;
int fd1 = 0, fd2 = 0, fd3 = 0;
int time1 = 0,time2 = 0,time3= 0;
char read_buf[13];

unsigned  char checksum(char *data, int len)
{
	unsigned short sum = 0;
	int i;

	for (i = 0; i < len; i++) {
		sum ^= data[i];
	}

	return (sum & 0xff);
}

int open_port(char * dev)
{
	int fd;
	int fcntl_ret;

	fd = open(dev, O_RDWR | O_NOCTTY);
	if (-1 == fd)
		return -1;
	fcntl_ret = fcntl (fd, F_SETFL, 0);
	if (fcntl_ret < 0)
		return -2;
	return fd;
}


int init_port(int fd,int baud_rate)
{
	struct termios option;

	//------------------ 获得当前配置选项 ------------------
	tcgetattr( fd,&option );    

#if 1
	//------------------ 设置为本地模式并使能接受 -----------------
	option.c_cflag |= (CLOCAL | CREAD);

	//----------------- 设置数据位位数为 8 ------------------ 
	option.c_cflag &= ~CSIZE; 
	option.c_cflag |= CS8;

	//------------------ 设置为无奇偶校验 ------------------       
	option.c_cflag &= ~PARENB;

	//------------------ 设置停止位为1 -----------------
	option.c_cflag &= ~CSTOPB;

	//------------------- 设置为无软件/硬件控制流 ------------------
	option.c_cflag &= ~(CRTSCTS);

	option.c_iflag &= ~(IXON | INLCR | ICRNL | IGNCR | IUCLC);

	//------------------ 设置输出模式为非预处理 ------------------
	option.c_oflag &= ~OPOST;

	//------------------ 设置输入为非格式化输入 ------------------
	option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


	//------------------ 所读最小字符数 ------------------
	option.c_cc[VMIN] = 1;

	//------------------ 等待数据时间 -----------------
	option.c_cc[VTIME] = 10;

	option.c_cc[VINTR] = 0;  
	option.c_cc[VQUIT] = 0;  
	option.c_cc[VERASE] = 0;  
	option.c_cc[VKILL] = 0;  
	option.c_cc[VEOF] = 0;  
	option.c_cc[VTIME] = 10;  
	option.c_cc[VMIN] = 1;  
	option.c_cc[VSWTC] = 0;  
	option.c_cc[VSTART] = 0;  
	option.c_cc[VSTOP] = 0;  
	option.c_cc[VSUSP] = 0;  
	option.c_cc[VEOL] = 0;  
	option.c_cc[VREPRINT] = 0;  
	option.c_cc[VDISCARD] = 0;  
	option.c_cc[VWERASE] = 0;  
	option.c_cc[VLNEXT] = 0;  
	option.c_cc[VEOL2] = 0;  


#endif

	//------------------ 设置波特率 ------------------  
	switch(baud_rate)
	{
		case 2400:
			cfsetispeed(&option, B2400);
			cfsetospeed(&option, B2400);
			break;
		case 4800:
			cfsetispeed(&option, B4800);
			cfsetospeed(&option, B4800);
			break;
		case 9600:
			cfsetispeed(&option, B9600);
			cfsetospeed(&option, B9600);
			break;
		case 115200:
			cfsetispeed(&option, B115200);
			cfsetospeed(&option, B115200);
			break;
		default:
			cfsetispeed(&option, B9600);
			cfsetospeed(&option, B9600);
			break;
	}


	tcflush(fd, TCIOFLUSH);
	//------------------ 立即保存以上配置 -----------------    
	if((tcsetattr (fd,TCSANOW,&option)) != 0)
	{
		return -1;
	}

	return 0;
}

int main(void)
{
	printf("start read uart\n");
	fd1 = open_port("/dev/ttySAC0");

	if (fd1 < 0) 
	{
		printf("open error\n");
		return -1;
	}

	if (init_port(fd1, 9600) < 0)
	{
		printf("set baudrate error");
		return -2;
	}

	int i = 0; 
	char check = 0;

	while(1)
	{
		read(fd1,&read_buf[0], 1);
		printf("%x\n",read_buf[0]);
	}
}
