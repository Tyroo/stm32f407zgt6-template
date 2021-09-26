#include "tftlcd_element.h"


TFTLCD_EleObj_Type* EleCreateObj(uint8_t ObjName, TFTLCD_EleProp_Type EleProp) {
	
	TFTLCD_EleObj_Type* EleObj;
	
	switch(ObjName) {
		
		case 0:
			EleObj->Ele_Box_Obj.Ele_BoxProp = EleProp;
			EleObj->Ele_Box_Obj.Display = DrawBox;
			
		default:
			EleObj = NULL;
	}
	
	return EleObj;
}


// ��һ��Box�����Σ�
void DrawBox(uint16_t StartXPix, uint16_t StartYPix, TFTLCD_EleProp_Type Box) {
	
	uint16_t BorderWidth;
	uint16_t BoxTotalWidth;
	uint16_t BoxTotalHeight;
	
	uint16_t BoxColor;
	uint16_t BorderColor;
	
	// ��ɫֵת��
	BoxColor = RGB_To_U16(Box.Ele_BoxProp.Ele_Box_Color);
	BorderColor = RGB_To_U16(Box.Ele_BoxProp.Ele_Box_Border.Box_Border_Color);

	// �����ε����±�
	BorderWidth = Box.Ele_BoxProp.Ele_Box_Border.Box_Border_Width;
	BoxTotalWidth = Box.Ele_BoxProp.Ele_Box_Width + BorderWidth * 2;
	BoxTotalHeight = Box.Ele_BoxProp.Ele_Box_Height + BorderWidth * 2;
	// ���ϱ߿�
	DrawLine(StartXPix, StartYPix, BoxTotalWidth,
		BorderWidth, BorderColor);
	// ���ұ߿�
	DrawLine(StartXPix+BoxTotalWidth-BorderWidth, StartYPix+BorderWidth, 
		BorderWidth, Box.Ele_BoxProp.Ele_Box_Height, BorderColor);
	// ���±߿�
	DrawLine(StartXPix, StartYPix+BoxTotalHeight-BorderWidth, 
		BoxTotalWidth, BorderWidth, BorderColor);
	// ����߿�
	DrawLine(StartXPix, StartYPix+BorderWidth, BorderWidth, 
		Box.Ele_BoxProp.Ele_Box_Height, BorderColor);
	
	if(!Box.Ele_BoxProp.Ele_Box_IsSolid) return;
	// ��ʵ�Ĳ���
	DrawLine(StartXPix+BorderWidth, StartYPix+BorderWidth, 
		Box.Ele_BoxProp.Ele_Box_Width, Box.Ele_BoxProp.Ele_Box_Height, BoxColor);
}


void DrawLine(uint16_t StartXPix, uint16_t StartYPix, 
	uint16_t Width, uint16_t Height, uint16_t Color) 
{
	
	uint16_t PixXIndex;
	uint16_t PixYIndex;
		
	for(PixYIndex=0;PixYIndex<Height;PixYIndex++) {
		
		TFTLCD_SetCursor(StartXPix, StartYPix+PixYIndex);
		TFTLCD_REG(TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode);
		
		for(PixXIndex=0;PixXIndex<Width;PixXIndex++) {
			TFTLCD_RAM(Color);
		}
	}
}