#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f4xx_dma.h"
#include "spi.h"
#include "nvic.h"


void DMA1_CH1_2_Init(void);

void DMA1_CH1_2_Start(void);

void DMA1_CH1_2_Reset(void);

#endif
