#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include "fileinfo.h"

jint Java_com_acanoe_appmanager_Appmanager_setappinfo
(JNIEnv *env, jclass thiz, jint apppath ,jint appattribute,jstring appname,jstring packname, jstring appversion, jstring appsize,  jint count)
{
	int len_name;
	int len_packname;
	int len_version;
	int len_size;

	jboolean iscopy;

	const char *app_name= (*env)->GetStringUTFChars(env, appname, &iscopy);
	const char *app_packname= (*env)->GetStringUTFChars(env, packname, &iscopy);
	const char *app_version= (*env)->GetStringUTFChars(env, appversion, &iscopy);
	const char *app_size = (*env)->GetStringUTFChars(env, appsize, &iscopy);

	if(!app_name || !app_packname || !app_version)
	{
		return -1;
	}

	len_name = strlen(app_name);
	len_packname = strlen(app_packname);
	len_version = strlen(app_version);
	len_size = strlen(app_size);

	if((len_name> 50) || (len_packname> 100) || (len_version> 15) || (len_size >  16))
	{
		LOGE("appinfo len is error : %s,%s,%s",app_name,app_packname,app_version);
		LOGE("appinfo len is error : %d,%d,%d",len_name,len_packname,len_version);
		return -2;
	}
	
	appinfo.count = count;
	
	// 0 stand for this app install on phone , 1 stand for this app install on sdcard
	appinfo.get_info[count].apppath = apppath;				

	// 0 stand for this is a system app, 1 stand for this is a user app
	appinfo.get_info[count].appattribute = appattribute;

//	LOGD("appinfo.get_info[count].appattribute = %d",appinfo.get_info[count].appattribute);

	memcpy(appinfo.get_info[count].appname,app_name,len_name);
//	LOGD("appinfo.get_info[count].appname = %s",appinfo.get_info[count].appname);
	memcpy(appinfo.get_info[count].apppackname,app_packname,len_packname);
	memcpy(appinfo.get_info[count].appversion,app_version,len_version);
	memcpy(appinfo.get_info[count].appsize,app_size,len_size);
/*
		LOGD("%s",appinfo.get_info[count].appname);
		LOGD("%s",appinfo.get_info[count].apppackname);
		*/

	return 1;
}
