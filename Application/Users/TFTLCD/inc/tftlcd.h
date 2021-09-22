#ifndef __TFTLCD_H__

#define __TFTLCD_H__

#include "stm32f4xx_fsmc.h"
#include "delay.h"
#include "fsmc.h"
#include "tftlcd_conf.h"
#include "tftlcd_word.h"
#include "ILI9341.h"
#include "rgb.h"


#define USE_FSMC

#ifdef USE_FSMC
#define TFTLCD_BASE_ADDR (0x6C000000|0x7e)
#define TFTLCD (*(TFTLCD_RWCD*)TFTLCD_BASE_ADDR)
#else
#define TFTLCD_CMD_MODE
#define TFTLCD_DATA_MODE
#endif


void TFTLCD_Init(void);

uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr);

void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data);

void TFTLCD_SetWindow(uint16_t StarXPos, uint16_t EndXPos,
	uint16_t StarYPos, uint16_t EndYPos);

void TFTLCD_ReadDeviceId(void);

void TFTLCD_SetScanDir(uint16_t Dir);

void TFTLCD_SetCursor(uint16_t XPos, uint16_t YPos);

void TFTLCD_Clear(uint32_t Color);

void TFTLCD_DrawPoint(uint16_t XPos, uint16_t YPos, uint32_t Color);



#endif
