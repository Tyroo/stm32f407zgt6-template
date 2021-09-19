#ifndef __NVIC_H__
#define __NVIC_H__

/**
* @include
**/
#include "misc.h"


/**
* @function
* ���ã�����NVIC�����жϷ���ȵ�
* ������Group
* ����ֵ��void
**/
void Nvic_Init(int Group);


/**
* @function
* ���ã�����USART1���ж����ȼ���ʹ�ܵ�
* ������{ PrePriority����ռ���ȼ���SubPriority����Ӧ���ȼ���Cmd���Ƿ�ʹ�ܱ�־λ }
* ����ֵ��void
**/
void Nvic_Config(int NvicName, int PrePriority, int SubPriority, int Cmd);

#endif

