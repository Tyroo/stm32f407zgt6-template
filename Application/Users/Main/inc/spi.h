#ifndef __SPI1_H__
#define __SPI1_H__


/**
*	@include
**/
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "sys.h"

/**
*	@variable
**/
extern char SpiReceiveData[50];


/**
*	@function
**/
void SPI1_Init(void);

bool SPI1_Send(char* SendData);



#endif
