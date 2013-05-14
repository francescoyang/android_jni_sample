/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for STmicroelectronics STM32F4xx
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/

void SER_Init1 (void) {
int i;
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
   
  RCC->APB2ENR  |= (1UL << 4);         /* Enable USART1 clock                */
  RCC->APB2ENR |= (1UL << 0);//TIMx?
  RCC->AHB1ENR  |= (1UL << 0);          /* Enable GPIOA clock                 */

  /* Connect PA9 and PA10 to Alternate Function 7 (AF7) */
  GPIOA->MODER  &= (~((3UL << 2*9) | (3UL << 2*10)));   // 11 1100 0000 0000 0000 0000 
  GPIOA->MODER  |= ((2UL << 2*9) | (2UL << 2*10));
 // GPIOA->MODER  &= ~((3UL << 2*8));   // 11 1100 0000 0000 0000 0000 
  //GPIOA->MODER  |= ((2UL << 2*8));
  GPIOA->AFR[1] &= ~0x00000FF0;
  GPIOA->AFR[1] |=  0x00000770;

  /* Configure USART1: 115200 baud @ 84MHz, 8 bits, 1 stop bit, no parity     */
  USART1->BRR = (45 << 4) | 9;
  USART1->CR3 = 0x0000;
  USART1->CR2 = 0x0000;
  for (i = 0; i < 0x1000; i++) __NOP();
  USART1->CR1 = 0x200C;
  

/* Configure USART1: 9600 baud @ 84MHz, 8 bits, 1 stop bit, no parity     */
   //USART1->BRR = (22 << 4) | 12;	  // 1 0110 1100
  USART1->BRR = (546 << 4) | 14;
  USART1->CR3 = 0x0000;
  USART1->CR2 = 0x0000;
  for (i = 0; i < 0x1000; i++) __NOP();
  USART1->CR1 = 0x200C;
  
#endif
}

void SER_Init2 (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RX  BUFFER_EMPTY;

#else
  RCC->APB1ENR  |= (1UL << 17);         /* Enable USART2 clock                */
  RCC->APB2ENR |= (1UL<<0);
  RCC->AHB1ENR  |= (1UL << 3);          /* Enable GPIOD clock                 */

  /* Connect PD5 and PD6 to Alternate Function 7 (AF7) */
  GPIOD->MODER  &= ~((3 << 2*5) | (3 << 2*6));
  GPIOD->MODER  |= (2 << 2*5) | (2 << 2*6);
  GPIOD->AFR[0] &= ~0x0FF00000;
  GPIOD->AFR[0] |=  0x07700000;

  /* Configure USART2: 115200 baud @ 42MHz, 8 bits, 1 stop bit, no parity     */
  USART2->BRR = (22 << 4) | 12;	  // 1 0110 1100
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;
#endif
}

void SER_Init3 (void) {

#ifdef __DBG_ITM
  ITM_RxBuffer   =  ITM_RXBUFFER_EMPTY;
#else
  int i;
  RCC->APB1ENR  |=  (   1 << 18);       /* Enable USART3 clock */                                          
  RCC->AHB1ENR  |=  (   1 <<  2);       /* Enable GPIOC clock                 */
  
  GPIOC->MODER  &= ~(   3 << 20);
  GPIOC->MODER  |=  (   2 << 20);       /* PC10: Alternate function mode      */
  GPIOC->AFR[1] &= ~(0x0F <<  8);
  GPIOC->AFR[1] |=  (   7 <<  8);       /* PC10: Alternate function USART3_TX */
  GPIOC->MODER  &= ~(   3 << 22);
  GPIOC->MODER  |=  (   2 << 22);       /* PC11: Alternate function mode      */
  GPIOC->AFR[1] &= ~(0x0F << 12);
  GPIOC->AFR[1] |=  (   7 << 12);       /* PC11: Alternate function USART3_RX */

  
   /* Configure UART3 for 57600 baud*/  
  USART3->BRR    =  0x016c;             /* Configure 57600 baud, @ 42MHz     */
  USART3->CR3    =  0x0000;             /*           8 bit, 1 stop bit,       */
  USART3->CR2    =  0x0000;             /*           no parity                */
  for (i = 0; i < 0x1000; i++) __NOP(); /* avoid unwanted output              */
  USART3->CR1    =  0x200C;
#endif
}

/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to Serial Port
 *----------------------------------------------------------------------------*/

 int32_t SER_PutChar1 (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
   while (!(USART1->SR & 0x0080));
  USART1->DR = (ch & 0xFF);
#endif  
  return (ch);
}

int32_t SER_PutChar2 (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
 	   while (!(USART2->SR & 0x0080));
  		USART2->DR = (ch & 0xFF);
#endif  

  return (ch);
}


int SER_PutChar3 (int ch) {

#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(USART3->SR & 0x0080));
  USART3->DR = (ch & 0xFF);
#endif
  return (ch);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/

 int32_t SER_GetChar1 (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART1->SR & 0x0020)
    return (USART1->DR);
#endif
  return (-1);
}

int32_t SER_GetChar2 (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART2->SR & 0x0020)
    return (USART2->DR);
#endif
  return (-1);
}

int SER_GetChar3 (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return (ITM_ReceiveChar());
#else
  if (USART3->SR & 0x0020)
  	 return (USART3->DR);	  
   
#endif
  return (-1);
}


/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
