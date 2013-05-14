//#include <iostream>
#include <stdio.h>
#include "otherport.h"
#include <jni.h>
#include <android/log.h>

using namespace std;


static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
pthread_cond_t ntcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ntmutex = PTHREAD_MUTEX_INITIALIZER;

Otherport::Otherport(void){

}

Otherport::~Otherport(void){

}

void Otherport::InitPort()
{
	pthread_create(&ntid, NULL, thr_fn, NULL);
}

void* Otherport::thr_fn(void *arg)
{
	LOGD("otherport thread success.");
	pthread_mutex_lock(&ntmutex);
	pthread_cond_signal(&ntcond);
	pthread_mutex_unlock(&ntmutex);
}
