#ifndef __TFTLCD_CONF_H__
#define __TFTLCD_CONF_H__

#include "stm32f4xx.h"


// TFTLCD����/���ݶ�д�ṹ��
typedef struct {
	
	uint16_t RWCD_REG;
	uint16_t RWCD_RAM;
	
} TFTLCD_RWCD;


// TFTLCD���
typedef struct {
	
	uint8_t CMD_Read_ID;			// ��ȡIDָ��
	
	uint8_t CMD_Write_ScanDir;// ����ɨ�跽��
	
	uint8_t CMD_Write_XPos;	  // X��������ָ��
	uint8_t CMD_Write_YPos;		// Y��������ָ��
	
	uint8_t CMD_Write_ColorCode;
	uint8_t CMD_Read_ColorCode;
	
} TFTLCD_CMD;


// TFTLCD���ýṹ��
typedef struct {
	
	uint16_t TFTLCD_Width;	// ��Ļ���
	uint16_t TFTLCD_Height;	// ��Ļ�߶�
	uint16_t TFTLCD_Id;			// ��ĻIC�ͺ�
	uint8_t TFTLCD_Dir;			// ��Ļ��ʾ����
	TFTLCD_CMD TFTLCD_Cmd;	// TFTLCD��ĻIC��ָ�
	
} TFTLCD_Config;


#endif
