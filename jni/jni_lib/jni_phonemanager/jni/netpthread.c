#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <jni.h>
#include <android/log.h>

#include "sockserver.h"
#include "pthread_api.h"
#include "fileinfo.h"
#include "jni_api.h"

char SIG_START = 0x77;

char sig_start[0];
#define CMD_IMAGE  1
#define CMD_VIDEO  2
#define CMD_MUSIC  3
#define CMD_APP    4
#define CMD_MMS    5
#define CMD_BOOK   6
#define CMD_STORAGE 7
#define CMD_SENDMESSAGE 8
#define CMD_CALLPHONE	9
#define CMD_ALL			10

char data[1024*200];
char getbuf[1024 * 2] = {0};

int SIGN = 0;
int SIZE = 0;
int SEND_CMD = -1;
char RECV_CMD = 0;
int i;
char  recv_buf = 0;
int REGET  = 0;			
int recv_leng = 0;

struct timeval tout;

static pthread_mutex_t net_mutex;

typedef struct __sockthread{
	void *managerp;
	pthread_t tid;
	pthread_attr_t tattr;
}sockthread_t;

int sockfd,new_fd;
static sockthread_t sockthread;
static sockthread_t sendthread;

pthread_mutex_t mutex;
pthread_cond_t  netsendcond;
sem_t reget[10]; 

char getdata[1024*30];

void relese_cond_signal()
{
	//	pthread_cond_signal(&netsendcond);
}

int seminit()
{
	for(i = 0; i < 2; i++)
	{
		sem_init(&reget[i],0,0);
	}
}

send_pack()
{
	int ret = 0;

	switch(SEND_CMD)
	{
		case CMD_IMAGE:
			send_sigstart();
			SIZE = sizeof(media_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&imageinfo,sizeof(imageinfo),0);
			SIZE = 0;
			SEND_CMD = -1;
			break;
		case CMD_APP:
			send_sigstart();
			SEND_CMD = CMD_APP;
			ret = send(new_fd,&SEND_CMD,1,0);

			SIZE = sizeof(allappinfo_t);
			LOGD("SIZE is %d",SIZE);
			ret = send(new_fd,&SIZE,4,0);

			ret = send(new_fd,&appinfo,sizeof(appinfo),0);
			SIZE = 0;
			SEND_CMD = -1;
			break;
		case CMD_BOOK:
			send_sigstart();
			SEND_CMD = CMD_BOOK;
			ret = send(new_fd,&SEND_CMD,1,0);
			SIZE = sizeof(allbookinfo_t);
			LOGD("SIZE is %d",SIZE);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&bookinfo,SIZE,0);
			SIZE = 0;
			SEND_CMD = -1;
			break;
		case CMD_MMS:
			send_sigstart();
			SEND_CMD = CMD_MMS;
			ret = send(new_fd,&SEND_CMD,1,0);
			SIZE = sizeof(allmmsinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&mmsinfo,SIZE,0);
			SIZE = 0;
			SEND_CMD = -1;
			break;

		case CMD_STORAGE:

			LOGD("CMD_STORAGE: send");
			send_sigstart();
			SEND_CMD = CMD_STORAGE;
			ret = send(new_fd,&SEND_CMD,1,0);

			SIZE = sizeof(storageinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&storageinfo,SIZE,0);
			SIZE = 0;
			SEND_CMD = -1;
			break;
		case CMD_CALLPHONE:

			LOGD("CMD_CALLPHONE: send");
			send_sigstart();
			SEND_CMD = CMD_CALLPHONE;
			ret = send(new_fd,&SEND_CMD,1,0);
			SIZE = 0;
			SEND_CMD = -1;
			break;

		case CMD_ALL:

			LOGD("CMD_SENDMESSAGE: send");
			send_sigstart();
			SEND_CMD = CMD_ALL;
			ret = send(new_fd,&SEND_CMD,1,0);

			SIZE = sizeof(storageinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&storageinfo,SIZE,0);		// send storageinfo

			SIZE = sizeof(allmmsinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&mmsinfo,SIZE,0);			// send mmsifo

			SIZE = sizeof(allbookinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&bookinfo,SIZE,0);		// send bookinfo

			SIZE = sizeof(allappinfo_t);
			ret = send(new_fd,&SIZE,4,0);
			ret = send(new_fd,&appinfo,sizeof(appinfo),0);		// send appinfo

			SIZE = 0;
			SEND_CMD = -1;
			break;

		case CMD_SENDMESSAGE:
			LOGD("CMD_SENDMESSAGE: send");
			send_sigstart();
			SEND_CMD = CMD_SENDMESSAGE;
			ret = send(new_fd,&SEND_CMD,1,0);

			SIZE = 1;
			ret = send(new_fd,&SIZE,4,0);
			SIZE = 0;
			SEND_CMD = -1;
			break;
		default :
			break;

	}
}

recv_pack()
{
	int ret = 0;
	char testbuf[4];

	tout.tv_sec = 3;
	tout.tv_usec = 0;
	for(i = 0; i < 4; i++)
	{
		dtcmr_recv_timeout(sockfd, &recv_buf, 1,&tout);
		LOGD("recv_buf= %x",recv_buf);
		if(recv_buf == SIG_START){
			recv_buf = 0;
		}else{
			i = -1;
			recv_buf = 0;
			sleep(1);
		}
	}

	dtcmr_recv_timeout(sockfd, &RECV_CMD, 1,&tout);
	LOGD("RECV_CMD  = %x",RECV_CMD);
	switch(RECV_CMD)
	{
		case CMD_IMAGE:
			memset(&imageinfo, 0, sizeof(media_t));
			REGET = CMD_IMAGE;
			LOGD("get RECV_CMD CMD_IMAGE");
			RECV_CMD = 0;
			break;

		case CMD_APP:
			memset(&appinfo, 0, sizeof(allappinfo_t));
			REGET = CMD_APP;
			LOGD("get RECV_CMD CMD_APP");
			RECV_CMD = 0;
			break;
		case CMD_MMS:
			memset(&mmsinfo, 0, sizeof(allmmsinfo_t));
			REGET = CMD_MMS;
			LOGD("get RECV_CMD CMD_MMS");
			RECV_CMD = 0;
			break;

		case CMD_BOOK:
			memset(&bookinfo, 0, sizeof(allbookinfo_t));
			REGET = CMD_BOOK;
			LOGD("get RECV_CMD CMD_BOOK");
			RECV_CMD = 0;
			break;

		case CMD_CALLPHONE:

			ret = dtcmr_recv_timeout(sockfd, testbuf, 4,&tout);
			memcpy(&recv_leng,testbuf,4);
			LOGD("recv_leng = %d",recv_leng);

			if(ret < 0){
				printf("recv error");
			}
			memset(getbuf,0, 2048);

			ret = dtcmr_recv_timeout(sockfd, getbuf, recv_leng,&tout);
			memcpy(callphone,getbuf,18);
			REGET = CMD_CALLPHONE;
			LOGD("get RECV_CMD CMD_CALLPHONE");
			RECV_CMD = 0;
			break;

		case CMD_SENDMESSAGE :
			LOGD("get RECV_CMD CMD_SENDMESSAGE");

			ret = dtcmr_recv_timeout(sockfd, testbuf, 4,&tout);
			memcpy(&recv_leng,testbuf,4);
			LOGD("recv_leng = %d",recv_leng);

			if(ret < 0){
				printf("recv error");
			}
			memset(getbuf,0, 2048);

			ret = dtcmr_recv_timeout(sockfd, getbuf, recv_leng,&tout);
			sendmessage_t *mmstmpinfo=(sendmessage_t *)getbuf;
			memcpy(sendmessage.mmsnumber,mmstmpinfo->mmsnumber,18);
			memcpy(sendmessage.mmsdata,mmstmpinfo->mmsdata,1024);

			LOGD("sendmessage.mmsnumbe,sendmessage.mmsdata  %s,%s",sendmessage.mmsnumber,sendmessage.mmsdata);
			REGET = CMD_SENDMESSAGE;

			RECV_CMD = 0;
			break;


		default:
			LOGD("default :get RECV_CMD %d",RECV_CMD);
			break;
	}
}

void* sock_pthread(void* arg) 
{
	LOGD("Jni_startservice");	
	sockfd = dtcmr_serv_creat(10086, 8, IPPROTO_TCP);
	if (sockfd<0){ 
		LOGD("dtcmr_serv_creat is error errorno = %d",sockfd);	
	}

	new_fd = dtcmr_accept(sockfd);
	if(new_fd < 0)
	{
		LOGE("accept error");
	}else{
		LOGD("recv accept ok");
	}
	//	SEND_CMD = CMD_STORAGE;
	seminit();
	send_pthread_init();

	while(1)
	{

		usleep(20000);
		pthread_mutex_lock(&net_mutex);
		recv_pack();
		pthread_mutex_unlock(&net_mutex);
		usleep(10000);
	}
}

void send_sigstart()
{
	int ret = 0;

	ret = send(new_fd,&SIG_START,1,0);
	ret = send(new_fd,&SIG_START,1,0);
	ret = send(new_fd,&SIG_START,1,0);
	ret = send(new_fd,&SIG_START,1,0);

	LOGD("int size = %d",sizeof(int));
	LOGD("char size = %d",sizeof(char));
}


void* send_pthread(void* arg) 
{
	//	send_pack();
	REGET = CMD_ALL;			// init send allinfo info;
	while(1)
	{
		pthread_mutex_lock(&net_mutex);
		LOGD("sem_wait(&reget[1]");
		sem_wait(&reget[1]);
		send_pack();
		pthread_mutex_unlock(&net_mutex);
		usleep(5000);

	}
}

int send_pthread_init()
{
	if (!pthread_attr_init(&sendthread.tattr)) {
		if (!pthread_attr_setdetachstate(&sendthread.tattr, PTHREAD_CREATE_DETACHED)) {
			pthread_create(&sendthread.tid, &sendthread.tattr, send_pthread, NULL);
			pthread_attr_destroy(&sendthread.tattr);
		} else {
			return -1;
		}	
	} else {
		return -1;
	}

	return 0;
}


//int sock_pthread_init()
jint Java_com_acanoe_appmanager_Appmanager_jnipthreadinit
(JNIEnv *env, jclass thiz)
{

	pthread_cond_init(&netsendcond,NULL);
	pthread_mutex_init(&mutex,NULL);

	if (!pthread_attr_init(&sockthread.tattr)) {
		if (!pthread_attr_setdetachstate(&sockthread.tattr, PTHREAD_CREATE_DETACHED)) {
			pthread_create(&sockthread.tid, &sockthread.tattr, sock_pthread, NULL);
			pthread_attr_destroy(&sockthread.tattr);
		} else {
			return -1;
		}	
	} else {
		return -1;
	}

	return 0;
}



void net_pthread_exit()
{
	//	pthread_cancel(sockthread.tid);
}
void close_socket()
{
	close(sockfd);
	close(new_fd);
	pthread_exit(NULL);
}





	jint Java_com_acanoe_appmanager_Appmanager_gotosend
(JNIEnv *env, jclass thiz, jint senditem)
{
	SEND_CMD = senditem;
	LOGD("gotosend resend %d",senditem);
	sem_post(&reget[1]);
}


	jstring Java_com_acanoe_appmanager_Appmanager_getmmsdata
(JNIEnv *env, jclass thiz)
{
	return (*env)->NewStringUTF(env,sendmessage.mmsdata);

}

	jstring Java_com_acanoe_appmanager_Appmanager_getphonenumber
(JNIEnv *env, jclass thiz)
{

	return (*env)->NewStringUTF(env,callphone);
}

	jstring Java_com_acanoe_appmanager_Appmanager_getmmsnumber
(JNIEnv *env, jclass thiz)
{

	return (*env)->NewStringUTF(env,sendmessage.mmsnumber);
}

	jint Java_com_acanoe_appmanager_Appmanager_whatyouwant
(JNIEnv *env, jclass thiz)
{
	int tmpreturn  = 0;
	tmpreturn  = REGET;
	REGET = 0;
	return tmpreturn;
}
