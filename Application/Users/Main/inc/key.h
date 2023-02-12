#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡ IO*/

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) //PE3 
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PA0

#define KEY0_PRES 1
#define KEY1_PRES 2
#define KEY2_PRES 3
#define WKUP_PRES 4


typedef enum
{
	enNoPress,
	enOncePress,
	enDoublePress,
	enLongPress,
} enKeyPress;


void Key_Init(void); //IO ��ʼ��
u8 Key_Scan(u8); 		 //����ɨ�躯��
u8 Key0_Scan(void);
enKeyPress ThreeStageKeyScan(void);	// ��״̬��ťɨ��


#endif
