#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>


jstring readgps(JNIEnv* env,int fd,int len)
{
	
	char read_buf[len];
	memset(read_buf,0,len);
	read(fd,read_buf, len);	
	read_buf[len-1] = '\0';
	return (*env)->NewStringUTF(env,read_buf);
}
