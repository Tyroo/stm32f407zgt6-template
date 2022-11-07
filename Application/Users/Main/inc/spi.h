#ifndef __SPI_H__
#define __SPI_H__


/**
*	@include
**/
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "system.h"
#include "dma.h"

/**
*	@defined
**/
#define SPI3_RTX_BUFF_SIZE    (16)

/**
*	@variable
**/
extern char Spi1_ReceiveData[50];

extern uint8_t Spi3_DmaWriteData[SPI3_RTX_BUFF_SIZE];
extern uint8_t Spi3_DmaReadData[SPI3_RTX_BUFF_SIZE];


/**
*	@function
**/
void SPI1_Init(void);

void SPI3_Init(void);

bool SPI1_Send(char* SendData);



#endif
