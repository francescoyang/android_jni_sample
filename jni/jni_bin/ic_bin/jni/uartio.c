#include    <stdio.h>      
#include    <string.h>        
#include    <sys/types.h>  
#include    <sys/stat.h>   
#include    <sys/ioctl.h>
#include    <errno.h>      
#include    <termios.h>   
#include    <fcntl.h>     
#include    <unistd.h>
#include    <stdlib.h>
#include    "uartio.h"

int open_port(int comport)
{
	int fd=0;
	if(comport == IC_DEV)
	{

		fd = open("/dev/s3c2410_serial2", O_RDWR | O_NDELAY);
		if(-1 == fd)
		{
			perror("can't open serial port");
			return -1;
		}
	}
	else if(comport == FINGE_DEV)
		 {
			fd = open("/dev/s3c2410_serial3", O_RDWR | O_NDELAY);
		   	if(-1 == fd)
			{
				perror("can't open serial port");
				return -1;
		    }
		 }
	 else
	 {
		return -1;
	 }  

	 if(fcntl (fd, F_SETFL, 0) < 0)
		 printf("fcntl failed !\n");
	 else
		 printf("fcntl = %d\n", fcntl(fd, F_SETFL, 0));

	 if(isatty(STDIN_FILENO) == 0)
		 printf("standard input is not a terminal device \n");
	 else
		 printf("isatty success !\n");
	 printf("fd_open = %d\n", fd);

	 return fd;
}





int set_opt(int fd, int nspeed, int nbits, char nevent, int nstop)
{
	struct termios newtio, oldtio;
	if(tcgetattr(fd, &oldtio) != 0)
	{
		perror("setup serial 1");
		return -1;
	}

	bzero(&newtio, sizeof(newtio));


	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;


	switch(nbits)
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}

	switch(nevent)
	{
		case 'O':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_cflag |= (INPCK | ISTRIP);
			break;
		case 'E':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			newtio.c_cflag |= (INPCK | ISTRIP);
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB; 
			break;
	}

	switch(nspeed)
	{
		case 2400:
			cfsetispeed(&newtio,B2400);
			cfsetospeed(&newtio,B2400);
			break;
		case 4800:
			cfsetispeed(&newtio,B4800);
			cfsetospeed(&newtio,B4800);
			break;
		case 9600:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);
			break;
		case 115200:
			cfsetispeed(&newtio,B115200);
			cfsetospeed(&newtio,B115200);
			break;
		case 460800:
			cfsetispeed(&newtio,B460800);
			cfsetospeed(&newtio,B460800);
			break;
		default:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);
			break;
	}

	if(nstop == 1) 
		newtio.c_cflag &= ~CSTOPB;
	else
		if(nstop == 2) 
			newtio.c_cflag |= CSTOPB;

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);


	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("can set error");
		return -1;
	}
	printf("set done !\n");

	return 0;
}

