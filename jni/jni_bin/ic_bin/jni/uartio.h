#ifndef UARTIO_H
#define UARTIO_H

#define IC_DEV          1 //s3c2410_serial2  ttl
#define FINGE_DEV       2 //s3c2410_serial3  rs485

int open_port(int comport);
int set_opt(int fd, int nspeed, int nbits, char nevent, int nstop);
#endif
