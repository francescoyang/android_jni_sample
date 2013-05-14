#include <stdio.h>

#define MAXNU   5000 
#define APPMAXNU   200	
#define BOOKMAXNU   2000	
#define MAXSIZE 16
#define MAXNAME 50
#define MAXPATH 200

#define ERROR_INFO	-1
#define ERROR_LEN	-2
#define ERROR_MAXNU	-3

static const char *TAG="JNI";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)


typedef struct __fileinfo{
	char filesize[16];
	char filepath[200];
	char filename[50];
}fileinfo_t;


typedef struct __appinfo{
	int  apppath;		// in sdcard or phonestore
	int  appattribute;  // system app or user app
	char appname[50];
	char apppackname[100];
	char appversion[15];
	char appsize[16];
}appinfo_t;

typedef struct __storageinfo{
	char sdSize[32];			// SD卡总容量
	char sdAvail[32];			// SD卡剩余容量
	char memoryAvail[32];		// 手机内存剩余容量
	char memorySize[32];		// 手机内存总容量
}storageinfo_t;

typedef struct __mmsinfo{
	char mmsname[20];
	char mmsnumber[18];
	char mmsbody[1024*2];
	char mmsdate[25];
	char typeId; 
}mmsinfo_t;

typedef struct __bookinfo{
	char bookname[20]; 
	char booknumber[18];
}bookinfo_t;



typedef struct __dirlist{
	char pasition;
	char dirname[200];
}dirlist_t;

typedef struct __dirline{
	int line;
	dirlist_t get_info[100];
}dirline_t;

typedef struct __sendmessage{
	char mmsnumber[18];
	char mmsdata[1024];
}sendmessage_t;

typedef struct __allinfo{
	int count;
	fileinfo_t get_info[MAXNU];
}media_t;


typedef struct __allappnfo{
	int count;
	appinfo_t get_info[APPMAXNU];
}allappinfo_t;


typedef struct __allmmsinfo{
	int count;
	mmsinfo_t get_info[BOOKMAXNU];
}allmmsinfo_t;

typedef struct __allbookinfo{
	int count;
	bookinfo_t get_info[BOOKMAXNU];
}allbookinfo_t;



media_t imageinfo;
media_t videoinfo;
media_t musicinfo;

allappinfo_t appinfo;
allbookinfo_t bookinfo;
allmmsinfo_t mmsinfo;
storageinfo_t storageinfo;
sendmessage_t sendmessage;


void close_socket();
void net_pthread_exit();
