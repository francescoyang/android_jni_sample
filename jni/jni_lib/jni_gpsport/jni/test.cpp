#include "testport.h"
#include "otherport.h"
#include <stdio.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

extern "C" {
 jint Java_com_example_pthread_Ptrhead_cjjtest (JNIEnv *env, jclass thiz);
 jint Java_com_example_pthread_Ptrhead_othertest (JNIEnv *env, jclass thiz);
};


jint Java_com_example_pthread_Ptrhead_cjjtest
(JNIEnv *env, jclass thiz)
{
 LOGD("cjjtest start");
 TestPort test;
 test.InitPort();
 pthread_mutex_lock(&lntmutex);
 pthread_cond_wait(&tntcond, &lntmutex);
 pthread_mutex_unlock(&lntmutex);
 LOGD("got the signal.\n");
}
jint Java_com_example_pthread_Ptrhead_othertest (JNIEnv *env, jclass thiz)
{
 LOGD("othertest start");
 Otherport othertest;
 othertest.InitPort();
 pthread_mutex_lock(&ntmutex);
 pthread_cond_wait(&ntcond, &ntmutex);
 pthread_mutex_unlock(&ntmutex);
 LOGD("get other over");

}
