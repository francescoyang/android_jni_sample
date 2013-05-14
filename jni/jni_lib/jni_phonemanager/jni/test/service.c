#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "sockserver.h"

int sockfd,new_fd;

int main()
{
	int times = 0;
	int sockfd;
	int i = 0;
	int fd;
	int ret;
	char  rdate[1024];
	char  sdate[1024];
	int rleng = 1;
	int *packetp, *sdp;
	struct timeval tout;
	int cmd = 0, did = 0,sign = 0, leng = 0;
//	(short )argv[2];
//	sockfd = jpnetsocket_client_init(argv[1], atoi(argv[2]), IPPROTO_TCP);
	printf("startservice start create serciesocket");
	sockfd = dtcmr_serv_creat(8081, 8, IPPROTO_TCP);
	if (sockfd<0){ 
		printf("dtcmr_client_creat is error\n");	
		return -1;
	}

	while(1)
	{
		new_fd = dtcmr_accept(sockfd);
		if(new_fd < 0)
		{
			continue;
		}else{
			break;
		}
	}

	printf("recv connect\n");
	while(1)
	{
		tout.tv_sec = 2;
		tout.tv_usec = 0;
		memset(rdate, 0, 1024);

		ret = dtcmr_recv_timeout(new_fd,rdate,1024,&tout);
		if (ret < 0)
		{
			printf("RECV ERROR\n");
			break;
		}
		printf("rdate is %s\n",rdate);

		sleep(2);

		ret = dtcmr_send(sockfd, sdate, 20);
		if (ret < 0)
		{
			printf("SEND ERROR\n");
			break;
		}

	}
}
