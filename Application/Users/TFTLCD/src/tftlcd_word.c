#include "tftlcd_word.h"


													
// 显示单个字符
void TFTLCD_Word(uint16_t AxisX, uint16_t AxisY, 
			uint8_t* WordArr, TFTLCD_Word_Font WordFont) {
				
	uint8_t Row, Col;
	uint8_t RowIndex;
	uint16_t RowValue;
	
	RowIndex = 0;
				
	for(Row=0;Row<WordFont.Word_FontSize;Row++) {
		
		RowValue = ((WordArr[RowIndex]<<8)|(WordArr[RowIndex+1]));
		TFTLCD_SetCursor(AxisX, AxisY+Row);
		TFTLCD.RWCD_REG = 0x2C;
		
		for(Col=0;Col<WordFont.Word_FontSize;Col++) {
			
			if (RowValue&0x8000) 
					TFTLCD.RWCD_RAM = WordFont.Word_FontColor;
			else 
					TFTLCD.RWCD_RAM = WordFont.Word_FontBackground;
			RowValue<<=1;
		}
		RowIndex += 2;
	}
}


// 显示多个字符
void TFTLCD_Words(uint16_t AxisArrsX, uint16_t AxisArrsY, 
			uint8_t* WordArrs, TFTLCD_Word_Font WordFont) {
	// pass
}
