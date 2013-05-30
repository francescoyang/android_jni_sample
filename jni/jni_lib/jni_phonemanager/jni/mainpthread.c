#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>


static const char *TAG="JNI";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

typedef struct __mainthread{
	void *managerp;
	pthread_t tid;
	pthread_attr_t tattr;
}mainthread_t;

static mainthread_t mainthread;

void* main_pthread(void* arg) 
{
	sock_pthread_init();

	while ( 1 ) {
		usleep(500000);
	}
}
/*
jint Java_com_acanoe_appmanager_Appmanager_jnipthreadinit
(JNIEnv *env, jclass thiz)
{

	if (!pthread_attr_init(&mainthread.tattr)) {
		if (!pthread_attr_setdetachstate(&mainthread.tattr, PTHREAD_CREATE_DETACHED)) {
			pthread_create(&mainthread.tid, &mainthread.tattr, main_pthread, NULL);
			pthread_attr_destroy(&mainthread.tattr);
		} else {
			return -1;
		}	
	} else {
		return -1;
	}

	return 0;
}
*/
