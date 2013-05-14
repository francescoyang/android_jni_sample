/*tcpserver.c  2011.9.1 by yyg*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>

#include <jni.h>
#include <android/log.h>

static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)



#define MYPORT 10086//定义端口
#define BACKLOG 10
#define MAXDATASIZE 1024

int sockfd,new_fd;
pthread_t accthread,recthread;

void recmessage(void){  //接收客户端信息函数
	while(1){
		int numbytes;
		char buf[MAXDATASIZE];
		if((numbytes = recv(new_fd,buf,MAXDATASIZE,0))==-1){
			perror("recv");
			exit(1);
		}
		buf[numbytes]='\0';
		if(strcmp(buf,"exit")==0){  //若收到的是exit字符，则代表退出通信
			printf("Client is closed\n");
			close(new_fd);
			close(sockfd);
			exit(1);
		}
		printf("client:%s\n",buf);
		sleep(2);
	}/*while*/
}

void acceptconnect(void){  //接受客户端连接请求函数
	struct sockaddr_in their_addr;
	int sin_size;
	sin_size = sizeof(struct sockaddr_in);
	if((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&sin_size))==-1){
		perror("accept");
		exit(1);
	}
	printf("server:got connection from %s\n",inet_ntoa(their_addr.sin_addr));
	/*创建子线程，用于接收信息*/
	if((pthread_create(&recthread,NULL,(void*)recmessage,NULL)) != 0){
		printf("create thread error!\r\n");
		exit(1);
	}
}

jint Java_com_acanoe_appmanager_Appmanager_s
//jint Java_com_acanoe_appmanager_Appmanager_startservice
(JNIEnv *env, jclass thiz)
{
	struct sockaddr_in my_addr;
	LOGI("jni startservice");
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("socket");
		exit(1);
	}
	/*初始化sockaddr_in结构体相关参数*/
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	/*绑定端口与套接字*/
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(1);
	}
	/*监听客户端套接字*/
	if(listen(sockfd,BACKLOG)== -1){
		perror("listen");
		exit(1);
	}
	printf("listening...\n");
	/*创建子线程，用于接收信息*/
	if((pthread_create(&accthread,NULL,(void*)acceptconnect,NULL))!=0){
		printf("create thread error!\n");
		exit(1);
	}
	while(1){
		char msg[MAXDATASIZE];
//		scanf("%s",msg);

		if(send(new_fd,"12345",strlen("12345"),0) == -1){  //发送信息，与客户端交流
			perror("send");
			exit(1);
		}
		if(strcmp(msg,"exit") ==0){
			printf("byebye\n");
			close(new_fd);
			close(sockfd);
			exit(1);

		}
		sleep(2);
	}/*while*/
	return 0;
}/*main*/

