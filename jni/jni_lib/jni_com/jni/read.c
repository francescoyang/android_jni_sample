/*
 * Copyright 2009-2011 Cedric Priscal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>

int fd;
write_buf(char a)
{
	write(fd, &a,1);
}


static speed_t getBaudrate(jint baudrate)
{
	switch(baudrate) {
		case 0: return B0;
		case 50: return B50;
		case 75: return B75;
		case 110: return B110;
		case 134: return B134;
		case 150: return B150;
		case 200: return B200;
		case 300: return B300;
		case 600: return B600;
		case 1200: return B1200;
		case 1800: return B1800;
		case 2400: return B2400;
		case 4800: return B4800;
		case 9600: return B9600;
		case 19200: return B19200;
		case 38400: return B38400;
		case 57600: return B57600;
		case 115200: return B115200;
		case 230400: return B230400;
		case 460800: return B460800;
		case 500000: return B500000;
		case 576000: return B576000;
		case 921600: return B921600;
		case 1000000: return B1000000;
		case 1152000: return B1152000;
		case 1500000: return B1500000;
		case 2000000: return B2000000;
		case 2500000: return B2500000;
		case 3000000: return B3000000;
		case 3500000: return B3500000;
		case 4000000: return B4000000;
		default: return -1;
	}
}

/*
 * Class:     android_serialport_SerialPort
 * Method:    open
 * Signature: (Ljava/lang/String;II)Ljava/io/FileDescriptor;
 */
main()
{
	int i;
	speed_t speed;

	/* Check arguments */
	{
		speed = getBaudrate(115200);
		if (speed == -1) {
			printf("set bud error\n");
		}
	}

	/* Opening device */
	{
		fd = open("/dev/s3c2410_serial3", O_RDWR | 0);
		if (fd == -1)
		{
			/* Throw an exception */
			/* TODO: throw an exception */
			printf("open error\n");
		}
	}

	/* Configure device */
	{
		struct termios cfg;
		if (tcgetattr(fd, &cfg))
		{
			close(fd);
		}

		cfmakeraw(&cfg);
		cfsetispeed(&cfg, speed);
		cfsetospeed(&cfg, speed);

		if (tcsetattr(fd, TCSANOW, &cfg))
		{
			close(fd);
		}
	}
	char no = 0x31;
	while(1){
///		PutChar3(no);
		for(i = 0; i < 4; i++){
			sleep(1);
			write_buf(no);
		}
		usleep(100000);
		tcflush(fd,TCIOFLUSH);
		usleep(20000);
		for(i = 0; i < 4; i++){
		getkey3();
		}
	}
}
int getkey3()
{
	int read_data = 0;
	int num;
	static int i;
//	char read_data[10];
//	memset(read_data, 0 ,10);
	num = read(fd,&read_data,1);
	i = i+num;
	printf("getkey3 recv read_data = %x, times %d\n",read_data, i);
}

PutChar3(int a)
{
	write(fd, &a,1);
	//	usleep(2000);
//	tcflush(fd,TCIOFLUSH);
	printf("write data = %x\n",a);
}
