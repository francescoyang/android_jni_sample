#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#define PARENTS 500
typedef struct __dirlist{
	char pasition;
	char dirname[200];
}dirlist_t;

typedef struct __dirline{
	int line;
	dirlist_t get_info[PARENTS];
}dirline_t;

int dirlen = 0;
int i = 0;
int j = 0;

void listDir(char *path)
{
	DIR              *pDir ;
	struct dirent    *ent  ;
	int               i=0  ;
	char              childpath[512];

	pDir=opendir(path);
	memset(childpath,0,sizeof(childpath));

	i ++;

	while((ent=readdir(pDir))!=NULL)
	{
		j ++;

		if(ent->d_type & DT_DIR)
//		if(ent->d_type)
		{
			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
				continue;
			/*
			if(strlen(ent->d_name) > 512)
				continue;
				*/
			sprintf(childpath,"%s/%s",path,ent->d_name);
//			printf( "%s  i =  %d, j = %d\n ",ent-> d_name,i,j); 
			printf("path:%s i =  %d, j = %d\n",childpath,i,j);

				listDir(childpath);
		}else {
			printf( "%s  i =  %d, j = %d\n ",ent-> d_name,i,j); 
		}
	}
	closedir(pDir);

}

int main(int argc,char *argv[])
//int main()
{

//	listDir("/home");
	listDir(argv[1]);
	return 0;
}
