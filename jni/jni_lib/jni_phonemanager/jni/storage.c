#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "fileinfo.h"

	jint Java_com_acanoe_appmanager_Appmanager_setstorageinfo
(JNIEnv *env, jclass thiz,jstring sdsize,jstring sdavail, jstring memsize, jstring memavail)
{
	jboolean iscopy;

	const char *sdSize	= (*env)->GetStringUTFChars(env, sdsize, &iscopy);
	const char *sdAvail	= (*env)->GetStringUTFChars(env, sdavail, &iscopy);
	const char *memorySize	= (*env)->GetStringUTFChars(env, memsize, &iscopy);
	const char *memoryAvail	= (*env)->GetStringUTFChars(env, memavail, &iscopy);

	memcpy(storageinfo.sdSize,sdSize,strlen(sdSize));
	memcpy(storageinfo.sdAvail,sdAvail,strlen(sdAvail));
	memcpy(storageinfo.memorySize,memorySize,strlen(memorySize));
	memcpy(storageinfo.memoryAvail,memoryAvail,strlen(memoryAvail));

	LOGD("%s,%s,%s,%s",sdSize,sdAvail,memorySize,memoryAvail);
}
