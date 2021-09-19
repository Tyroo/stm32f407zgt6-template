#ifndef __TFTLCD_WORD_H__
#define __TFTLCD_WORD_H__

#include "tftlcd.h"

typedef struct {
	
	uint8_t* Word_FontSize;
	uint8_t* Word_FontType;
	uint8_t* Word_FontColor;
	
} TFTLCD_Word_Font;


void TFTLCD_Words(uint8_t AxisArrsX, uint8_t AxisArrsY, 
			uint8_t WordArrs, TFTLCD_Word_Font* WordFont);


void TFTLCD_Word(uint8_t AxisX, uint8_t AxisY, 
			uint8_t WordArr, TFTLCD_Word_Font* WordFont);



#endif
