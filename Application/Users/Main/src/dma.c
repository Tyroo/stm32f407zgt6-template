#include "dma.h"


uint8_t DMA_C0S0_5_StartProcess = 0;


void DMA1_C0S0_5_Init(void)
{
	/* Config struct define */
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* Deinitialize DMA Streams */
	DMA_DeInit(DMA1_Stream0);
	DMA_DeInit(DMA1_Stream5);
  
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = SPI3_RTX_BUFF_SIZE;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&SPI3->DR);
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	/* Configure RX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(&Spi3_DmaReadData[0]); 
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);
	
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(&Spi3_DmaWriteData[0]);
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	
	/* Config DMA1 DMA1_Stream0 or DMA1_Stream1 Interrupt */
	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
	
	/* NVIC config */
	Nvic_Config(DMA1_Stream5_IRQn, 2, 2, ENABLE);
	Nvic_Config(DMA1_Stream0_IRQn, 2, 2, ENABLE);

	/* Enable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Stream5,ENABLE);
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Stream0,ENABLE);
}


void DMA1_C0S0_5_Start(void)
{
	if (DMA_C0S0_5_StartProcess == 1)
	{
		DMA_C0S0_5_StartProcess = 0;
		
		// User Code...

		/* Enable DMA SPI TX Stream */
		DMA_Cmd(DMA1_Stream5,ENABLE);
		/* Enable DMA SPI RX Stream */
		DMA_Cmd(DMA1_Stream0,ENABLE);
	}
}


// SPI3 DMA接收完成中断
void DMA1_Stream0_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_Stream0, DMA_IT_TCIF0);
	DMA_Cmd(DMA1_Stream0, DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Stream0, SPI3_RTX_BUFF_SIZE);
	// 设置传输内存首地址
	DMA1_Stream0->M0AR = (uint32_t)(&Spi3_DmaReadData[0]);
	
	DMA_C0S0_5_StartProcess = 1;
}

// SPI3 DMA发送完成中断
void DMA1_Stream5_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);
	DMA_Cmd(DMA1_Stream5, DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Stream5, SPI3_RTX_BUFF_SIZE);
	// 设置传输内存首地址
	DMA1_Stream5->M0AR = (uint32_t)(&Spi3_DmaWriteData[0]);
}



