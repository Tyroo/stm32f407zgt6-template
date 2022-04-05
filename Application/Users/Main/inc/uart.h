#ifndef __UART_H__
#define __UART_H__


/**
* @include
**/
#include "stm32f4xx_usart.h"
#include <string.h>
#include <stdio.h>
#include "nvic.h"


/**
* @define
**/
#define USE_PRINTF_FUNC 1


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
void Uart1_Init(uint32_t Baud);


/**
* @function
* ���ã�ʹ��USART1��������
* ������{ Data: Ҫ���͵�����ָ�� }
* ����ֵ��void
**/
void Uart1_Send(char *Data);



#endif
