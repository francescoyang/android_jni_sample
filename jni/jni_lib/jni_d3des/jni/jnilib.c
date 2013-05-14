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

#define OPEN_DEV 	-1
#define SET_BAU 	-2
#define ICCARD 		1
#define FINGER 		2 
#define GPS 		3
typedef struct tag_encrypteddata{
	char head[24];
	char data[1024*30];
}encrypteddata;


jstring strToJstring(JNIEnv* env, const char* pStr)
{
	int        strLen    = strlen(pStr);
	jclass     jstrObj   = (*env)->FindClass(env, "java/lang/String");
	jmethodID  methodId  = (*env)->GetMethodID(env, jstrObj, "<init>", "([BLjava/lang/String;)V");
	jbyteArray byteArray = (*env)->NewByteArray(env, strLen);
	jstring    encode   =   (*env)->NewStringUTF(env,"GB2312");

	(*env)->SetByteArrayRegion(env, byteArray, 0, strLen, (jbyte*)pStr);

	return (jstring)(*env)->NewObject(env, jstrObj, methodId, byteArray, encode);
}



char*   Jstring2CStr(JNIEnv*   env,   jstring   jstr)
{
	char*   rtn   =   NULL;
	jclass   clsstring   =   (*env)->FindClass(env,"java/lang/String");
	jstring   strencode   =   (*env)->NewStringUTF(env,"GB2312");
	jmethodID   mid   =   (*env)->GetMethodID(env,clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
	jbyteArray   barr=   (jbyteArray)(*env)->CallObjectMethod(env,jstr,mid,strencode); // String .getByte("GB2312");
	jsize   alen   =   (*env)->GetArrayLength(env,barr);
	jbyte*   ba   =   (*env)->GetByteArrayElements(env,barr,JNI_FALSE);
	if(alen   >   0)
	{
		rtn   =   (char*)malloc(alen+1);         //new   char[alen+1]; "\0"
		memcpy(rtn,ba,alen);
		rtn[alen]=0; } (*env)->ReleaseByteArrayElements(env,barr,ba,0);  //¿¿¿¿ 
		return rtn;
}

	jint Java_com_example_jnitest_Jnilib_open
(JNIEnv *env, jclass thiz, jstring path, jint baudrate, jint flags)
{
	int fd;
	jboolean iscopy;
	const char *path_utf = (*env)->GetStringUTFChars(env, path, &iscopy);
	fd = open_port(path_utf, flags);
	if (fd <= 0) 
	{
		return OPEN_DEV;
	}
	if (init_port(fd, baudrate) < 0)
	{
		return SET_BAU;
	}

	(*env)->ReleaseStringUTFChars(env, path, path_utf);
	return fd;
}

	JNIEXPORT void JNICALL Java_com_example_jnitest_Jnilib_close
(JNIEnv *env, jobject thiz, jint fd)
{
	close(fd);
}



	jstring Java_com_example_jnitest_Jnilib_write
(JNIEnv *env, jobject thiz,jint fd,jbyteArray data,jint len)
{
	char *p = (*env)->GetByteArrayElements(env, data, 0);
	write(fd,p,len);
}


	jstring
Java_com_example_jnitest_Jnilib_readfile( JNIEnv* env,
		jobject thiz, jstring jstr,jint len)
{

	FILE * stream;
	char s[len];
	memset(s,0,len);
	int i;
	char* p =  Jstring2CStr(env,jstr);

	//	__android_log_print(ANDROID_LOG_INFO, "Acanoe", argv);

	//	stream = fopen("/mnt/sdcard/test.file","r");
	stream = fopen(p,"r");
	if(!stream)
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "fopen file error");

	fread(s,len,1,stream);
	fclose(stream);

	s[ len - 1] = '\0';

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Java -- > C JNI :read str = %s",s);
	usleep(500);
	//	free(s);
	return (*env)->NewStringUTF(env,s);
}

JNIEXPORT jbyteArray JNICALL Java_com_example_jnitest_Jnilib_read( JNIEnv* env,jobject thiz ,jint dev, jint fds, jint len)
{
	switch (dev){
		case ICCARD:
			return readiccard(fds);
		case FINGER:
			return readfinger(fds,len);	
		case GPS:
			return readgps(fds,len);	
		default:
			break;
	}
}

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
jstring  Java_com_example_jnitest_Jnilib_d3dtest( JNIEnv* env,jobject thiz, jbyteArray icid) 
{

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Encrypt");
	char d3ddata[100];
	memset(d3ddata, 0,100);
	char *data = (*env)->GetByteArrayElements(env, icid, 0);

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "The source data is %s",data);
	DESede_Encrypt(d3ddata,data,6,"Itc_Ymtk703_YmtkItc_Ymtk");

	char* tmp = D3dDes_Decrypt(d3ddata,8,"Itc_Ymtk703_YmtkItc_Ymtk");
	if(!tmp){
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Java -- > C JNI :d3d error");
	}else {
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Get data is %s",tmp);
	}
	return (*env)->NewStringUTF(env,tmp);
}

jbyteArray  Java_com_example_jnitest_Jnilib_ItcEncrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len) 
{

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Encrypt");
	char d3ddata[100];
	char *data = (*env)->GetByteArrayElements(env, icid, 0);

	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "The source data is %s",data);
	DESede_Encrypt(d3ddata,data,6,"Itc_Ymtk703_YmtkItc_Ymtk");
	jbyteArray rtnbytes = (*env)->NewByteArray(env,(jsize)strlen(d3ddata)); 
	(*env)->SetByteArrayRegion(env,rtnbytes,0,(jsize)strlen(d3ddata),(jbyte*)d3ddata); 
	return rtnbytes;
}

jstring  Java_com_example_jnitest_Jnilib_ItcDecrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len)
{
	__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Dectypt");
	char *d3ddata = (*env)->GetByteArrayElements(env, icid, 0);


	char* tmp = D3dDes_Decrypt(d3ddata,8,"Itc_Ymtk703_YmtkItc_Ymtk");
	if(!tmp){
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Java -- > C JNI :d3d error");
	}else {
		__android_log_print(ANDROID_LOG_INFO, "Acanoe", "Get data is %s",tmp);
	}
	return (*env)->NewStringUTF(env,tmp);
}
