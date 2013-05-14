/*-----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *-----------------------------------------------------------------------------
 *      Name:    RETARGET.C 
 *      Purpose: Retarget low level functions
 *      Rev.:    V4.22
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <rt_misc.h>
#include <string.h>
#include <rt_sys.h>
#include <File_Config.h>
#include "Serial.h"



#pragma import(__use_no_semihosting_swi)

/* The following macro definitions may be used to translate this file:

  STDIO - use standard Input/Output device
          (default is NOT used)
 */
extern 	int _sys_close (FILEHANDLE fh);
/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
#ifdef STDIO

/*-----------------------------------------------------------------------------
  Write character to the Serial Port
 *----------------------------------------------------------------------------*/

int sendchar1 (int c) {

  if (c == '\n')  {
    SER_PutChar1 ('\r');
  }
  SER_PutChar1 (c);

  return (c);
}

int sendchar2 (int c) {

  if (c == '\n')  {
    SER_PutChar2 ('\r');
  }
  SER_PutChar2 (c);

  return (c);
}

int sendchar3 (int c) {

  if (c == '\n')  {
    SER_PutChar3 ('\r');
  }
  SER_PutChar3 (c);

  return (c);
}



/*-----------------------------------------------------------------------------
  Read character from the Serial Port
 *----------------------------------------------------------------------------*/

int getkey1 (void) {
  int ch = SER_GetChar1();

  if (ch < 0) {
    return 0;
  }
  return ch;
}

int getkey2 (void) {
  int ch = SER_GetChar2();

  if (ch < 0) {
    return 0;
  }
  return ch;
}

int getkey3 (void) {
  int ch = SER_GetChar3();

  if (ch < 0) {
    return -1;
  }
   return ch;
}

#endif


/*--------------------------- _ttywrch ---------------------------------------*/

void _ttywrch (int ch) {
#ifdef STDIO
  sendchar2 (ch);
#endif
}

/*--------------------------- _sys_exit --------------------------------------*/

void _sys_exit (int return_code) {
  /* Endless loop. */
  while (1);
}

/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/

int fputc (int c,FILE *f)
{
	return sendchar2(c);
}

int fgetc (FILE *f)
{
	return getkey2 ();	
}

static int TimingDelay;

void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);

}
 

