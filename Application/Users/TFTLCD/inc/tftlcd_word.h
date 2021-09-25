#ifndef __TFTLCD_WORD_H__
#define __TFTLCD_WORD_H__

#include <math.h>
#include "tftlcd.h"

typedef struct {
	
	uint8_t Word_FontSize;
	uint8_t Word_FontType;
	uint32_t Word_FontColor;
	uint32_t Word_FontBackground;
	
} TFTLCD_Word_Font;


void TFTLCD_Word(uint16_t AxisX, uint16_t AxisY, 
			uint8_t* WordArr, TFTLCD_Word_Font WordFont);


void TFTLCD_Word(uint16_t AxisArrsX, uint16_t AxisArrsY, 
			uint8_t* WordArrs, TFTLCD_Word_Font WordFont);



#endif
