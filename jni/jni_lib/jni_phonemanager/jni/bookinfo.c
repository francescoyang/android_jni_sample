#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jni.h>
#include <android/log.h>
#include "fileinfo.h"

jint Java_com_acanoe_appmanager_Appmanager_setbookinfo
(JNIEnv *env, jclass thiz,jstring bookname, jstring booknumber,  jint count)
{
	int len_bookname = 0;
	int len_booknumber = 0;

	jboolean iscopy;

	const char *book_name= (*env)->GetStringUTFChars(env, bookname, &iscopy);
	const char *book_number= (*env)->GetStringUTFChars(env, booknumber, &iscopy);
//	LOGD("setbookinfo start %s, %s, %d",book_name ,book_number, count);
/*
	if((book_name = NULL) ||  (book_number = NULL))
	{
		return -1;
	}
	*/

	len_bookname = strlen(book_name);
	len_booknumber= strlen(book_number);

	if((len_bookname > 20) || (len_booknumber > 18))
	{
		return -2;
	}

	bookinfo.count = count;
//	LOGD("bookinfo.count = %d",bookinfo.count);

	memcpy(bookinfo.get_info[count].bookname,book_name,len_bookname);
//	LOGD("bookinfo.get_info[count].bookname = %s",bookinfo.get_info[count].bookname);
	memcpy(bookinfo.get_info[count].booknumber,book_number,len_booknumber);
//	LOGD("bookinfo.get_info[count].booknumber= %s",bookinfo.get_info[count].booknumber);
	return 1;
}
