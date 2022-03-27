#include "dma.h"


void DMA1_CH0_1_Init(void)
{
	/* Config struct define */
	DMA_InitTypeDef DMA_InitStructure;

	/* Deinitialize DMA Streams */
	DMA_DeInit(DMA1_Stream0);
	DMA_DeInit(DMA1_Stream1);
  
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(SPI1->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(&Spi3_DmaWriteData[0]);
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);
	
	/* Configure RX DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(&Spi3_DmaReadData[0]); 
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	
	/* Config DMA1 DMA1_Stream0 or DMA1_Stream1 Interrupt */
	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
	
	/* NVIC config */
	Nvic_Config(DMA1_Stream0_IRQn, 0, 2, ENABLE);
	Nvic_Config(DMA1_Stream1_IRQn, 0, 2, ENABLE);
}


void DMA1_CH0_1_Start(void)
{
	/* NSS Pin control */
	// pass
	
	// 清空SPI3手法缓冲区
	SPI3->DR;
	/* Enable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Stream0,ENABLE);
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Stream1,ENABLE);
}


void DMA1_CH0_1_Reset(void)
{
	/* Disable DMA SPI TX Stream */
	DMA_Cmd(DMA1_Stream0, DISABLE);
	/* Disable DMA SPI RX Stream */
	DMA_Cmd(DMA1_Stream1, DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Stream0, 8);
	DMA_SetCurrDataCounter(DMA1_Stream1, 8);
	
	// 设置传输内存首地址
	DMA1_Stream0->M0AR = (uint32_t)(&Spi3_DmaWriteData[0]);
	DMA1_Stream1->M0AR = (uint32_t)(&Spi3_DmaReadData[0]);
	
	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
	DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
}


// SPI3 DMA发送完成中断
void DMA1_Stream0_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
	}
}

// SPI3 DMA接收完成中断
void DMA1_Stream1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1))
	{
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		
		// 处理数据
		// pass
		
		// 复位SPI3 DMA
		DMA1_CH0_1_Reset();
	}
}



