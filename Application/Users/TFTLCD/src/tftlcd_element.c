#include "tftlcd_element.h"


//void CreateEleObj(uint8_t ObjName, void Obj, void* Prop) {
//	
//	switch(ObjName) {
//		
//		case ELE_LINE_OBJ:
//			Obj.Ele_LineProp = *(Ele_Line_PropType*)Prop;;
//			Obj.Display = NULL;
//			Obj.SetBd = SetLineBd;
//			Obj = &LineObj;
//			break;
//		case ELE_BOX_OBJ:
//			BoxObj.Ele_BoxProp = *(Ele_Box_PropType*)Prop;
//			BoxObj.Display = DrawBox;
//			BoxObj.SetBd = SetBoxBd;
//			BoxObj.SetBg = SetBoxBg;
//			Obj = &BoxObj;
//			break;
//		case ELE_BUTTON_OBJ:
//			break;
//		default:
//			Obj = NULL;
//			break;
//	}
//	
//	return (*Obj);
//}

Ele_Box_ObjType CreateBoxObject(Ele_Box_PropType Prop) {
	
	Ele_Box_ObjType Obj;
	Obj.Ele_BoxProp = Prop;
	Obj.Display = DrawBox;
	Obj.SetBd = SetBoxBd;
	Obj.SetBg = SetBoxBg;
	
	return Obj;
}

/* Box object function */
void DrawBox(uint16_t StartXPix, uint16_t StartYPix, 
	void* _Obj) {
	
	uint16_t BorderWidth;
	uint16_t BoxTotalWidth;
	uint16_t BoxTotalHeight;
	
	uint16_t BoxColor;
	uint16_t BorderColor;
	
	// ÑÕÉ«Öµ×ª»»
//	BoxColor = RGB_To_U16(BoxProp.Ele_BoxProp.Ele_Box_Color);
//	BorderColor = RGB_To_U16(BoxProp.Ele_BoxProp.Ele_Box_Border.Ele_Line_Color);

//	BorderWidth = BoxProp.Ele_BoxProp.Ele_Box_Border.Ele_Line_Width;
//	BoxTotalWidth = BoxProp.Ele_BoxProp.Ele_Box_Width + BorderWidth * 2;
//	BoxTotalHeight = BoxProp.Ele_BoxProp.Ele_Box_Height + BorderWidth * 2;
//	
//	// »­ÉÏ±ß¿ò
//	DrawLine(StartXPix, StartYPix, BoxTotalWidth,
//		BorderWidth, BorderColor);
//	// »­ÓÒ±ß¿ò
//	DrawLine(StartXPix+BoxTotalWidth-BorderWidth, StartYPix+BorderWidth, 
//		BorderWidth, BoxProp.Ele_BoxProp.Ele_Box_Height, BorderColor);
//	// »­ÏÂ±ß¿ò
//	DrawLine(StartXPix, StartYPix+BoxTotalHeight-BorderWidth, 
//		BoxTotalWidth, BorderWidth, BorderColor);
//	// »­×ó±ß¿ò
//	DrawLine(StartXPix, StartYPix+BorderWidth, BorderWidth, 
//		BoxProp.Ele_BoxProp.Ele_Box_Height, BorderColor);
//	
//	if(!BoxProp.Ele_BoxProp.Ele_Box_IsSolid) return;
//	// »­ÊµÐÄ²¿·Ö
//	DrawLine(StartXPix+BorderWidth, StartYPix+BorderWidth, 
//		BoxProp.Ele_BoxProp.Ele_Box_Width, BoxProp.Ele_BoxProp.Ele_Box_Height, BoxColor);

	Ele_Box_ObjType* Obj;
	Obj = (Ele_Box_ObjType*)_Obj;

	BoxColor = RGB_To_U16(Obj->Ele_BoxProp.Ele_Box_Color);
	BorderColor = RGB_To_U16(Obj->Ele_BoxProp.Ele_Box_Border.Ele_Line_Color);

	BorderWidth = Obj->Ele_BoxProp.Ele_Box_Border.Ele_Line_Width;
	BoxTotalWidth = Obj->Ele_BoxProp.Ele_Box_Width + BorderWidth * 2;
	BoxTotalHeight = Obj->Ele_BoxProp.Ele_Box_Height + BorderWidth * 2;
	
	// »­ÉÏ±ß¿ò
	DrawLine(StartXPix, StartYPix, &(Obj->Ele_BoxProp.Ele_Box_Border));
	// »­ÓÒ±ß¿ò
	DrawLine(StartXPix+BoxTotalWidth-BorderWidth, StartYPix+BorderWidth, 
		&(Obj->Ele_BoxProp.Ele_Box_Border));
	// »­ÏÂ±ß¿ò
	DrawLine(StartXPix, StartYPix+BoxTotalHeight-BorderWidth, 
		&(Obj->Ele_BoxProp.Ele_Box_Border));
	// »­×ó±ß¿ò
	DrawLine(StartXPix, StartYPix+BorderWidth, &(Obj->Ele_BoxProp.Ele_Box_Border));
	
	if(!Obj->Ele_BoxProp.Ele_Box_IsSolid) return;
	// »­ÊµÐÄ²¿·Ö
	DrawLine(StartXPix+BorderWidth, StartYPix+BorderWidth, &(Obj->Ele_BoxProp.Ele_Box_Border));

}


void SetBoxBg(uint32_t Color, uint8_t IsSolid, void* _Obj) {
	uint16_t ColorU16;
	ColorU16 = RGB_To_U16(Color);
	
	Ele_Box_ObjType* Obj;
	Obj = (Ele_Box_ObjType*)_Obj;
	
	Obj->Ele_BoxProp.Ele_Box_IsSolid = IsSolid;
	if (!IsSolid) return;
	Obj->Ele_BoxProp.Ele_Box_Color = ColorU16;
}


void SetBoxBd(uint32_t Color, uint16_t Width, 
	uint16_t Height, uint8_t Type, void* _Obj) {
		
	Ele_Box_ObjType* Obj;
	Obj = (Ele_Box_ObjType*)_Obj;

	SetLineBd(Color, Width, Height, Type, &Obj->Ele_BoxProp.Ele_Box_Border);
	
}


/* Line object function */
void DrawLine(uint16_t StartXPix, 
	uint16_t StartYPix, void* _Obj)
{
	
	uint16_t PixXIndex;
	uint16_t PixYIndex;
	
	Ele_Line_ObjType* Obj;
	Obj = (Ele_Line_ObjType*)_Obj;
	
	for(PixYIndex=0;PixYIndex<Obj->Ele_LineProp.Ele_Line_Height;PixYIndex++) {
		
		TFTLCD_SetCursor(StartXPix, StartYPix+PixYIndex);
		TFTLCD_REG(TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode);
		
		for(PixXIndex=0;PixXIndex<Obj->Ele_LineProp.Ele_Line_Width;PixXIndex++) {
			TFTLCD_RAM(Obj->Ele_LineProp.Ele_Line_Color);
		}
	}
}


void SetLineBd(uint32_t Color, uint16_t Width, 
	uint16_t Height, uint8_t Type, void* _Obj) 
{
	uint16_t ColorU16;
	Ele_Line_ObjType* Obj;
	
	ColorU16 = RGB_To_U16(Color);
	Obj = (Ele_Line_ObjType*)_Obj;
	
	Obj->Ele_LineProp.Ele_Line_Color = ColorU16;
	Obj->Ele_LineProp.Ele_Line_Height = Height;
	Obj->Ele_LineProp.Ele_Line_Width = Width;
	Obj->Ele_LineProp.Ele_Line_Type = Type;
}