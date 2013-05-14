#include "sockserverlib_api.h"
#include "sockserver.h"
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc,char * argv[]) 
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
	sockfd = dtcmr_serv_creat(atoi(argv[1]), atoi(argv[2]), IPPROTO_TCP);
	if (sockfd<0){ 
		printf("dtcmr_client_creat is error\n");	
		return -1;
	}

	dtcmr_accept(sockfd);
	printf("recv connect\n");
	while(1)
	{
		tout.tv_sec = 2;
		tout.tv_usec = 0;
		memset(rdate, 0, 1024);

		ret = dtcmr_recv_timeout(sockfd,rdate,1024,&tout);
		if (ret < 0)
		{
			break;
		}
#if 1
		for(i=0;i<4;i++)
		{
			ret = dtcmr_recv_timeout(sockfd,rdate+i,1,&tout);
			if(ret < 0) {
				perror("recv :");
				return -1;	
			}
			if (rdate[i] != 0x7e) {
				i = -1;	
			}
		}

		rleng = 16;	
		ret = dtcmr_recv_timeout(sockfd,rdate+4,rleng,&tout);
		if (ret < 0) {
			printf("recv error, return\n");
			return 0;	
		}
		packetp  =(int *) rdate;
		printf("times = 0x%x,send date:0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",times++, packetp[0], packetp[1], packetp[2],packetp[3], packetp[4]);
		cmd = ntohl(packetp[1]);
		did = ntohl(packetp[2]);
		sign= ntohl(packetp[3]);
		leng = ntohl(packetp[4]);
		ret = dtcmr_recv_timeout(sockfd,rdate+20,leng,&tout);
		if (ret < 0) {
			printf("recv error, return\n");
			return 0;	
		}
		switch (cmd) {
		case 0:
			memcpy(sdate, rdate, 20);
			sdp = (int *)sdate;
			sdp[4] = 0;
			sdp[3] |= (0x1 << 31);
			ret = dtcmr_send(sockfd, sdate, 20);
			if (ret < 0) {
				printf("send error,\n");
				return 0;
			}
			printf("times = 0x%x,send date:0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",times++, sdp[0], sdp[1], sdp[2], sdp[3], sdp[4]);
			break;
		default:
			printf("recv not gps packet\n");
			break;
		}
#endif
	}
}
