#ifndef __UART_H__
#define __UART_H__


/**
* @include
**/
#include "stm32f4xx_usart.h"


/**
* @variable
**/
extern char UsartReceiveData[50];



/**
* @function
* ���ã���ʼ��UART1
* ������{ Baud: UART�Ĳ����� }
* ����ֵ��void
**/
void Uart1_Init(u32 Baud);


/**
* @function
* ���ã�ʹ��USART1��������
* ������{ Data: Ҫ���͵�����ָ�� }
* ����ֵ��void
**/
void Uart1_Send(char *Data);



#endif
