#ifndef __TFTLCD_H__

#define __TFTLCD_H__

#include "stm32f4xx_fsmc.h"
#include "delay.h"
#include "fsmc.h"
#include "tftlcd_conf.h"


#define USE_FSMC

#ifdef USE_FSMC
#define TFTLCD_BASE_ADDR (0x6C000000|0x7e)
#define TFTLCD (*(TFTLCD_RWCD*)TFTLCD_BASE_ADDR)
#else
#define TFTLCD_CMD_MODE
#define TFTLCD_DATA_MODE
#endif


__weak void TFTLCD_Init(void);

uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr);

void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data);

void TFTLCD_SetScreenFrame(void);

void TFTLCD_SetCursor(uint16_t XPos, uint16_t YPos);



#endif
