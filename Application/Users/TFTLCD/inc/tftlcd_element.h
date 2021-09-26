#ifndef __TFTLCD_ELEMENT_H__
#define __TFTLCD_ELEMENT_H__

#include "stdio.h"
#include "tftlcd_conf.h"
#include "tftlcd.h"

extern TFTLCD_Config TFTLCD_Conf;

/* Box元素 */

typedef struct {

	uint8_t Box_Border_Width;
	uint32_t Box_Border_Color;
	uint8_t Box_Border_LineType;
	
} _Ele_Box_Border;


typedef struct {

	uint16_t Ele_Box_Width;
	uint16_t Ele_Box_Height;
	uint32_t Ele_Box_Color;
	uint8_t Ele_Box_IsSolid;
	
	_Ele_Box_Border Ele_Box_Border;
	
} TFTLCD_Ele_Box;

typedef union {

	TFTLCD_Ele_Box Ele_BoxProp;
	
} TFTLCD_EleProp_Type;


typedef struct {
	
	TFTLCD_EleProp_Type Ele_BoxProp;
	
	void (*Display)(uint16_t sx, uint16_t sy, TFTLCD_EleProp_Type box);
	void (*SetBackground)(uint32_t color);
	
} TFTLCD_Ele_Box_Obj;



typedef union {
	TFTLCD_Ele_Box_Obj Ele_Box_Obj;
} TFTLCD_EleObj_Type;


/* Element对象索引宏定义 */
#define ELE_BOX 0x01

TFTLCD_EleObj_Type* EleCreateObj(uint8_t ObjName, TFTLCD_EleProp_Type EleProp);

void DrawBox(uint16_t StartXPix, uint16_t StartYPix, TFTLCD_EleProp_Type Box);

void DrawLine(uint16_t StartXPix, uint16_t StartYPix, 
	uint16_t Width, uint16_t Height, uint16_t Color);


#endif