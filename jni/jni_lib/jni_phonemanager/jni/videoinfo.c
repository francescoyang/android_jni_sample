#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jni.h>
#include <android/log.h>
#include <string.h>
#include "fileinfo.h"


	jint Java_com_acanoe_appmanager_Appmanager_setvideoinfo
(JNIEnv *env, jclass thiz,jstring name,jstring path, jstring size,jint count)
{

	int len_name;
	int len_size;
	int len_path;

	jboolean iscopy;

	const char *video_name = (*env)->GetStringUTFChars(env, name, &iscopy);
	const char *video_path = (*env)->GetStringUTFChars(env, path, &iscopy);
	const char *video_size = (*env)->GetStringUTFChars(env, size, &iscopy);

	if(!video_name || !video_path || !video_size)
	{
		return ERROR_INFO;
	}
	len_name = strlen(video_name);
	len_size = strlen(video_size);
	len_path = strlen(video_path);


	if(len_name > MAXNAME&& len_size > MAXSIZE && len_path > MAXPATH)
	{
		return ERROR_LEN;
	}

	if(count > MAXNU)
	{
		return ERROR_MAXNU;
	}

	LOGD("count is %d",count);

	videoinfo.count = count;

	memcpy(videoinfo.get_info[count].filename,video_name,len_name);
	LOGD("filename  is %s",videoinfo.get_info[count].filename);
	memcpy(videoinfo.get_info[count].filesize,video_size,len_size);
	LOGD("filesize  is %s",videoinfo.get_info[count].filesize);
	memcpy(videoinfo.get_info[count].filepath,video_path,len_path);
	LOGD("filepath  is %s",videoinfo.get_info[count].filepath);
}
