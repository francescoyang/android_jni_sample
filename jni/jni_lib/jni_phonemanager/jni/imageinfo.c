#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jni.h>
#include <android/log.h>
#include <string.h>
#include "fileinfo.h"

	jint Java_com_acanoe_appmanager_Appmanager_setimageinfo
(JNIEnv *env, jclass thiz,jstring name,jstring path, jstring size,jint count)
//(JNIEnv *env, jclass thiz,jstring name,jstring path, jstring size,jbyteArray imagebyte,jint count)
{

	int len_name;
	int len_size;
	int len_path;

	jboolean iscopy;

	const char *image_name = (*env)->GetStringUTFChars(env, name, &iscopy);
	const char *image_path = (*env)->GetStringUTFChars(env, path, &iscopy);
	const char *image_size = (*env)->GetStringUTFChars(env, size, &iscopy);

	if(!image_name || !image_path || !image_size)
	{
		return ERROR_INFO;
	}
	len_name = strlen(image_name);
	len_size = strlen(image_size);
	len_path = strlen(image_path);


	if(len_name > MAXNAME&& len_size > MAXSIZE && len_path > MAXPATH)
	{
		return ERROR_LEN;
	}

	if(count > MAXNU)
	{
		return ERROR_MAXNU;
	}

	/*
	jsize len = (*env)->GetArrayLength(env, imagebyte);
	char *data = (*env)->GetByteArrayElements(env, imagebyte, 0);
	memcpy(imageinfo.get_info[count].imagedyte,&data,len);
	*/

	imageinfo.count = count;

	memcpy(imageinfo.get_info[count].filename,image_name,len_name);
	memcpy(imageinfo.get_info[count].filesize,image_size,len_size);
	memcpy(imageinfo.get_info[count].filepath,image_path,len_path);
}

void getimageinfo(char *data)
{
	int i;
	for(i = 0; i < imageinfo.count; i++)
	{
		memcpy(data,imageinfo.get_info[i].filename,strlen(imageinfo.get_info[i].filename));
//		LOGD("filename  is %s",imageinfo.get_info[i].filename);
		memcpy(data,imageinfo.get_info[i].filesize,strlen(imageinfo.get_info[i].filesize));
//		LOGD("filename  is %s",imageinfo.get_info[i].filesize);
		memcpy(data,imageinfo.get_info[i].filepath,strlen(imageinfo.get_info[i].filepath));
//		LOGD("filename  is %s",imageinfo.get_info[i].filepath);
	}
}
