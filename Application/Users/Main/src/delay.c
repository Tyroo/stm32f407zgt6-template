/* ��ʱģ�飨��֤ͨ���� */
#include "delay.h"


// ��ʱģ���ʼ������
void Delay_Init() {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	// ����SysTick��ʱ��(9MHz)
}


// ʵ��΢�뼶��ʱ
void Delay_Us(uint16_t Us) {
	
	uint32_t Temp;
	uint32_t CountValue = Us * DELAY_ONE_US;	 // �������ʱUs΢����Ҫ������ֵ
	
	SysTick->LOAD = CountValue; 						   // ʱ����� 
	SysTick->VAL = 0x00; 											 // ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // ��ʼ����

	// ��systick��ʱ���ݼ�������δ����0�Ҽ����������ڿ���ʱһֱ����
	do {
		Temp = SysTick->CTRL;
	} while((Temp&0x01) && !(Temp&(1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // ֹͣ��ʱ��
	SysTick->VAL = 0x00; 											 // ��ռ�����
}


// ʵ�ֺ��뼶��ʱ
void Delay_Ms(uint16_t Ms) {
	
	uint32_t Temp;
	uint32_t CountValue = Ms * DELAY_ONE_MS;				 // �������ʱMs������Ҫ������ֵ
	
	SysTick->LOAD = CountValue; 						   // ʱ����� -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SysTick->VAL = 0x00; 											 // ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // ��ʼ����
	
	// ��systick��ʱ���ݼ�������δ����0�ҵݼ����������ڿ���ʱһֱ����
	do {
		Temp = SysTick->CTRL;
	} while((Temp&0x01) && !(Temp&(1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // ֹͣ��ʱ��
	SysTick->VAL = 0x00; 											 // ��ռ�����
}

