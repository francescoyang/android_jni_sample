#include <jni.h>
#include <android/log.h>
#include "fileinfo.h"


	jint Java_com_acanoe_appmanager_Appmanager_exitprograme
(JNIEnv *env, jclass thiz)
{
	LOGD("exit programe");
	close_socket();
//	net_pthread_exit();
}
