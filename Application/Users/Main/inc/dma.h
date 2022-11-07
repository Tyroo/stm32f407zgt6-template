#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f4xx_dma.h"
#include "spi.h"
#include "nvic.h"


extern uint8_t DMA_C0S0_5_StartProcess;


void DMA1_C0S0_5_Init(void);

void DMA1_C0S0_5_Start(void);

#endif
