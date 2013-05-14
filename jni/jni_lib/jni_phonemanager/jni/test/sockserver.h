#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sys/select.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define SUCCEESS			  0
#define ERR_TOOMANYSOCKET  -1
#define ERR_MALLOC			-2
#define ERR_CREATSOCKET		-3
#define ERR_CONNECT			-4
#define ERR_BIND			-5
#define ERR_LINSTEN			-6
#define ERR_ACCEPT			-7
#define ERR_FD				-8
#define ERR_SEND			-9
#define ERR_RECV			-10
#define ERR_UDP_SEND			-11
#define ERR_UDP_RECV			-12
#define ERR_TYPE			-13

#define MAXERRNO			12

#define MAXSOCKETNO	1024
#define JUDGECFD			0
#define JUDGESFD			1
#define INVALIDFD			2

#define  FREE_SERVER  1
#define FREE_CLINT	2
#define  JPNETRECVTRYTIME 3




#define  FREE_SERVER  1
#define FREE_CLIENT	2





#define handle_error(msg) \
     do { perror(msg); } while (0)

#define PACKET_LENG 	1500
#define REVERSE16(val)  ((val >> 8) | (val & 0xff <<8))
#define REVERSE32(val)  (((val) >> 24) | ((val) >> 8 & 0xff00) | (((val) & 0xff00) << 8)  | (((val) & 0xff) << 24))


typedef struct dtmsg{
	int sfd;
	int cfd;
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	socklen_t s_addrsize;
	socklen_t c_addrsize;

	int sendmsgleng;	
	int recvmsgleng;	
	long sendmsg[PACKET_LENG];
	long recvmsg[PACKET_LENG];
}dtcmr_msg_t;


typedef struct __cmr_msglib{
	int msgfd;
	dtcmr_msg_t *dtmsg;
}cmr_msglib_t;

typedef struct __msglib_errno{
	int errnb;
	char *errstr;
}msglib_errno_t;


//creat socket of tcp,and return fd 
int dtcmr_client_creat(char *serverip, short port, int type);
int dtcmr_serv_creat(int sport, int listenback, int type);
//type IPPROTO_TCP/IPPROTO_UDP
int dtcmr_accept(int fd);
int dtcmr_send(int fd, char *buf, int leng);
int dtcmr_recv(int fd, char *buf, int leng) ;
int dtcmr_recv_timeout(int fd, char *buf, int leng, struct timeval *timeout);
int dtcmr_udp_send(int fd, char *buf, int leng);
int dtcmr_udp_recv(int fd, char *buf, int leng);
void dtcmr_shotdown(int fd, int flags);
void dtcmr_free(int fd);



