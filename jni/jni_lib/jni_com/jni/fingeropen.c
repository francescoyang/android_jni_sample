#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <termios.h>
#include <jni.h>
#include <android/log.h>


#define CHAR_BUFFER_1          0x01
#define CHAR_BUFFER_2          0x02
#define CHAR_BUFFER_3          0x03
#define CHAR_BUFFER_4		   0x04
#define MODEL_BUFFER           0x05
#define FINGERLIB_SUM  	       500
#define Continue_input_Finger_Num	 3

static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)



int fd;
int fd_485;
static int *body;


static int recvtimeout(int fd, char *buf, int leng, struct timeval *timeout)
{
	fd_set fd_read;
	int ret;
	int recved = 0;

	FD_ZERO(&fd_read);
	FD_SET(fd, &fd_read);
	ret = select(fd + 1, &fd_read, NULL, NULL, timeout);
	if (ret == -1) {
		return -1;
	} else if (ret){
		if(FD_ISSET(fd, &fd_read)) {
			recved = read(fd, buf, leng);
			if (recved <= 0)	{
				return -2;
			} else {
				return recved;
			}
		}
	} else if (ret == 0){
		return -1;
	}
	return -1;
}


jint  Java_com_example_jnitest_Jnilib_matchfinger( JNIEnv* env,jobject thiz, jintArray nums, jint fds)
{
	int i,ret;
	int timeout;
	int *p;
	int TempleteNum_ret[3];
	int Finger_Num =0;
	int Buffernum[4]={CHAR_BUFFER_1,CHAR_BUFFER_2,CHAR_BUFFER_3,CHAR_BUFFER_4};
	int code[1024] = {0};

	fd = fds;

	jsize len = (*env)->GetArrayLength(env, nums);
	LOGD("len = %d",len);
	if(len <= 0) {
		return NULL;
	}
	body = (*env)->GetIntArrayElements(env, nums, 0);

	while(1){
		ret=ZAZ_Match_Comparison(Buffernum[0],Buffernum[1],body);	
		if(ret!=0){
			LOGD("Match error");
			return -1;
		}else {
			LOGD("Match ok!");
			return 1;
		}
	}
}
jint  Java_com_example_jnitest_Jnilib_rematchfinger( JNIEnv* env,jobject thiz,jint fds)
{
	int i,ret;
	int timeout;
	int *p;
	int TempleteNum_ret[3];
	int Finger_Num =0;
	int Buffernum[4]={CHAR_BUFFER_1,CHAR_BUFFER_2,CHAR_BUFFER_3,CHAR_BUFFER_4};
	int code[1024] = {0};

	fd = fds;
	while(1){
		ret=ZAZ_Match_Comparison(Buffernum[0],Buffernum[1],body);	
		if(ret!=0){
			LOGD("Match error");
			return -1;
		}else {
			LOGD("Match ok!");
			return 1;
		}
	}
}

int getkey3()
{

	int read_data = 0;
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	if ( recvtimeout(fd, &read_data, 1,&timeout) < 0 )
	{
		return -1;
	} else {
		return read_data;
	}


	//	read(fd,&read_data,1);
}

SER_PutChar3(int a)
{
	write(fd, &a,1);
}
