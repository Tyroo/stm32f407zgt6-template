#include "fsmc.h"


void FsmcSram_Init(void) {
	
	u8 GPIOD_AF_Array[12] = {0,1,4,5,8,9,10,11,12,13,14,15};
	u8 GPIOE_AF_Array[11] = {0,1,7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[10] = {0,1,2,3,4,5,12,13,14,15};
	u8 GPIOG_AF_Array[7] = {0,1,2,3,4,5,10};
	
		/* �������ýṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 

	/* ʱ��ʹ��/ʧ�� */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* ����GPIO */
	// GPIOB����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						//PB15 �������,���Ʊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);								//��ʼ�� //PB15 �������,���Ʊ���
	// GPIOD����
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(0XFF<<8);// PD0,1,4,5,8~15,NOE,NWE
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// �������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// ����
  GPIO_Init(GPIOD, &GPIO_InitStructure);								// ��ʼ��������
	// GPIOE����
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);								// ��ʼ��������
	// GPIOF����
	GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12);		// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);								// ��ʼ����ַ��
	// GPIOG����	
	GPIO_InitStructure.GPIO_Pin =(0X3F<<0)| GPIO_Pin_10;	// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);								// ��ʼ����ַ��
	
	/* GPIO�������� */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 12);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 11);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 10);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 7);
	
	/* FSMC���� */
	// FSMC��дʱ������
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0x00; //��ַ����ʱ��Ϊ 2 �� HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��ģʽ A δ�õ�
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 0x08; //���ݱ���ʱ��Ϊ 9�� HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //ģʽ A 
	// FSMC����
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;											// ����FSMC������ⲿSRAM�ĵ�ַ��Bank1�Ŀ�3��
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;					// �ڴ�����ΪSRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; //�洢�����ݿ��Ϊ8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//�洢��дʹ��
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 	// ��дʹ����ͬ��ʱ��
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_ReadWriteTiming; 	//��дͬ��ʱ��
	
	/* ʹ��/ʧ�� */
	FSMC_NORSRAMInit(&FSMC_InitStructure); //��ʼ�� FSMC ����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE); // ʹ�� BANK1 ���� 3

}


void FsmcSram_Write(u16* Data, u32 StartAddr, u16 Length) {
	
	u16 DataIndex;
	
	for(DataIndex=0;DataIndex<Length;DataIndex+=2) {
		*(vu16*)(SRAM_START_ADDR+StartAddr+DataIndex) = *(vu16*)(Data+DataIndex);
	}
}


vu16 FsmcSram_Read(u32 Addr) {
	return *(vu16*)(SRAM_START_ADDR+Addr);
}


void Fsmc_AF_Config(GPIO_TypeDef* Gpio, u8* Arr, u8 Len) {
	int i;
	for(i=0;i<Len;i++) {
		GPIO_PinAFConfig(Gpio, *(Arr+i), GPIO_AF_FSMC);
	}
}
