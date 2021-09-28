#ifndef __TFTLCD_ELEMENT_H__
#define __TFTLCD_ELEMENT_H__

#include "stdio.h"
#include "tftlcd_conf.h"
#include "tftlcd.h"

extern TFTLCD_Config TFTLCD_Conf;

/*----------------------------------------------------------/\
				           Object name definition                   /\
------------------------------------------------------------*/

/* Object name definition */
#define ELE_BOX_OBJ             0x00
#define ELE_LINE_OB             0x01
#define ELE_BUTTON_OBJ          0x02

/*----------------------------------------------------------\/
				            Set element property                    \/
------------------------------------------------------------*/


/*----------------------------------------------------------/\
				            Set element property                    /\
------------------------------------------------------------*/

/* Property of the element Line  */
#pragma pack(push,2)
typedef struct {

    uint16_t  Ele_Line_Width;
    uint16_t  Ele_Line_Height;
	uint8_t   Ele_Line_Type;
	uint32_t  Ele_Line_Color;
	
} Ele_Line_PropType;



/* Property of the element Box  */
typedef struct {
	
	uint16_t Ele_Box_Width;
	uint16_t Ele_Box_Height;
	uint32_t Ele_Box_Color;
	uint8_t  Ele_Box_IsSolid;
	
	Ele_Line_PropType Ele_Box_Border;
	
} Ele_Box_PropType;
#pragma pack(pop)

/*----------------------------------------------------------\/
				            Set element property                    \/
------------------------------------------------------------*/


/*----------------------------------------------------------/\
				             Set element object                     /\
------------------------------------------------------------*/

/* The object of the element Box */
typedef struct {
	
	Ele_Box_PropType Ele_BoxProp;
	
	void (*Display)(uint16_t const SX, uint16_t const SY, void* _Obj);
	void (*SetBg)(uint32_t const Color, uint16_t const Width, 
        uint16_t const Height, uint8_t const IsSolid, void* _Obj);
	void (*SetBd)(uint32_t const Color, uint16_t const WidthLR, 
        uint16_t const WidthTD, uint8_t const Type, void* _Obj);
	
} Ele_Box_ObjType;


/* The object of the element Line */
typedef struct {
	
	Ele_Line_PropType Ele_LineProp;
	
	void (*Display)(uint16_t SX, uint16_t SY, void* _Obj);
	void (*SetBd)(uint32_t const Color, uint16_t Width, 
		uint16_t Height, uint8_t Type, void* _Obj);
	
} Ele_Line_ObjType;

/*----------------------------------------------------------\/
				            Set element object                      \/
------------------------------------------------------------*/


/*----------------------------------------------------------/\
				       Object function declaration                  /\
------------------------------------------------------------*/

/* Box object constructor */
void CreateBoxObject(Ele_Box_ObjType* BoxObj, 
	Ele_Box_PropType Prop);

/* Box object display function */
void DrawBox(uint16_t const StartXPix, uint16_t const StartYPix, void* _Obj);

/* Box object setting background color function */
void SetBoxBg(uint32_t const Color, uint16_t const Width, 
    uint16_t const Height, uint8_t const IsSolid, void* _Obj);

/* Box object setting border color function */
void SetBoxBd(uint32_t const Color, uint16_t const Width, 
	uint16_t const Height, uint8_t const Type, void* _Obj);


/* Line object constructor */
void CreateLineObject(Ele_Line_ObjType* LineObj, 
	Ele_Line_PropType Prop);
    
/* Line object display function */
void DrawLine(uint16_t const StartXPix, uint16_t const StartYPix, void* Obj);

/* Line object setting border function */
void SetLineBd(uint32_t const Color, uint16_t const Width, 
	uint16_t const Height, uint8_t const Type, void* Obj);

/*----------------------------------------------------------\/
				         Object function declaration                \/
------------------------------------------------------------*/

#endif
