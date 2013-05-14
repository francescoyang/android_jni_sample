#include "sockserver.h"
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include <jni.h>
#include <android/log.h>

static const char *TAG="JNI";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
int sockfd,new_fd;
pthread_t accthread,recthread;

typedef struct __netcmd{
	volatile enum netsyncsign sign;
	volatile enum netcmdval cmd;
	volatile char *sp;
	volatile char *rp;
	volatile int sleng;
	volatile int rleng;
	pthread_mutex_t  cmdmutex;
}netcmd_t;

void senddata()
{
	int ret;
	while(1)
	{
		ret = send(new_fd,"87654321",8,0);
		if (ret < 0)
		{
			LOGE("error send");
		}
		sleep(1);
	}
}

	void Java_com_acanoe_appmanager_Appmanager_startservice
(JNIEnv *env, jclass thiz)
{
	int times = 0;
	int i = 0;
	int fd;
	int ret;
	char  rdate[1024];
	char  sdate[1024];
	int rleng = 1;
	int *packetp, *sdp;
	struct timeval tout;
	int cmd = 0, did = 0,sign = 0, leng = 0;
	//	sockfd = dtcmr_serv_creat(atoi(argv[1]), atoi(argv[2]), IPPROTO_TCP);

	LOGD("Jni_startservice");	
	sockfd = dtcmr_serv_creat(10086, 8, IPPROTO_TCP);
	if (sockfd<0){ 
		LOGD("dtcmr_serv_creat is error");	
	}

	new_fd = dtcmr_accept(sockfd);
	if(new_fd < 0)
	{
		LOGE("accept error");
	}

	LOGD("recv connect");
	while(1)
	{
		/*
		tout.tv_sec = 2;
		tout.tv_usec = 0;
		memset(rdate, 0, 1024);
		if((pthread_create(&accthread,NULL,(void*)senddata,NULL))!=0){
			LOGE("create thread error!\n");
			exit(1);
		}

		sleep(2);

		*/
		ret = dtcmr_recv_timeout(sockfd,rdate,1024,&tout);
		if (ret < 0)
		{
			LOGE("error recv");
			break;
		}
		LOGD("recv data %s",rdate);

	}
}
