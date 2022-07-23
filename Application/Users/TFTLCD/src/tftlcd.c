#include "tftlcd.h"


TFTLCD_Config TFTLCD_Conf;




#ifdef USE_FSMC
/* ʹ��FSMC������� */

// TFTLCD��ʼ������
void TFTLCD_Init(void) {
	
	u8 GPIOD_AF_Array[9] = {0,1,4,5,8,9,10,14,15};
	u8 GPIOE_AF_Array[9] = {7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[1] = {12};
	u8 GPIOG_AF_Array[1] = {12};
	
	/* �������ýṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef FSMC_WriteTiming;

	/* ʱ��ʹ��/ʧ�� */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* ����GPIO */
	// GPIOB����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						// PB15 �������,���Ʊ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					// ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// ����
    GPIO_Init(GPIOB, &GPIO_InitStructure);								// ��ʼ�� //PB15 �������,���Ʊ���
	GPIO_SetBits(GPIOB, GPIO_Pin_15);											// ��������
	// GPIOD����
	GPIO_InitStructure.GPIO_Pin =  (0x33)|(7<<8)|(3<<14); // PD0,1,4,5,8~15,NOE,NWE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// �������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// ����
    GPIO_Init(GPIOD, &GPIO_InitStructure);								// ��ʼ��������
	// GPIOE����
	GPIO_InitStructure.GPIO_Pin =  (0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);					// ��ʼ��������
	// GPIOF����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);					// ��ʼ����ַ��
	// GPIOG����	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;				// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// ��ʼ����ַ��
	
	/* GPIO�������� */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 9);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 9);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 1);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 1);
	
	/* FSMC���� */
	// FSMC��дʱ������
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0xF; //��ַ����ʱ��Ϊ 16 �� HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��ģʽ A δ�õ�
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 60; 	//���ݱ���ʱ��Ϊ 25�� HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //ģʽ A 
	// FSMCдʱ��
	FSMC_WriteTiming.FSMC_AddressSetupTime = 0x03; //��ַ����ʱ��Ϊ 3 �� HCLK
	FSMC_WriteTiming.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��ģʽ A δ�õ�
	FSMC_WriteTiming.FSMC_DataSetupTime = 0x02; 	//���ݱ���ʱ��Ϊ 3�� HCLK
	FSMC_WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //ģʽ A 
	// FSMC����
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;						// ����FSMC������ⲿSRAM�ĵ�ַ��Bank1�Ŀ�4��
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	// �ر����ݵ�ַ����
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;				// �ڴ�����ΪSRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 	// �洢�����ݿ��Ϊ8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	// �洢��дʹ��
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 		// ��дʹ�ò���ͬ��ʱ��
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;	// ��дʱ����ͬ
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTiming;			// ��дʱ����ͬ
	
	/* ʹ��/ʧ�� */
	FSMC_NORSRAMInit(&FSMC_InitStructure);									// ��ʼ��FSMC����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);							// ʹ��BANK1����4
	
	/* TFTLCD�豸��ʼ�� */
	TFTLCD_Conf.TFTLCD_Dir = 0;		// ����ģʽ
	TFTLCD_Conf.TFTLCD_Height = 320;// ��Ļ�߶�320
	TFTLCD_Conf.TFTLCD_Width = 240; // ��Ļ���240
	TFTLCD_Conf.TFTLCD_Cmd = (TFTLCD_CMD){0xD3,0x36,0x2A,0x2B,0x2C,0x2E};	// ICָ��

	TFTLCD_ReadDeviceId();								// ��ȡTFTLCD IC ID
	ILI9341_Init(&TFTLCD.RWCD_REG, &TFTLCD.RWCD_RAM);	// ILI9341��ʼ��
	TFTLCD_SetScanDir(TFTLCD_Conf.TFTLCD_Dir);			// ������Ļ����
	TFTLCD_SetWindow(0, TFTLCD_Conf.TFTLCD_Width-1, 0, TFTLCD_Conf.TFTLCD_Height-1);	// ����Ļ�Ͽ���
	
	TFTLCD_Clear(0xFFFFFF);	

}

 
// ��ָ�������TFTLCD RAM
uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr) {
	TFTLCD.RWCD_REG = REG_Addr;
	Delay_Us(5);
	return TFTLCD.RWCD_RAM;
}


// ��ָ��ָ��дTFTLCD RAM
void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data) {
	TFTLCD.RWCD_REG = REG_Addr;
	TFTLCD.RWCD_RAM = Data;
}


void TFTLCD_ReadDeviceId(void) {
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Read_ID;
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// ��Ч����
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// ��Ч���� 0x00
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// 0x93
	TFTLCD_Conf.TFTLCD_Id <<= 8;
	TFTLCD_Conf.TFTLCD_Id |= TFTLCD.RWCD_RAM;	// 0x9341
}


void TFTLCD_SetScanDir(uint16_t Dir) {
	// Dir=0��������Dir=1������
	TFTLCD_WriteRAM(TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ScanDir, Dir);
}


void TFTLCD_SetCursor(uint16_t XPos, uint16_t YPos) {
	
	// ����X����
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_XPos;
	TFTLCD.RWCD_RAM = XPos>>8;
	TFTLCD.RWCD_RAM = XPos&0xFF;
	// ����Y����
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_YPos;
	TFTLCD.RWCD_RAM = YPos>>8;
	TFTLCD.RWCD_RAM = YPos&0xFF;
	
}


void TFTLCD_DrawPoint(uint16_t XPos, uint16_t YPos, uint32_t Color) {
	
	uint16_t Color16;
	
	Color16 = COLOR16(Color);
	TFTLCD_SetCursor(XPos, YPos);	// ���õ�����
	
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
	TFTLCD.RWCD_RAM = Color16;
}


//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void TFTLCD_DrawBlock(uint16_t StartXPos, uint16_t StartYPos, 
	uint16_t EndXPos, uint16_t EndYPos, 
	uint32_t *ColorBuff)
{  
	static uint16_t height, width;
	static uint16_t i, j;
	
	width = EndXPos - StartXPos + 1; 			//�õ����Ŀ��
	height = EndYPos - StartYPos + 1;			//�߶�
	
 	for(i=0;i<height;i++)
	{
 		TFTLCD_SetCursor(StartXPos, StartYPos+i);  //���ù��λ�� 
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(j=0;j<width;j++)
			TFTLCD.RWCD_RAM = ColorBuff[i*width+j];//д������ 
	}		  
} 


// ��ָ����ɫ����
void TFTLCD_Clear(uint32_t Color) {
	
	uint32_t PointSum;
	uint32_t PointIndex;
	uint16_t Color16;
	
	Color16 = COLOR16(Color);
	PointSum = TFTLCD_Conf.TFTLCD_Height * TFTLCD_Conf.TFTLCD_Width;
	
	TFTLCD_SetCursor(0, 0);
	
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
	
	for(PointIndex=0;PointIndex<PointSum;PointIndex++) 
	{
		TFTLCD.RWCD_RAM = Color16;	
	}
	
}


// ����Ļ�Ͽ���һ������
void TFTLCD_SetWindow(uint16_t StarXPos, uint16_t EndXPos,
	uint16_t StarYPos, uint16_t EndYPos) {
	
	// �滮��Ļ�ĺ�����
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_XPos;
	TFTLCD.RWCD_RAM = StarXPos>8;
	TFTLCD.RWCD_RAM = StarXPos&0xFF;
	TFTLCD.RWCD_RAM = EndXPos>8;
	TFTLCD.RWCD_RAM = EndXPos&0xFF;
	
	// �滮��Ļ������߶�
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_YPos;
	TFTLCD.RWCD_RAM = StarYPos>8;
	TFTLCD.RWCD_RAM = StarYPos&0xFF;
	TFTLCD.RWCD_RAM = EndYPos>8;
	TFTLCD.RWCD_RAM = EndYPos&0xFF;
}





#else
// TFTLCD��ʼ������
__weak void TFTLCD_Init(void) {
	
	u8 GPIOD_AF_Array[9] = {0,1,4,5,8,9,10,14,15};
	u8 GPIOE_AF_Array[9] = {7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[1] = {12};
	u8 GPIOG_AF_Array[1] = {12};
	
		/* �������ýṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef FSMC_WriteTiming;

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
	GPIO_InitStructure.GPIO_Pin =  (0x33)|(7<<8)|(3<<14); // PD0,1,4,5,8~15,NOE,NWE
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// �������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// ����
  GPIO_Init(GPIOD, &GPIO_InitStructure);								// ��ʼ��������
	// GPIOE����
	GPIO_InitStructure.GPIO_Pin =  (0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);					// ��ʼ��������
	// GPIOF����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);					// ��ʼ����ַ��
	// GPIOG����	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;				// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// ��ʼ����ַ��
	
	/* GPIO�������� */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 9);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 9);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 1);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 1);
	
	/* FSMC���� */
	// FSMC��дʱ������
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0xF; //��ַ����ʱ��Ϊ 16 �� HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��ģʽ A δ�õ�
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 0x19; 	//���ݱ���ʱ��Ϊ 24�� HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //ģʽ A 
	// FSMCдʱ��
	FSMC_WriteTiming.FSMC_AddressSetupTime = 0xF; //��ַ����ʱ��Ϊ 16 �� HCLK
	FSMC_WriteTiming.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��ģʽ A δ�õ�
	FSMC_WriteTiming.FSMC_DataSetupTime = 0x19; 	//���ݱ���ʱ��Ϊ 24�� HCLK
	FSMC_WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //ģʽ A 
	// FSMC����
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;										// ����FSMC������ⲿSRAM�ĵ�ַ��Bank1�Ŀ�4��
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // �ر����ݵ�ַ����
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;						// �ڴ�����ΪSRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 	// �洢�����ݿ��Ϊ8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	// �洢��дʹ��
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 			// ��дʹ�ò���ͬ��ʱ��
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;// ��дʱ����ͬ
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTiming; 				// ��дʱ����ͬ
	
	/* ʹ��/ʧ�� */
	FSMC_NORSRAMInit(&FSMC_InitStructure); 				// ��ʼ��FSMC����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); // ʹ��BANK1����4
}


// ��ָ�������TFTLCD RAM
uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr) {
	TFTLCD.RWCD_REG = REG_Addr;
	Delay_Us(5);
	return TFTLCD.RWCD_RAM;
}


// ��ָ��ָ��дTFTLCD RAM
void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data) {
	TFTLCD.RWCD_REG = REG_Addr;
	TFTLCD.RWCD_RAM = Data;
}

#endif




