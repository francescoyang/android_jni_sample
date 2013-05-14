#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jni.h>
#include <android/log.h>
#include <string.h>
#include "fileinfo.h"


	jint Java_com_acanoe_appmanager_Appmanager_setmusicinfo
(JNIEnv *env, jclass thiz,jstring name,jstring path, jstring size,jint count)
{

	int len_name;
	int len_size;
	int len_path;

	jboolean iscopy;

	const char *music_name = (*env)->GetStringUTFChars(env, name, &iscopy);
	const char *music_path = (*env)->GetStringUTFChars(env, path, &iscopy);
	const char *music_size = (*env)->GetStringUTFChars(env, size, &iscopy);

	if(!music_name || !music_path || !music_size)
	{
		return ERROR_INFO;
	}
	len_name = strlen(music_name);
	len_size = strlen(music_size);
	len_path = strlen(music_path);


	if(len_name > MAXNAME&& len_size > MAXSIZE && len_path > MAXPATH)
	{
		return ERROR_LEN;
	}

	if(count > MAXNU)
	{
		return ERROR_MAXNU;
	}

	LOGD("count is %d",count);

	musicinfo.count = count;

	memcpy(musicinfo.get_info[count].filename,music_name,len_name);
	LOGD("filename  is %s",musicinfo.get_info[count].filename);
	memcpy(musicinfo.get_info[count].filesize,music_size,len_size);
	LOGD("filesize  is %s",musicinfo.get_info[count].filesize);
	memcpy(musicinfo.get_info[count].filepath,music_path,len_path);
	LOGD("filepath  is %s",musicinfo.get_info[count].filepath);
}
