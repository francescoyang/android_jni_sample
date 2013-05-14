#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <termios.h>
#include <jni.h>
#include <android/log.h>


char* D3dDes_Decrypt(char *from, unsigned int len, char *key)
{
	int length = 0;
	char* tmp = (char*)malloc(len+8);
	length = DESede_Decrypt(tmp,from,len,key);
	if(length <= 0){
		free(tmp);
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "length <= 0");
		tmp = NULL;
		return NULL;
	}
	char* ret = (char*)malloc(length);
	memcpy(ret,tmp,length);
	free(tmp);
	tmp = NULL;
	return ret;
}

jbyteArray  Java_com_example_jnitest_Jnilib_ItcEncrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len) 
{

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Encrypt");
	char d3ddata[100];
	char *data = (*env)->GetByteArrayElements(env, icid, 0);

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "The source data is %s",data);
	DESede_Encrypt(d3ddata,data,len,"Itc_Ymtk703_YmtkItc_Ymtk");
	jbyteArray rtnbytes = (*env)->NewByteArray(env,(jsize)strlen(d3ddata)); 
	(*env)->SetByteArrayRegion(env,rtnbytes,0,(jsize)strlen(d3ddata),(jbyte*)d3ddata); 
	return rtnbytes;
}

jstring  Java_com_example_jnitest_Jnilib_ItcDecrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len)
{
	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Dectypt");
	char *d3ddata = (*env)->GetByteArrayElements(env, icid, 0);


	char* tmp = D3dDes_Decrypt(d3ddata,len,"Itc_Ymtk703_YmtkItc_Ymtk");
	if(!tmp){
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Java -- > C JNI :d3d error");
	}else {
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Get data is %s",tmp);
	}
	return (*env)->NewStringUTF(env,tmp);
}
