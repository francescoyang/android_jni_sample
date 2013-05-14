#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#include<jni.h>
#include<android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

//线程数
#define NUMTHREADS 5

//全局变量
JavaVM *g_jvm = NULL;
jobject g_obj = NULL;


void *thread_fun(void* arg)
{
	JNIEnv *env;
	jclass cls;
	jmethodID mid;

	//Attach主线程
	if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK)
	{
		LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
		return NULL;
	}
	//找到对应的类
	cls = (*env)->GetObjectClass(env,g_obj);
	if(cls == NULL)
	{
		LOGE("FindClass() Error.....");
		goto error; 
	}
	//再获得类中的方法
	mid = (*env)->GetStaticMethodID(env, cls, "fromJNI", "(I)V");
	if (mid == NULL) 
	{
		LOGE("GetMethodID() Error.....");
		goto error;  
	}
	//最后调用java中的静态方法
	(*env)->CallStaticVoidMethod(env, cls, mid ,(int)arg);


error:    
	//Detach主线程
	if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK)
	{
		LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
	}


	pthread_exit(0);
}

//由java调用以创建子线程
JNIEXPORT void Java_com_nan_thread_MyThreadActivity_mainThread( JNIEnv* env, jobject obj)
{
	int i;
	pthread_t pt[NUMTHREADS];

	for (i = 0; i < NUMTHREADS; i++)
		//创建子线程
		pthread_create(&pt[i], NULL, &thread_fun, (void *)i);
}


//由java调用来建立JNI环境
JNIEXPORT void Java_com_nan_thread_MyThreadActivity_setJNIEnv( JNIEnv* env, jobject obj)
{
	//保存全局JVM以便在子线程中使用
	(*env)->GetJavaVM(env,&g_jvm);
	//不能直接赋值(g_obj = obj)
	g_obj = (*env)->NewGlobalRef(env,obj);
}


//当动态库被加载时这个函数被系统调用
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;    

	//获取JNI版本
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK) 
	{
		LOGE("GetEnv failed!");
		return result;
	}

	return JNI_VERSION_1_4;
}
