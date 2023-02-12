#include "led.h"


// ��ʼ��LEDģ��
void Led_Init() {
	
	/* ���ýṹ�嶨�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʱ��ʹ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	// ʹ��PC�˿�ʱ��
	
	/* GPIOF���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			// ���ø�GPIOΪ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9;	// ����Ϊ10��
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;		// ����ٶ�Ϊ����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// �������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		// ��ʹ������������
	
	/* Ӧ��GPIO���� */
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}


// ����LED��״̬
void Led_Control(uint8_t Status) {
	
	if (Status&1)
		GPIO_ResetBits(GPIOF, GPIO_Pin_10);		// ����LED��
	else
		GPIO_SetBits(GPIOF, GPIO_Pin_10);		// �ر�LED��
		
}
