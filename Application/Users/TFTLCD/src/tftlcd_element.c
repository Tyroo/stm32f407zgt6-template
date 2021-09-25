#include "tftlcd_element.h"


// 画一个Box（矩形）
void DrawBox(uint16_t StartXPix, uint16_t StartYPix, TFTLCD_Ele_Box Box) {
	
	uint16_t PixIndex;
	uint8_t BoderIndex;
	
	uint16_t BorderWidth;
	uint8_t BorderWidth2;
	
	uint8_t BorderTopDown;
	uint16_t BoxTotalWidth;
	
	uint16_t BoxColor;
	uint16_t BorderColor;
	
	// 颜色值转换
	BoxColor = RGB_To_U16(Box.Ele_Box_Color);
	BorderColor = RGB_To_U16(Box.Ele_Box_Border.Box_Border_Color);

	// 画矩形的上下边
	BorderWidth = Box.Ele_Box_Border.Box_Border_Width;
	BorderWidth2 = BorderWidth*2;
	BoxTotalWidth = Box.Ele_Box_Width + BorderWidth2;
	
	for(BoderIndex=0;BoderIndex<BorderWidth2;BoderIndex++) {
		
		BorderTopDown = BoderIndex/BorderWidth;
		TFTLCD_SetCursor(StartXPix, StartYPix+BoderIndex+BorderTopDown*Box.Ele_Box_Height);
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(PixIndex=0;PixIndex<BoxTotalWidth;PixIndex++) {
				TFTLCD.RWCD_RAM = BorderColor;
		}
	}
	
	// 画左右边框以及背景部分
	for(BoderIndex=0;BoderIndex<Box.Ele_Box_Height;BoderIndex++) {
		
		TFTLCD_SetCursor(StartXPix, StartYPix+BoderIndex+BorderWidth);
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(PixIndex=0;PixIndex<BoxTotalWidth;PixIndex++) {
			if(PixIndex<Box.Ele_Box_Border.Box_Border_Width || PixIndex>=BoxTotalWidth-BorderWidth)
				TFTLCD.RWCD_RAM = BorderColor;
			else
				TFTLCD.RWCD_RAM = BoxColor;
		}
	}
	
	
}