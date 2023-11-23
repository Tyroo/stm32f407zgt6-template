#include "tftlcd_element.h"


/* Box object constructor */
void CreateBoxObject(Ele_Box_ObjType* BoxObj, 
	Ele_Box_PropType Prop)
{
	BoxObj->Ele_BoxProp = Prop;
	BoxObj->Display = DrawBox;
	BoxObj->SetBd = SetBoxBd;
	BoxObj->SetBg = SetBoxBg;
}


/* Box object function */
void DrawBox(uint16_t StartXPix, uint16_t StartYPix, 
	void* _Obj) 
{
	
    uint32_t BoxColor;
	uint16_t BorderWidth, BorderHeight;
    uint16_t BoxWidth, BoxHeight;
    uint16_t BoxTotalWidth, BoxTotalHeight;
    
	Ele_Box_ObjType BoxObj;
    Ele_Line_ObjType LineObj;
    
    CreateBoxObject(&BoxObj, 
        ((Ele_Box_ObjType*)_Obj)->Ele_BoxProp);
    CreateLineObject(&LineObj, 
        ((Ele_Box_ObjType*)_Obj)->Ele_BoxProp.Ele_Box_Border);

	BorderWidth = BoxObj.Ele_BoxProp.Ele_Box_Border.Ele_Line_Width;
    BorderHeight = BoxObj.Ele_BoxProp.Ele_Box_Border.Ele_Line_Height;
	
    BoxWidth = BoxObj.Ele_BoxProp.Ele_Box_Width;
    BoxHeight = BoxObj.Ele_BoxProp.Ele_Box_Height;
    BoxColor = BoxObj.Ele_BoxProp.Ele_Box_Color;
    
    BoxTotalWidth = BoxObj.Ele_BoxProp.Ele_Box_Width + BorderWidth * 2;
	BoxTotalHeight = BoxObj.Ele_BoxProp.Ele_Box_Height + BorderHeight * 2;
	
    LineObj.Ele_LineProp.Ele_Line_Width = BoxTotalWidth;
    LineObj.Ele_LineProp.Ele_Line_Height = BorderHeight;
	
	// »­BoxÉÏ±ß¿ò
	LineObj.Display(StartXPix, StartYPix, &LineObj);
    // »­BoxÏÂ±ß¿ò
	LineObj.Display(StartXPix, StartYPix+BoxTotalHeight-BorderHeight, 
        &LineObj);
    
    LineObj.Ele_LineProp.Ele_Line_Width = BorderWidth;
    LineObj.Ele_LineProp.Ele_Line_Height = BoxHeight;
	
    // »­BoxÓÒ±ß¿ò
	LineObj.Display(StartXPix+BoxTotalWidth-BorderWidth, 
        StartYPix+BorderHeight, &LineObj);
	// »­Box×ó±ß¿ò
	LineObj.Display(StartXPix, StartYPix+BorderHeight, &LineObj);
	
	if(!BoxObj.Ele_BoxProp.Ele_Box_IsSolid) return;
	
    // »­BoxÊµÐÄ²¿·Ö
    LineObj.Ele_LineProp.Ele_Line_Width = BoxWidth;
    LineObj.Ele_LineProp.Ele_Line_Height = BoxHeight;
    LineObj.Ele_LineProp.Ele_Line_Color = BoxColor;
    
	LineObj.Display(StartXPix+BorderWidth, 
        StartYPix+BorderHeight, &LineObj);

}


/* Setting Box background color */
void SetBoxBg(uint32_t const Color, uint16_t const Width, 
    uint16_t const Height, uint8_t const IsSolid, void* _Obj) 
{

	Ele_Box_ObjType* Obj;
	Obj = (Ele_Box_ObjType*)_Obj;
	
	Obj->Ele_BoxProp.Ele_Box_IsSolid = IsSolid;
	Obj->Ele_BoxProp.Ele_Box_Color = IsSolid ? Color : 0xFFFFFF;
    Obj->Ele_BoxProp.Ele_Box_Width = Width;
    Obj->Ele_BoxProp.Ele_Box_Height = Height;
}


/* Setting Box border color */
void SetBoxBd(uint32_t const Color, uint16_t const WidthLR, 
    uint16_t const WidthTD, uint8_t const Type, void* _Obj) 
{
		
	SetLineBd(Color, WidthLR, WidthTD, Type, 
        &(((Ele_Box_ObjType*)_Obj)->Ele_BoxProp.Ele_Box_Border));
	
}


/* Line object constructor */
void CreateLineObject(Ele_Line_ObjType* LineObj, 
	Ele_Line_PropType Prop)
{
    LineObj->Ele_LineProp = Prop;
    LineObj->Display = DrawLine;
    LineObj->SetBd = SetLineBd;
}


/* Set Line object display */
void DrawLine(uint16_t const StartXPix, 
	uint16_t const StartYPix, void* _Obj)
{
	
	uint16_t PixXIndex;
	uint16_t PixYIndex;
    
    uint16_t ColorU16;
	Ele_Line_ObjType* Obj;
    
	Obj = (Ele_Line_ObjType*)_Obj;
    ColorU16 = COLOR16(Obj->Ele_LineProp.Ele_Line_Color);
    
	for(PixYIndex=0;
        PixYIndex<Obj->Ele_LineProp.Ele_Line_Height;
        PixYIndex++) 
    {
		
		TFTLCD_SetCursor(StartXPix, StartYPix+PixYIndex);
		TFTLCD_REG(TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode);
		
		for(PixXIndex=0;
            PixXIndex<Obj->Ele_LineProp.Ele_Line_Width;
            PixXIndex++) 
        {
			TFTLCD_RAM(ColorU16);
		}
	}
}


/* Setting Line border color */
void SetLineBd(uint32_t const Color, uint16_t const Width, 
	uint16_t const Height, uint8_t const Type, void* _Obj) 
{
	Ele_Line_ObjType* Obj;
	
	Obj = (Ele_Line_ObjType*)_Obj;
	
	Obj->Ele_LineProp.Ele_Line_Color = Color;
	Obj->Ele_LineProp.Ele_Line_Height = Height;
	Obj->Ele_LineProp.Ele_Line_Width = Width;
	Obj->Ele_LineProp.Ele_Line_Type = Type;
}

