#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


#define OPSTARTDATA  0X7E7E7E7E

static unsigned  char checksum(char *data, int len)
{
	unsigned short sum = 0;
	int i;

	for (i = 0; i < len; i++) {
		sum ^= data[i];
	}
	sum = ~sum;

	return ((sum + 1) & 0xff);
}

compake()
{
	sdate.opstart = OPSTARTDATA;
	sdate.opcmd = cmd;
	sdate.opdevid = devid;
	seq = (int *)(&(sdate.sks.sign));
	//*seq = seqsk;
	sdate.sks.sq = sequence;
	sdate.sks.sign |= (seqsk << 5);

	memcpy(sdate.data, data, leng);
	sdate.leng = leng;

	if(mode == 1){
		sdate.leng = leng + OPSGPSLENG;
		get_standard_gpsmsg(sdate.data+leng);
	}
	unsigned int Encrypt_Length = (sdate.leng/8+1)*8;


	DESede_Encrypt(senddata.data,sdate.data,sdate.leng,"123456789012345678901234");

	sdate.sks.chk = checksum((char *)&(sdate.opstart), sdate.leng + 20);
	sdate.opstart = htonl(sdate.opstart);
	sdate.opcmd = htonl(sdate.opcmd);
	sdate.opdevid = htonl(sdate.opdevid);
	sdate.leng = htonl(sdate.leng);
	sdate.sks.sq = htons(sdate.sks.sq);

	if(fp_baklog){
		fwrite(&sdate,1,leng+OPSGPSLENG+20,fp_baklog);
		fflush(fp_baklog);
	}

	DESede_Encrypt(senddata.head,(char*)&sdate.opcmd,16,"123456789012345678901234");
	senddata.opstart = htonl(OPSTARTDATA);
}
