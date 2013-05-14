		  /*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.h
 *      Purpose: Serial I/O Definitions
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __SERIAL_H
#define __SERIAL_H

void     SER_Init1    (void);
int  SER_GetChar1 (void);
int  SER_PutChar1 (int ch);
int sendchar1(int);

void     SER_Init2    (void);
int  SER_GetChar2 (void);
int  SER_PutChar2 (int ch);
int sendchar2(int);
//int getkey2(void);


void     SER_Init3    (void);
int  SER_GetChar3 (void);
int  SER_PutChar3 (int ch);
int sendchar3(int);
//int getkey3(void);
//extern void     SER_Init4    (void);

void Delay(unsigned int nTime);

#endif  /* __SERIAL_H */
