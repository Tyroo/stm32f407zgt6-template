#include "rgb.h"

uint16_t RGB_To_U16(uint32_t Rgb) {
	
	uint8_t Rgb_Red;
	uint8_t Rgb_Geen;
	uint8_t Rgb_Blue;
	
	Rgb_Red = Rgb>>16;
	Rgb_Geen = (Rgb&0xFFFF)>>8;
	Rgb_Blue = Rgb&0xFF;
	
	Rgb_Red = 31*Rgb_Red/255;
	Rgb_Geen = 63*Rgb_Geen/255;
	Rgb_Blue = 31*Rgb_Blue/255;
	
	return ((Rgb_Red)|(Rgb_Geen<<5)|(Rgb_Blue<<11));
}
