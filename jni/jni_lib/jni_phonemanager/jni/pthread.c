
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <jni.h>
#include <android/log.h>

static const char *TAG="Acanoe";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

pthread_mutex_t mutex;
pthread_cond_t  c1cond;
pthread_cond_t  c2cond;
pthread_cond_t  c3cond;
pthread_cond_t  c4cond;
pthread_cond_t  c5cond;
pthread_cond_t  c6cond;
pthread_cond_t  c7cond;
pthread_cond_t  c8cond;
pthread_cond_t  c9cond;
pthread_cond_t  c10cond;
int times = 0;

void * child1(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c1cond,&mutex);
		LOGD("thread 1 condition applied  thread1 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}
void *child2(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c2cond,&mutex);
		LOGD("thread 2 condition applied  thread2 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child3(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c3cond,&mutex);
		LOGD("thread 3 condition applied  thread3 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child4(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c4cond,&mutex);
		LOGD("thread 4 condition applied  thread4 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child5(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c5cond,&mutex);
		LOGD("thread 5 condition applied  thread5 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child6(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c6cond,&mutex);
		LOGD("thread 6 condition applied  thread6 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child7(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c7cond,&mutex);
		LOGD("thread 7 condition applied  thread7 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child8(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c8cond,&mutex);
		LOGD("thread 8 condition applied  thread8 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}

void *child9(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c9cond,&mutex);
		LOGD("thread 9 condition applied  thread9 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}


void *child10(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&c10cond,&mutex);
		LOGD("thread 10 condition applied  thread10 times is %d\n",times++);
		pthread_mutex_unlock(&mutex);
	}
}


//int main(void)

	jint Java_com_example_pthread_Ptrhead_createpthread
(JNIEnv *env, jclass thiz)
{
	LOGD("start create pthread ");
	pthread_t tid1,tid2, tid3, tid4, tid5, tid6, tid7, tid8, tid9, tid10;
	pthread_mutex_init(&mutex,NULL);

	pthread_cond_init(&c1cond,NULL);
	pthread_cond_init(&c2cond,NULL);
	pthread_cond_init(&c3cond,NULL);
	pthread_cond_init(&c4cond,NULL);
	pthread_cond_init(&c5cond,NULL);
	pthread_cond_init(&c6cond,NULL);
	pthread_cond_init(&c7cond,NULL);
	pthread_cond_init(&c8cond,NULL);
	pthread_cond_init(&c9cond,NULL);
	pthread_cond_init(&c10cond,NULL);

	pthread_create(&tid1,NULL,child1,NULL);
	pthread_create(&tid2,NULL,child2,NULL);
	pthread_create(&tid3,NULL,child3,NULL);
	pthread_create(&tid4,NULL,child4,NULL);
	pthread_create(&tid5,NULL,child5,NULL);
	pthread_create(&tid6,NULL,child6,NULL);
	pthread_create(&tid7,NULL,child7,NULL);
	pthread_create(&tid8,NULL,child8,NULL);
	pthread_create(&tid8,NULL,child9,NULL);
	pthread_create(&tid10,NULL,child10,NULL);
}

	jint Java_com_example_pthread_Ptrhead_runpthread
(JNIEnv *env, jclass thiz, jint pthread_no)
{
	switch(pthread_no) {
		case 1:
			pthread_cond_signal(&c1cond);
			sleep(1);                /* comment 6 */
			break;
		case 2:
			pthread_cond_signal(&c2cond);
			sleep(1);                /* comment 6 */
			break;
		case 3:
			pthread_cond_signal(&c3cond);
			sleep(1);                /* comment 6 */
			break;
		case 4:
			pthread_cond_signal(&c4cond);
			sleep(1);                /* comment 6 */
			break;
		case 5:
			pthread_cond_signal(&c5cond);
			sleep(1);                /* comment 6 */
			break;
		case 6:
			pthread_cond_signal(&c6cond);
			sleep(1);                /* comment 6 */
			break;
		case 7:
			pthread_cond_signal(&c7cond);
			sleep(1);                /* comment 6 */
			break;
		case 8:
			pthread_cond_signal(&c8cond);
			sleep(1);                /* comment 6 */
			break;
		case 9:
			pthread_cond_signal(&c9cond);
			sleep(1);                /* comment 6 */
			break;
		case 10:
			pthread_cond_signal(&c10cond);
			sleep(1);                /* comment 6 */
			break;
		default :
			break;
	}
}

	jint Java_com_example_pthread_Ptrhead_exitpthread
(JNIEnv *env, jclass thiz)
{
	pthread_exit(0);
}
