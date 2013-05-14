#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "data_update.h"
#include "jpnetsocket_api.h"
#include "softwareconfig.h"
#include "debug_out.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_dataupmod{
	void *datap;
	int pthread_status;
	netdev_t dev;
	pthread_t tid;
	pthread_attr_t tattr;
}dataupmod_t;

static dataupmod_t dataupmod;

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

int dataup_pthread_modules_kill(void)
{
	pthread_cancel(dataupmod.tid);
	//save val
	return 0;
}

int test_dataup_pthread_status(void)
{
	int status;
	status = dataupmod.pthread_status;
	return status;
}

static int dataup_reply(void* handle, int cmd, int leng, char *data)
{
	int *seq = NULL;
	struct oppacketdata sdata;
	encrypteddata senddata;
	memset(&sdata, 0, sizeof(struct oppacketdata));
	memset(&senddata, 0, sizeof(encrypteddata));

	sdata.opstart = OPSTARTDATA;
	sdata.opcmd = cmd;
	sdata.opdevid = get_devid();
	seq = (int *)(&(sdata.sks.sign));
	//*seq = seqsk;

	sdata.leng = leng;
	unsigned int Encrypt_Length = (sdata.leng/8+1)*8;
	memcpy(sdata.data,data,leng);

	DESede_Encrypt(senddata.data,sdata.data,leng,"123456789012345678901234");

	sdata.sks.chk = checksum((char *)&(sdata.opstart), leng + 20);
	sdata.opstart = htonl(sdata.opstart);
	sdata.opcmd = htonl(sdata.opcmd);
	sdata.opdevid = htonl(sdata.opdevid);
	sdata.leng = htonl(sdata.leng);

	DESede_Encrypt(senddata.head,(char*)&sdata.opcmd,16,"123456789012345678901234");
	senddata.opstart = htonl(OPSTARTDATA);

	jpnetsocket_send(handle,(char*)&senddata,Encrypt_Length + OPPACKETHEADLENG);
//		*seq = htonl(*seq);
}

void* dataup_pthread(void *arg)
{
	netdev_t *dev = &dataupmod.dev;
	encrypteddata recvdata;
	oppacketdata_t data;
	memset(&recvdata,0,sizeof(recvdata));
	memset(&data,0,sizeof(data));
	char flag_succ = 0x0;
	char flag_fail = 0x1;
	struct timeval timeout;
	memset(&timeout,0,sizeof(struct timeval));
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	int seq;
	
	dataupmod.datap = jpnetsocket_server_init(dev->sport, dev->slistens, dev->sprotocol);
	if (!dataupmod.datap) {
		DEBUG_OUT(DEBUG_GZQ,"dataup socket error!\n");
		goto datauperror;
	}
	DEBUG_OUT(DEBUG_GZQ,"data update: server init ok. server port = %d\n",dev->sport);

	while(1){
		if(jpnetsocket_accept(dataupmod.datap) == -1){
			DEBUG_OUT(DEBUG_GZQ,"dataup accept error!\n");
			goto datauperror;
		}
		DEBUG_OUT(DEBUG_GZQ,"data update: accepting...\n");

		if(jpnetsocket_recv_timeout(dataupmod.datap, (char*)&recvdata.opstart, 1, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		} else if (*((char*)&recvdata.opstart) != 0x7e){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		}

		if(jpnetsocket_recv_timeout(dataupmod.datap, (char*)&recvdata.opstart+1, 1, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		} else if (*((char*)&recvdata.opstart+1) != 0x7e) continue;

		if(jpnetsocket_recv_timeout(dataupmod.datap, (char*)&recvdata.opstart+2, 1, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		} else if (*((char*)&recvdata.opstart+2) != 0x7e) continue;

		if(jpnetsocket_recv_timeout(dataupmod.datap, (char*)&recvdata.opstart+3, 1, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		} else if (*((char*)&recvdata.opstart+3) != 0x7e) continue;

		if(jpnetsocket_recv_timeout(dataupmod.datap, recvdata.head, 24, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		}

		char* tmp = JP3Des_Decrypt(recvdata.head,24,"123456789012345678901234");
		if(!tmp){
			DEBUG_OUT(DEBUG_GZQ,"JP3Des_Decrypt return is bad\n");
			continue;
		}
		memcpy(&data.opcmd,tmp,16);

		data.opcmd = ntohl(data.opcmd);	
		data.opdevid = ntohl(data.opdevid);	
		seq = ntohl(*((int *)&data.sks));	
		data.sks.sign = ((seq >> 24) & 0xff);	
		data.sks.chk = ((seq >> 16) & 0xff);	
		data.sks.sq = (seq & 0xffff);	
		data.leng = ntohl(data.leng);
		if (data.leng >= NET_RECVBUF_SIZE) {
			data.leng = NET_RECVBUF_SIZE - 1;
		}

		DEBUG_OUT(DEBUG_GZQ,"data update: receive packet head success.\n");

		if(jpnetsocket_recv_timeout(dataupmod.datap, (char*)&recvdata.data, (data.leng/8+1)*8, &timeout) < 0){
			jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
			continue;
		}

		tmp = JP3Des_Decrypt(recvdata.data,(data.leng/8+1)*8,"123456789012345678901234");

		if(!tmp){
			DEBUG_OUT(DEBUG_GZQ,"JP3Des_Decrypt return is bad\n");
			continue;
		}
		memcpy(&data.data[0],tmp,data.leng);
/*		int i;
		DEBUG_OUT(DEBUG_GZQ,"data update: cmd = 0x%x data: ",data.opcmd);
		for(i=0;i<data.leng;i++){
			DEBUG_OUT(DEBUG_GZQ,"0x%x ",*((int*)&data.data+i));
		}
		DEBUG_OUT(DEBUG_GZQ,"\n");
*/		
		switch (data.opcmd){
			case 0x0c:
				DEBUG_OUT(DEBUG_GZQ,"data update: cmd = 12, now start to analyse...\n");
				set_dataup_updateconfig((updateconfig*)data.data);
				dataup_reply(dataupmod.datap,12,1,&flag_succ);
				break;
			case 0x0d:
				DEBUG_OUT(DEBUG_GZQ,"data update: cmd = 13, now start to analyse...\n");
				localconfig config;
				memset(&config,0,sizeof(config));
				get_dataup_localconfig(&config);
				dataup_reply(dataupmod.datap,13,sizeof(config),(char*)&config);
				break;
			case 0x0e:
				DEBUG_OUT(DEBUG_GZQ,"data update: cmd = 14, now start to analyse...\n");
				set_dataup_updatekey((d3deskey*)data.data);
				dataup_reply(dataupmod.datap,14,1,&flag_succ);
				break;
			case 0x0f:
				DEBUG_OUT(DEBUG_GZQ,"data update: cmd = 15, now start to analyse...\n");
				set_dataup_msg((systemmsg*)data.data);
				dataup_reply(dataupmod.datap,15,1,&flag_succ);
				break;
			default:
				dataup_reply(dataupmod.datap,15,1,&flag_fail);
				break;
		}//analyse
			//reply
		jpnetsocket_shutdown(dataupmod.datap,FREE_CLIENT);
		memset(&recvdata,0,sizeof(recvdata));
		memset(&data,0,sizeof(data));
		DEBUG_OUT(DEBUG_GZQ,"data update: disconnect...\n");
	}
	jpnetsocket_close(dataupmod.datap);
datauperror:
	
	dataupmod.pthread_status = PTHREAD_ERROR;
	
	if (dataupmod.datap) {
		jpnetsocket_shutdown(dataupmod.datap, FREE_CLIENT);
		jpnetsocket_close(dataupmod.datap);
		dataupmod.datap = NULL;
	}

	pthread_exit(NULL);
}

int dataup_pthread_modules_init(netdev_t *devname)
{
	dataupmod.pthread_status = PTHREAD_START;
	
	memcpy(&dataupmod.dev, devname, sizeof(netdev_t));

	if (!pthread_attr_init(&dataupmod.tattr)) {
		if (!pthread_attr_setdetachstate(&dataupmod.tattr, PTHREAD_CREATE_DETACHED)) {
			pthread_create(&dataupmod.tid, &dataupmod.tattr, dataup_pthread, NULL);
			pthread_attr_destroy(&dataupmod.tattr);
		}	
	}

	DEBUG_OUT(DEBUG_GZQ,"data update: pthread started.\n");
	
	return 0;
}



#ifdef __cplusplus
}
#endif
