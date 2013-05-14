#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <jni.h>
#include <android/log.h>
#include "fileinfo.h"

	jint Java_com_acanoe_appmanager_Appmanager_setmmsinfo
(JNIEnv *env, jclass thiz,jstring mmsname, jstring mmsnumber,jstring mmsbody, jstring date, jint typeId, jint count)
{
	int len_mmsname= 0;
	int len_mmsnumber= 0;
	int len_mmsbody= 0;
	int len_mmsdate= 0;

	jboolean iscopy;

	const char *mms_name= (*env)->GetStringUTFChars(env, mmsname, &iscopy);
	const char *mms_number= (*env)->GetStringUTFChars(env, mmsnumber, &iscopy);
	const char *mms_body= (*env)->GetStringUTFChars(env, mmsbody, &iscopy);
	const char *mms_date= (*env)->GetStringUTFChars(env, date, &iscopy);


	if(!mms_number || !mms_body || !mms_date)
	{
		return -1;
	}

	len_mmsnumber= strlen(mms_number);
	len_mmsbody= strlen(mms_body);
	len_mmsdate= strlen(mms_date);

	if((len_mmsnumber> 18) || (len_mmsbody > 2048) || (len_mmsdate > 25))
	{
		return -2;
	}

	mmsinfo.count = count;
	mmsinfo.get_info[count].typeId= typeId;

	if(mms_name)
	{
		len_mmsname= strlen(mms_name);
		if(len_mmsname> 20)
			return -2;

		memcpy(mmsinfo.get_info[count].mmsname,mms_name,len_mmsname);
//		LOGD("mmsname = %s",mmsinfo.get_info[count].mmsname);
	}

	memcpy(mmsinfo.get_info[count].mmsnumber,mms_number,len_mmsnumber);
	memcpy(mmsinfo.get_info[count].mmsbody,mms_body,len_mmsbody);
	memcpy(mmsinfo.get_info[count].mmsdate,mms_date,len_mmsdate);
#if 0
	LOGD("mmsnumber= %s",mmsinfo.get_info[count].mmsnumber);
	LOGD("mmsbody= %s",mmsinfo.get_info[count].mmsbody);
	LOGD("mmsdate= %s",mmsinfo.get_info[count].mmsdate);
#endif

}
