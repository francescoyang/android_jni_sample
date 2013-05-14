//#include <iostream>
#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include "testport.h"

using namespace std;

static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

pthread_cond_t tntcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lntmutex = PTHREAD_MUTEX_INITIALIZER;

TestPort::TestPort(void){

}

TestPort::~TestPort(void){

}

void TestPort::InitPort()
{
	pthread_create(&ntid, NULL, thr_fn, NULL);
}

void* TestPort::thr_fn(void *arg)
{
	LOGD("testport pthread success.");
	pthread_mutex_lock(&lntmutex);
	pthread_cond_signal(&tntcond);
	pthread_mutex_unlock(&lntmutex);
}
