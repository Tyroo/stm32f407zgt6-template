#ifndef __TFTLCD_ELEMENT_H__
#define __TFTLCD_ELEMENT_H__

#include "tftlcd_conf.h"
#include "tftlcd.h"

extern TFTLCD_Config TFTLCD_Conf;

typedef struct {

	uint8_t Box_Border_Width;
	uint32_t Box_Border_Color;
	uint8_t Box_Border_LineType;
	
} _Ele_Box_Border;


typedef struct {

	uint16_t Ele_Box_Width;
	uint16_t Ele_Box_Height;
	uint32_t Ele_Box_Color;
	
	_Ele_Box_Border Ele_Box_Border;
	
} TFTLCD_Ele_Box;


void DrawBox(uint16_t StartXPix, uint16_t StartYPix, TFTLCD_Ele_Box Box);


#endif