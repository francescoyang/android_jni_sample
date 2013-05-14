#include <stdio.h>
#include <jni.h>
#include "pthread_api.h"
#include "loginfo.h"

	void Java_com_acanoe_appmanager_Appmanager_imageinfosend
(JNIEnv *env, jclass thiz)
{
	LOGE("relese_cond_signal");
	relese_cond_signal();
}
