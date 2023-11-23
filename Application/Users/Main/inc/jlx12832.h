#ifndef __JLX12832_H__
#define __JLX12832_H__


#define JLX12832_SCREEN_WIDTH	(128)
#define JLX12832_SCREEN_HEIGTH	(32)

#define LCM_RS(x)		(void)(x)
#define LCM_RST(x)		(void)(x)
#define LCM_BLK_EN(x)	(void)(x)

typedef enum
{
	enJLX12832_Color_Black = 0x00,
	enJLX12832_Color_White = 0xFF,
} enJLX12832_Color_t;

typedef struct
{
	const unsigned char u8Width;
	const unsigned char u8Height;
	unsigned char u8Refresh;
	unsigned char au8DisplayBuff[JLX12832_SCREEN_HEIGTH / 8][JLX12832_SCREEN_WIDTH];
	
} stcJLX12832_ShowAttr_t;

typedef union
{
	unsigned int u32PageWord;
	unsigned char au8PageByte[JLX12832_SCREEN_HEIGTH / 8];
} uniJLX12832_PagePix_t;

extern stcJLX12832_ShowAttr_t gstcJLX12832_ShowAttr;

void JLX12832_Init(void);
void JLX12832_Refresh(void);
void JLX12832_SetDisplayEnable(unsigned char u8En);
void JLX12832_SetContrastRatio(unsigned char u8Ratio);
void JLX12832_SetScanDirection(unsigned char u8HDir, unsigned char u8VDir);
void JLX12832_SetPixelDirection(unsigned char u8Dir);
void JLX12832_SetPower(unsigned char u8Power);
void JLX12832_ClearScreen(enJLX12832_Color_t enColor);

#if 0
void JLX12832_DrawString(unsigned char u8Xpoint, unsigned char u8Ypoint, const char *ps8Str,
						 const FONT_t *pstcFont, enJLX12832_Color_t enColor);
void JLX12832_DrawNumber(unsigned char u8Xpoint, unsigned char u8Ypoint, const char *ps8Str,
						 const FONT_t *pstcFont, enJLX12832_Color_t enColor);
#endif

void JLX12832_DrawRectFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width, unsigned char u8Hieght,
						   enJLX12832_Color_t enColor);
void JLX12832_DrawRect(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width, unsigned char u8Hieght,
					   enJLX12832_Color_t enColor);
void JLX12832_DrawRightTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Height,
									enJLX12832_Color_t enColor);
void JLX12832_DrawLeftTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Height,
								   enJLX12832_Color_t enColor);
void JLX12832_DrawUpTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width,
								 enJLX12832_Color_t enColor);
void JLX12832_DrawDownTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width,
								   enJLX12832_Color_t enColor);
#if 0
void JLX12832_DrawPicture(unsigned char u8Xpoint, unsigned char u8Ypoint, const IMAGE_t *pstcImage,
						  enJLX12832_Color_t enColor);
#endif

void JLX12832_DrawBattery(unsigned char u8Xpoint, unsigned char u8YPoint, unsigned char u8Width, unsigned char u8Height, 
						  float fCapacity, enJLX12832_Color_t enColor);

#endif
