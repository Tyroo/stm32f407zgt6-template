#ifndef __LED_H__
#define __LED_H__

/**
*	@include
**/
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


/**
*	@function
**/
void Led_Init(void);

void Led_Control(uint8_t Status);



#endif
