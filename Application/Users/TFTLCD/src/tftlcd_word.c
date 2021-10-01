#include "tftlcd_word.h"


													
// 显示单个字符
void TFTLCD_Word(uint16_t XPix, uint16_t YPix, void* _Obj) 
{			
	uint8_t Row, Col;
	uint16_t RowValue;
    
    TFTLCD_Word_PropType* WordObj;
    
    WordObj = (TFTLCD_Word_PropType*)_Obj;
    WordObj->XPix = XPix;
    WordObj->YPix = YPix;
				
	for(Row=0;Row<WordObj->FontSize;Row++) 
    {
		RowValue = ((WordObj->WordBuff[2*Row]<<8)|(WordObj->WordBuff[2*Row+1]));
		TFTLCD_SetCursor(XPix, YPix+Row);
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(Col=0;Col<WordObj->FontSize;Col++) 
        {
			if (RowValue&0x8000) 
                TFTLCD.RWCD_RAM = WordObj->FontFg;
			else 
                TFTLCD.RWCD_RAM = WordObj->FontBg;
			RowValue<<=1;
		}
	}
}


// 显示多个字符
void TFTLCD_Words(uint16_t XPix, uint16_t YPix, 
    uint16_t Width, uint16_t Height, void* _Obj) 
{
    TFTLCD_Words_ObjType* WordsObj;
    WordsObj = (TFTLCD_Words_ObjType*)_Obj;

	Width = Width/8;
    
//    for ()
//    WordsObj->WordObj.Display()
    
}

