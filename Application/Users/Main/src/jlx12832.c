/*******************************************************************************
** 名称：JLX12832.c
** 功能：JLX12832驱动
** 版本：V1.0
** 作者：张嘉良
** 日期：2023年9月13日
*******************************************************************************/

#include <string.h>
#include "spi.h"
#include "delay.h"
#include "jlx12832.h"


stcJLX12832_ShowAttr_t gstcJLX12832_ShowAttr = 
{
	JLX12832_SCREEN_WIDTH,
	JLX12832_SCREEN_HEIGTH,
};


// 写指令到LCD模块
static void JLX12832_SendCmd(unsigned char u8Cmd) 
{
	LCM_RS(0);
	SPI_I2S_SendData(SPI1, u8Cmd);
}


// 写数据到LCD模块
static void JLX12832_SendData(unsigned char pu8Buff[], unsigned short u16Size)
{
	LCM_RS(1);

	for(unsigned short u16Index = 0; u16Index < u16Size; u16Index++)
	{
		SPI_I2S_SendData(SPI1, pu8Buff[u16Index]);
	}
}


// 初始化LCD显示屏
void JLX12832_Init(void)
{
	LCM_RST(0); 			// 硬复位
	Delay_Ms(20);
	LCM_RST(1);
	
	JLX12832_SendCmd(0xE2); // 软复位
	JLX12832_SendCmd(0x2C); // 升压步聚1
	Delay_Ms(5);
	JLX12832_SendCmd(0x2E); // 升压步聚2
	Delay_Ms(5);
	JLX12832_SendCmd(0x2F); // 升压步聚3
	Delay_Ms(5);
	JLX12832_SendCmd(0x21); // 粗调对比度，可设置范围 20～27
	JLX12832_SendCmd(0x81); // 微调对比度
	JLX12832_SendCmd(0x0A); // 微调对比度的值，可设置范围 0～63
	JLX12832_SendCmd(0xA2); // 1/9 偏压比（bias）
	JLX12832_SendCmd(0xC8); // 行扫描顺序：从下到上
	JLX12832_SendCmd(0xA1); // 列扫描顺序：从右到左
	JLX12832_SendCmd(0x40); // 起始行：从第一行开始
	JLX12832_SendCmd(0xAF); // 开显示
	JLX12832_ClearScreen(enJLX12832_Color_White);
}


void JLX12832_SetContrastRatio(unsigned char u8Ratio)
{
	u8Ratio *= 0.64f;
	JLX12832_SendCmd(0x81);
	JLX12832_SendCmd(u8Ratio);
}


void JLX12832_SetScanDirection(unsigned char u8HDir, unsigned char u8VDir)
{
	JLX12832_SendCmd(0xA0 | u8HDir);			// 设置水平扫描顺序
	JLX12832_SendCmd((0xC0 | (u8VDir << 3)));	// 设置垂直扫描顺序
}


void JLX12832_SetPixelDirection(unsigned char u8Dir)
{
	JLX12832_SendCmd(0xA6 | u8Dir);			// 正显/反显		
}


void JLX12832_SetDisplayEnable(unsigned char u8En)
{
	LCM_BLK_EN(u8En);
	JLX12832_SendCmd(0xAE | (u8En & 0x01)); // 开/关显示
}


void JLX12832_SetPower(unsigned char u8Power)
{
	// 退出正常模式，进入节能模式
	if (u8Power == 0)
	{
		JLX12832_SendCmd(0xAE);
		JLX12832_SendCmd(0xA5);
	}
	// 退出节能模式，进入正常模式
	else
	{
		JLX12832_SendCmd(0xA4);
		JLX12832_SendCmd(0xAF);
	}
}


void JLX12832_Refresh(void)
{
	if (gstcJLX12832_ShowAttr.u8Refresh == 0)
	{
		return;
	}
	
	unsigned char u8PageIndex;
	unsigned char u8PageNum = gstcJLX12832_ShowAttr.u8Height >> 3;
	
#if 1
	
	unsigned char u8PageCmd = 0xB3;		// 屏幕从下到上扫描的起始页
	
	for(u8PageIndex = 0; u8PageIndex < u8PageNum; u8PageIndex++)
	{
		JLX12832_SendCmd(u8PageCmd--);	// 屏幕从下到上扫描
		JLX12832_SendCmd(0x10);
		JLX12832_SendCmd(0x04); 		// 屏幕从右到左扫描
		JLX12832_SendData(gstcJLX12832_ShowAttr.au8DisplayBuff[u8PageIndex], gstcJLX12832_ShowAttr.u8Width);
	}
	
#else
	
	unsigned char u8PageCmd = 0xB0;		// 屏幕从上到下扫描的起始页
	
	for(u8PageIndex = 0; u8PageIndex < u8PageNum; u8PageIndex++)
	{
		JLX12832_SendCmd(u8PageCmd++);	// 屏幕从上到下扫描
		JLX12832_SendCmd(0x10);
		JLX12832_SendCmd(0x00); 		// 屏幕从左到右扫描
		JLX12832_SendData(gstcJLX12832_ShowAttr.au8DisplayBuff[u8PageIndex], gstcJLX12832_ShowAttr.u8Width);
	}
	
#endif
	
	gstcJLX12832_ShowAttr.u8Refresh = 0;
}


void JLX12832_ClearScreen(enJLX12832_Color_t enColor)
{
	memset(gstcJLX12832_ShowAttr.au8DisplayBuff, ~enColor, sizeof(gstcJLX12832_ShowAttr.au8DisplayBuff));
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}

#if 0
void JLX12832_DrawString(unsigned char u8Xpoint, unsigned char u8Ypoint, const char *ps8Str,
						 const FONT_t *pstcFont, enJLX12832_Color_t enColor)
{
	unsigned char * pu8FontPtr;
	unsigned int u32FontUnicode, u32FontIndex, u32FontSize;
	unsigned char u8FontOffset, u8FontPage, u8FontPageIndex, u8FontWidthIndex, u8FontWidth, u8FontHeight = pstcFont->Height;
	
	unsigned int u32VPixelFont;
	unsigned int u32VPixel0Mask = (0xFFFFFFFFU << (gstcJLX12832_ShowAttr.u8Height - u8FontHeight));
	
	uniJLX12832_PagePix_t uniPagePix;
	
	u32VPixel0Mask >>= u8Ypoint;
	
	u32FontSize = strlen(ps8Str);
	u8FontPage = (u8FontHeight >> 3) + ((u8FontHeight % 8) != 0);
	
	while(u32FontSize > 0)
	{
		if (*ps8Str & 0x80)
		{
			u8FontWidth = pstcFont->UnicodeWide;
			pu8FontPtr = pstcFont->UnicodeFontPtr;
			u32FontUnicode = Font_Utf8ToUnicode((const unsigned char *)(ps8Str), &u8FontOffset);
			u32FontIndex = Font_UnicodeFontFind(u32FontUnicode) * pstcFont->UnicodeByteSize;
		}
		else
		{
			u8FontWidth = pstcFont->Wide;
			pu8FontPtr = pstcFont->FontPtr;
			u8FontOffset = 1;
			u32FontIndex = (*ps8Str - ' ') * pstcFont->ByteSize;
		}
		
		u32FontSize -= u8FontOffset;
		ps8Str += u8FontOffset;
		
		for(u8FontWidthIndex = 0; u8FontWidthIndex < u8FontWidth; u8FontWidthIndex++)
		{
			u32VPixelFont = 0;
			
			for(u8FontPageIndex = 0; u8FontPageIndex < u8FontPage; u8FontPageIndex++)
			{
				u32VPixelFont <<= 8;
				u32VPixelFont |= (unsigned int)(pu8FontPtr[u8FontPage * u8FontWidthIndex + u32FontIndex + u8FontPageIndex] ^ enColor);
			}
			
			u32VPixelFont <<= (gstcJLX12832_ShowAttr.u8Height - (u8FontPage << 3));
			u32VPixelFont >>= u8Ypoint;
			
			uniPagePix.au8PageByte[3] = gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint];
			uniPagePix.au8PageByte[2] = gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint];
			uniPagePix.au8PageByte[1] = gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint];
			uniPagePix.au8PageByte[0] = gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint];
			
			uniPagePix.u32PageWord &= ~u32VPixel0Mask;
			uniPagePix.u32PageWord |= (u32VPixelFont & u32VPixel0Mask);
			
			gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint] = uniPagePix.au8PageByte[3];
			gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint] = uniPagePix.au8PageByte[2];
			gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint] = uniPagePix.au8PageByte[1];
			gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint++] = uniPagePix.au8PageByte[0];
		}
	}
	
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}
#endif


#if 0
void JLX12832_DrawNumber(unsigned char u8Xpoint, unsigned char u8Ypoint, const char *ps8Str, 
						 const FONT_t *pstcFont, enJLX12832_Color_t enColor)
{
	unsigned char * pu8FontPtr;
	unsigned int u32FontIndex, u32FontSize;
	unsigned char u8FontPage, u8FontPageIndex, u8FontWidthIndex, u8FontWidth, u8FontHeight = pstcFont->Height;
	
	unsigned int u32VPixelFont;
	unsigned int u32VPixel0Mask = (0xFFFFFFFFU << (gstcJLX12832_ShowAttr.u8Height - u8FontHeight));
	
	uniJLX12832_PagePix_t uniPagePix;
	
	u32VPixel0Mask >>= u8Ypoint;
	
	u32FontSize = strlen(ps8Str);
	u8FontPage = (u8FontHeight >> 3) + ((u8FontHeight % 8) != 0);
	
	while(u32FontSize > 0)
	{
		pu8FontPtr = pstcFont->FontPtr;
		u32FontIndex = (*ps8Str - '-') * pstcFont->ByteSize;
		u32FontSize--;
		u8FontWidth = (*ps8Str != '.') ? (pstcFont->Wide) : ((pstcFont->Wide >> 1) + 1);
		
		ps8Str++;
		
		for(u8FontWidthIndex = 0; u8FontWidthIndex < u8FontWidth; u8FontWidthIndex++)
		{
			u32VPixelFont = 0;
			
			for(u8FontPageIndex = 0; u8FontPageIndex < u8FontPage; u8FontPageIndex++)
			{
				u32VPixelFont <<= 8;
				u32VPixelFont |= (unsigned int)(pu8FontPtr[u8FontPage * u8FontWidthIndex + u32FontIndex + u8FontPageIndex] ^ enColor);
			}

			u32VPixelFont <<= (gstcJLX12832_ShowAttr.u8Height - (u8FontPage << 3));
			u32VPixelFont >>= u8Ypoint;
			
			uniPagePix.au8PageByte[3] = gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint];
			uniPagePix.au8PageByte[2] = gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint];
			uniPagePix.au8PageByte[1] = gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint];
			uniPagePix.au8PageByte[0] = gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint];
			
			uniPagePix.u32PageWord &= ~u32VPixel0Mask;
			uniPagePix.u32PageWord |= (u32VPixelFont & u32VPixel0Mask);
			
			gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint] = uniPagePix.au8PageByte[3];
			gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint] = uniPagePix.au8PageByte[2];
			gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint] = uniPagePix.au8PageByte[1];
			gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint++] = uniPagePix.au8PageByte[0];
		}
	}
	
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}
#endif


void JLX12832_DrawRectFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width, unsigned char u8Hieght,
						   enJLX12832_Color_t enColor)
{
	unsigned int u32VPixel1Mask = enColor ? 0x00000000U : 0xFFFFFFFFU;
	unsigned int u32VPixel0Mask = 0xFFFFFFFFU;
	
	uniJLX12832_PagePix_t uniPagePix;
	
	u32VPixel0Mask <<= (gstcJLX12832_ShowAttr.u8Height - u8Hieght);
	u32VPixel0Mask >>= (u8Ypoint);
	u32VPixel1Mask = u32VPixel1Mask & u32VPixel0Mask;
	u32VPixel0Mask = ~u32VPixel0Mask;
	
	while(u8Width--)
	{
		uniPagePix.au8PageByte[3] = gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint];
		uniPagePix.au8PageByte[2] = gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint];
		uniPagePix.au8PageByte[1] = gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint];
		uniPagePix.au8PageByte[0] = gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint];
		
		uniPagePix.u32PageWord &= u32VPixel0Mask;
		uniPagePix.u32PageWord |= u32VPixel1Mask;
		
		gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint] = uniPagePix.au8PageByte[3];
		gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint] = uniPagePix.au8PageByte[2];
		gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint] = uniPagePix.au8PageByte[1];
		gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint++] = uniPagePix.au8PageByte[0];
	}
	
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


void JLX12832_DrawRect(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width, unsigned char u8Hieght,
					   enJLX12832_Color_t enColor)
{
	JLX12832_DrawRectFill(u8Xpoint,					u8Ypoint,					u8Width,			1, 				enColor);	  
	JLX12832_DrawRectFill(u8Xpoint + u8Width - 1,	u8Ypoint,					1,					u8Hieght, 		enColor);  
	JLX12832_DrawRectFill(u8Xpoint,					u8Ypoint + (u8Hieght - 1),	u8Width,			1, 				enColor);
	JLX12832_DrawRectFill(u8Xpoint,					u8Ypoint,					1,					u8Hieght, 		enColor);
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


void JLX12832_DrawRightTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Height,
									enJLX12832_Color_t enColor)
{
	unsigned short u16HalfHeight = (u8Height >> 1) + 1;
	unsigned short u16StartYpoint = u8Ypoint + u16HalfHeight - ((u8Height % 2) == 0);
	
	for(unsigned short i = 0; i < u16HalfHeight; i++)
	{
		JLX12832_DrawRectFill(u8Xpoint + i, u16StartYpoint - u16HalfHeight + i + 1, 1, (u16HalfHeight - i - 1) * 2 + 1, enColor);
	}
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


void JLX12832_DrawLeftTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Height,
								   enJLX12832_Color_t enColor)
{
	unsigned short u16HalfHeight = (u8Height >> 1) + 1;
	unsigned short u16StartYpoint = u8Ypoint + u16HalfHeight - ((u8Height % 2) == 0);
	
	for(unsigned short i = 0; i < u16HalfHeight; i++)
	{
		JLX12832_DrawRectFill(u8Xpoint + i, u16StartYpoint - i, 1, 2 * i + 1, enColor);
	}
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


void JLX12832_DrawUpTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width,
								 enJLX12832_Color_t enColor)
{
	unsigned short u16Width = u8Width >> 1;
	
	for(unsigned short i = 0; i <= u16Width; i++)
	{
		JLX12832_DrawRectFill((u8Xpoint + i), (u8Ypoint + u16Width - i), (u8Width - 2 * i), 1, enColor);
	}
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


void JLX12832_DrawDownTriangleFill(unsigned char u8Xpoint, unsigned char u8Ypoint, unsigned char u8Width,
								   enJLX12832_Color_t enColor)
{
	unsigned short u16Width = u8Width >> 1;
	
	for(unsigned short i = 0; i <= u16Width; i++)
	{
		JLX12832_DrawRectFill((u8Xpoint + i), (u8Ypoint + i), u8Width - 2 * i, 1, enColor);
	}
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}


#if 0
void JLX12832_DrawPicture(unsigned char u8Xpoint, unsigned char u8Ypoint, const IMAGE_t *pstcImage,
						  enJLX12832_Color_t enColor)
{
	if (pstcImage->Height == 0) return;
	
	const unsigned char au8EdgeClip[8] = { 0xFF, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
	
	unsigned char u8FontWidthIndex, u8FontPagePixel, u8FontPageEdgePixel = pstcImage->Height % 8, u8FontWidth = pstcImage->Wide;
	unsigned char u8FontPageIndex, u8FontPage = (pstcImage->Height >> 3) + (u8FontPageEdgePixel != 0);
	unsigned int u32VPixelImage;
	unsigned int u32VPixel0Mask = (0xFFFFFFFFU << (gstcJLX12832_ShowAttr.u8Height - pstcImage->Height));
	
	uniJLX12832_PagePix_t uniPagePix;
	
	u32VPixel0Mask >>= (u8Ypoint);
	u32VPixel0Mask = ~u32VPixel0Mask;
	
	for(u8FontWidthIndex = 0; u8FontWidthIndex < u8FontWidth; u8FontWidthIndex++)
	{
		u32VPixelImage = 0;
		
		for(u8FontPageIndex = 0; u8FontPageIndex < u8FontPage - 1; u8FontPageIndex++)
		{
			u32VPixelImage <<= 8;
			u32VPixelImage |= (unsigned int)(pstcImage->ImagePtr[u8FontPage * u8FontWidthIndex + u8FontPageIndex] ^ enColor);
		}
		
		u8FontPagePixel = pstcImage->ImagePtr[u8FontPage * u8FontWidthIndex + u8FontPageIndex] ^ enColor;
		u8FontPagePixel &= au8EdgeClip[u8FontPageEdgePixel];
		
		u32VPixelImage <<= 8;
		u32VPixelImage |= ((unsigned int)(u8FontPagePixel));
		u32VPixelImage <<= (gstcJLX12832_ShowAttr.u8Height - (u8FontPage << 3));
		u32VPixelImage >>= (u8Ypoint);
		
		uniPagePix.au8PageByte[3] = gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint];
		uniPagePix.au8PageByte[2] = gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint];
		uniPagePix.au8PageByte[1] = gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint];
		uniPagePix.au8PageByte[0] = gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint];
		
		uniPagePix.u32PageWord &= u32VPixel0Mask;
		uniPagePix.u32PageWord |= u32VPixelImage;
		
		gstcJLX12832_ShowAttr.au8DisplayBuff[0][u8Xpoint] = uniPagePix.au8PageByte[3];
		gstcJLX12832_ShowAttr.au8DisplayBuff[1][u8Xpoint] = uniPagePix.au8PageByte[2];
		gstcJLX12832_ShowAttr.au8DisplayBuff[2][u8Xpoint] = uniPagePix.au8PageByte[1];
		gstcJLX12832_ShowAttr.au8DisplayBuff[3][u8Xpoint++] = uniPagePix.au8PageByte[0];
	}
	gstcJLX12832_ShowAttr.u8Refresh = 1;
}
#endif


void JLX12832_DrawBattery(unsigned char u8Xpoint, unsigned char u8YPoint, 
						  unsigned char u8Width, unsigned char u8Height, 
						  float fCapacity, enJLX12832_Color_t enColor)
{
	unsigned char u8LeftBlockWidth, u8RightBlockWidth, u8MiddleBlockWidth, u8CapacityBlockWidth;
	unsigned char u8LeftBlockHeight, u8RightBlockHeight, u8MiddleBlockHeight;
	
	if (fCapacity > 100.0f) fCapacity = 100.0f;
	
	u8LeftBlockWidth = (u8Width / 10) & 0x03;
	u8RightBlockWidth = u8Width - u8LeftBlockWidth;
	
	u8MiddleBlockWidth = (u8RightBlockWidth > 4) ? u8RightBlockWidth - 4 : 0;
	
	u8CapacityBlockWidth = u8MiddleBlockWidth * fCapacity / 100;
	
	u8LeftBlockHeight = (u8Height >> 1) - ((u8LeftBlockHeight % 2) && ((u8Height % 2) == 0));
	
	u8RightBlockHeight = u8Height;
	
	u8MiddleBlockHeight = (u8Height > 4) ? u8RightBlockHeight - 4 : 0;
	
	JLX12832_DrawRectFill(u8Xpoint, u8YPoint + ((u8Height - u8LeftBlockHeight) >> 1), u8LeftBlockWidth, u8LeftBlockHeight, enColor);
	
	JLX12832_DrawRect(u8Xpoint + u8LeftBlockWidth, u8YPoint, u8RightBlockWidth, u8RightBlockHeight, enColor);
	
	JLX12832_DrawRectFill(u8Xpoint + u8LeftBlockWidth + 2 + (u8MiddleBlockWidth - u8CapacityBlockWidth), u8YPoint + 2, 
						  u8CapacityBlockWidth, u8MiddleBlockHeight, enColor);
}
