#ifndef __TFTLCD_WORD_H__
#define __TFTLCD_WORD_H__

#include <math.h>
#include "tftlcd.h"


extern TFTLCD_Config TFTLCD_Conf;

/*----------------------------------------------------------/\
			  Font Settings macro definition                /\
------------------------------------------------------------*/

/* The font size to display */
#ifndef WORD_SIZE_LIST_NULL_VALUE
#define WORD_SIZE_LIST_NULL_VALUE    FONT_PIXED_SIZE16
#endif

#define FONT_PIXED_SIZE12          0x0C
#define FONT_PIXED_SIZE16          0x10
#define FONT_PIXED_SIZE24          0x18

#if (WORD_SIZE_LIST_NULL_VALUE == FONT_PIXED_SIZE12)
    #define WORD_BUFF_DEFAULT_SIZE             0x18
#elif (WORD_SIZE_LIST_NULL_VALUE == FONT_PIXED_SIZE16)
    #define WORD_BUFF_DEFAULT_SIZE             0x20
#elif (WORD_SIZE_LIST_NULL_VALUE == FONT_PIXED_SIZE24)
    #define WORD_BUFF_DEFAULT_SIZE             0x48
#else
    #define WORD_BUFF_DEFAULT_SIZE             0x20
#endif

/*----------------------------------------------------------\/
			  Font Settings macro definition                \/
------------------------------------------------------------*/


typedef struct
{
    uint16_t XPix;
    uint16_t YPix;
    uint8_t  FontSize;
	uint8_t  FontType;
	uint32_t FontFg;
	uint32_t FontBg;
    
    uint8_t* WordBuff;

} TFTLCD_Word_PropType;


typedef struct 
{
	
    TFTLCD_Word_PropType WordProp;
    
    void (*SetWordBuff)(uint8_t* Buff);
    
    void (*Display)(uint16_t XPix, 
        uint16_t YPix, void* _Obj);
    void (*SetFont)(uint8_t Size, uint8_t Type,
        uint32_t Color);
    void (*SetBg)(uint32_t Color);
	
} TFTLCD_Word_ObjType;


typedef struct 
{
    uint32_t* FontColorList;
    uint8_t*  FontSizeList;
    uint8_t*  FontTypeList;
    uint16_t  XPix;
    uint16_t  YPix;
    uint16_t  Width;
    uint16_t  Height;
    uint8_t   VerticalAlign;
    uint8_t   HorizontalAlign;
    uint8_t   RollMode;
    
} TFTLCD_Words_PropType;


typedef struct
{
    TFTLCD_Word_ObjType   WordObj;
    TFTLCD_Words_PropType WordsProp;
    
    void (*Display)(uint16_t XPix, uint16_t YPix, 
        uint16_t Width, uint16_t Height, void* _Obj);
    void (*SetColorList)(uint16_t Index, uint32_t Color);
    void (*FontSizeList)(uint16_t Index, uint32_t Size);
    void (*FontTypeList)(uint16_t Index, uint32_t Type);
    void (*SetAlign)(uint8_t Ver, uint8_t Hor);
    void (*SetRollMode)(uint8_t Mode);
    
} TFTLCD_Words_ObjType;


void TFTLCD_Word(uint16_t XPix, uint16_t YPix, 
    void* _Obj);

void TFTLCD_Words(uint16_t XPix, uint16_t YPix, 
        uint16_t Width, uint16_t Height, void* _Obj);



#endif
