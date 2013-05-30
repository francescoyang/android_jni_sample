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
#include <stdlib.h>
#include "md5.h"

#define OPEN_DEV 	-1
#define SET_BAU 	-2
#define ICCARD 		1
#define FINGER 		2 
#define GPS 		3
typedef struct tag_encrypteddata{
	char head[24];
	char data[1024*30];
}encrypteddata;

char databuf[13];
char data[13];

int i;
int readcount= 0;
int readleng = 0;



int open_port(char * dev)
{
	int fd;
	int fcntl_ret;

	//------------------ 以非控制台模式打开设备并忽略对端状态 -------------------
	fd = open(dev, O_RDWR | O_NOCTTY);
	if (-1 == fd)
		return -1;

	//------------------ 恢复打开端口为阻塞模式 ------------------
	fcntl_ret = fcntl (fd, F_SETFL, 0);
	// fcntl_ret = fcntl (fd, F_GETFL, 0);
	if (fcntl_ret < 0)
		return -2;

	return fd;
}

int init_port(int fd,int baud_rate)
{
	struct termios option;

	//------------------ 获得当前配置选项 ------------------
	tcgetattr( fd,&option );    

#if 1
	//------------------ 设置为本地模式并使能接受 -----------------
	option.c_cflag |= (CLOCAL | CREAD);

	//----------------- 设置数据位位数为 8 ------------------ 
	option.c_cflag &= ~CSIZE; 
	option.c_cflag |= CS8;

	//------------------ 设置为无奇偶校验 ------------------       
	option.c_cflag &= ~PARENB;

	//------------------ 设置停止位为1 -----------------
	option.c_cflag &= ~CSTOPB;

	//------------------- 设置为无软件/硬件控制流 ------------------
	option.c_cflag &= ~(CRTSCTS);

	option.c_iflag &= ~(IXON | INLCR | ICRNL | IGNCR | IUCLC);

	//------------------ 设置输出模式为非预处理 ------------------
	option.c_oflag &= ~OPOST;

	//------------------ 设置输入为非格式化输入 ------------------
	option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


	//------------------ 所读最小字符数 ------------------
	option.c_cc[VMIN] = 1;

	//------------------ 等待数据时间 -----------------
	option.c_cc[VTIME] = 10;

	option.c_cc[VINTR] = 0;  
	option.c_cc[VQUIT] = 0;  
	option.c_cc[VERASE] = 0;  
	option.c_cc[VKILL] = 0;  
	option.c_cc[VEOF] = 0;  
	option.c_cc[VTIME] = 10;  
	option.c_cc[VMIN] = 1;  
	option.c_cc[VSWTC] = 0;  
	option.c_cc[VSTART] = 0;  
	option.c_cc[VSTOP] = 0;  
	option.c_cc[VSUSP] = 0;  
	option.c_cc[VEOL] = 0;  
	option.c_cc[VREPRINT] = 0;  
	option.c_cc[VDISCARD] = 0;  
	option.c_cc[VWERASE] = 0;  
	option.c_cc[VLNEXT] = 0;  
	option.c_cc[VEOL2] = 0;  


#endif

	//------------------ 设置波特率 ------------------  
	switch(baud_rate)
	{
		case 2400:
			cfsetispeed(&option, B2400);
			cfsetospeed(&option, B2400);
			break;
		case 4800:
			cfsetispeed(&option, B4800);
			cfsetospeed(&option, B4800);
			break;
		case 9600:
			cfsetispeed(&option, B9600);
			cfsetospeed(&option, B9600);
			break;
		case 115200:
			cfsetispeed(&option, B115200);
			cfsetospeed(&option, B115200);
			break;
		default:
			cfsetispeed(&option, B9600);
			cfsetospeed(&option, B9600);
			break;
	}


	tcflush(fd, TCIOFLUSH);
	//------------------ 立即保存以上配置 -----------------    
	if((tcsetattr (fd,TCSANOW,&option)) != 0)
	{
		return -1;
	}

	return 0;
}


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
		rtn[alen]=0; } (*env)->ReleaseByteArrayElements(env,barr,ba,0);  //靠靠 
	return rtn;
}

	jint Java_com_example_jnitest_Jnilib_open
(JNIEnv *env, jclass thiz, jstring path, jint baudrate)
{
	int fd;
	jboolean iscopy;
	const char *path_utf = (*env)->GetStringUTFChars(env, path, &iscopy);
	fd = open_port(path_utf);
	__android_log_print(ANDROID_LOG_INFO, "Jni", "dev is %s, fd is %d", path_utf,fd);
	if (fd < 0) 
	{
		__android_log_print(ANDROID_LOG_INFO, "Jni", "open dev error");
		return OPEN_DEV;
	}
	if (init_port(fd, baudrate) < 0)
	{
		__android_log_print(ANDROID_LOG_INFO, "Jni", "set baudrate error");
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

	//	__android_log_print(ANDROID_LOG_INFO, "Jni", argv);

	//	stream = fopen("/mnt/sdcard/test.file","r");
	stream = fopen(p,"r");
	if(!stream)
		__android_log_print(ANDROID_LOG_INFO, "Jni", "fopen file error");

	fread(s,len,1,stream);
	fclose(stream);

	s[ len - 1] = '\0';

	__android_log_print(ANDROID_LOG_INFO, "Jni", "Java -- > C JNI :read str = %s",s);
	usleep(500);
	//	free(s);
	return (*env)->NewStringUTF(env,s);
}




jstring Java_com_example_jnitest_Jnilib_read( JNIEnv* env,jobject thiz ,jint fds,jint len)
{
	__android_log_print(ANDROID_LOG_INFO, "Jni", "Read");
	int times = 0;

	memset(databuf,0,13);
	memset(data,0,13);


	while(1)
	{
		times ++;
		for(i = 0; i < 13; i++)
		{
			readleng = read(fds,databuf + i, 1);
			__android_log_print(ANDROID_LOG_INFO, "Jni", "databuf %d ,%x",i,databuf[i]);
			if(databuf[i] == 0x88)
			{
				i = -1;
				__android_log_print(ANDROID_LOG_INFO, "Jni", "times-------------------------------- %d",times);
			}
		}

			/*
			   memset(databuf,0,13);

			   readleng = read(fds,&databuf[0], 1);

			   if(readleng >  0)
			   {
			   if(databuf[0] == 0x88)
			   {
			   for(i = 1; i < 13; i++)
			   {
			   readleng = read(fds,databuf + i, 1);
			   if(readleng <= 0)
			   {
			   __android_log_print(ANDROID_LOG_INFO, "Jni", "return NULL");
			   break;
			   }

			   }
			   } else {
			   __android_log_print(ANDROID_LOG_INFO, "Jni", "not get 0x88, get buf %x",databuf[0]);
			   continue;
			   }

			   } else {
			   __android_log_print(ANDROID_LOG_INFO, "Jni", "readleng error continue");
			   continue;
			   }

			   for(i = 0;i < len; i ++)
			   {
			   __android_log_print(ANDROID_LOG_INFO, "Jni", "The source data is %x",databuf[i]);
			   sprintf(data, "%s%02x",data,databuf[i]);
			   }
			   */

			//		return (*env)->NewStringUTF(env,data);
	}
}




jstring  Java_com_example_jnitest_Jnilib_readgps( JNIEnv* env,jobject thiz ,jint fds,jint len)
{
	return readgps(env,fds,len);
}

jstring  Java_com_example_jnitest_Jnilib_readiccard( JNIEnv* env,jobject thiz ,jint fds)
{
	return readiccard(env,fds);

}

char* D3dDes_Decrypt(char *from, unsigned int len, char *key)
{
	int length = 0;
	char* tmp = (char*)malloc(len+8);
	length = DESede_Decrypt(tmp,from,len,key);
	if(length <= 0){
		free(tmp);
		__android_log_print(ANDROID_LOG_INFO, "Jni", "length <= 0");
		tmp = NULL;
		return NULL;
	}
	char* ret = (char*)malloc(length);
	memcpy(ret,tmp,length);
	free(tmp);
	tmp = NULL;
	return ret;
}

jstring  Java_com_example_jnitest_Jnilib_d3dtest( JNIEnv* env,jobject thiz, jbyteArray icid)  //  加密
{

	__android_log_print(ANDROID_LOG_INFO, "Jni", "Encrypt");
	char d3ddata[100];
	memset(d3ddata, 0,100);
	//jsize len = (*env)->GetArrayLength(env, nums);
	char *data = (*env)->GetByteArrayElements(env, icid, 0);
	__android_log_print(ANDROID_LOG_INFO, "Jni", "The source data is %s",data);
	DESede_Encrypt(d3ddata,data,6,"Itc_Ymtk703_YmtkItc_Ymtk");

	char* tmp = D3dDes_Decrypt(d3ddata,8,"Itc_Ymtk703_YmtkItc_Ymtk");
	if(!tmp){
		__android_log_print(ANDROID_LOG_INFO, "Jni", "Java -- > C JNI :d3d error");
	}else {
		__android_log_print(ANDROID_LOG_INFO, "Jni", "Get data is %s",tmp);
	}
	return (*env)->NewStringUTF(env,tmp);
}

jbyteArray  Java_com_example_jnitest_Jnilib_ItcEncrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len) 
{

	__android_log_print(ANDROID_LOG_INFO, "Jni", "Encrypt");
	char d3ddata[100];
	char *data = (*env)->GetByteArrayElements(env, icid, 0);

	__android_log_print(ANDROID_LOG_INFO, "Jni", "The source data is %s",data);
	DESede_Encrypt(d3ddata,data,6,"Itc_Ymtk703_YmtkItc_Ymtk");

	jbyteArray rtnbytes = (*env)->NewByteArray(env,(jsize)strlen(d3ddata)); 
	(*env)->SetByteArrayRegion(env,rtnbytes,0,(jsize)strlen(d3ddata),(jbyte*)d3ddata); 
	return rtnbytes;
}

jstring  Java_com_example_jnitest_Jnilib_ItcDecrypt( JNIEnv* env,jobject thiz, jbyteArray icid, jint len)
{
	__android_log_print(ANDROID_LOG_INFO, "Jni", "Dectypt");
	char *d3ddata = (*env)->GetByteArrayElements(env, icid, 0);


	char* tmp = D3dDes_Decrypt(d3ddata,8,"Itc_Ymtk703_YmtkItc_Ymtk");
	if(!tmp){
		__android_log_print(ANDROID_LOG_INFO, "Jni", "Java -- > C JNI :d3d error");
	}else {
		__android_log_print(ANDROID_LOG_INFO, "Jni", "Get data is %s",tmp);
	}
	return (*env)->NewStringUTF(env,tmp);
}




jstring  Java_com_example_jnitest_Jnilib_md5( JNIEnv* env,jobject thiz, jbyteArray md5buf)
{
	md5_state_t ctx;

	char nu[33];
	unsigned char ret[16];
	char *data = (*env)->GetByteArrayElements(env, md5buf, 0);

	bzero(nu,33);
	memset(ret, 0, sizeof(ret));

	md5_init(&ctx);
	md5_append(&ctx, data, strlen(data));
	md5_finish(&ctx, ret);

	int i;
	printf("Md5 result: ");
	for (i = 0;i < 16;++i){
		sprintf(nu, "%s%02x",nu,ret[i]);
	}
	return (*env)->NewStringUTF(env,nu);
}
