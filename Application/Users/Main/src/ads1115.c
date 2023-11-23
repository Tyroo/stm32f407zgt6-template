/*******************************************************************************
** 名称：ADS1115.c
** 功能：ADS1115驱动
** 版本：V1.0
** 作者：张嘉良
** 日期：2023年2月15日
*******************************************************************************/

#include <stdio.h>
#include "ads1115.h"


extern uint32_t HAL_GetTick(void);

// ADS1115写入寄存器
static bool ADS1115_WriteREG(uint8_t u8Addr, uint8_t u8Reg, uint16_t u16Data)
{
	u16Data = (u16Data >> 8) | (u16Data << 8);
	// 发送命令
	return (!ADS1115_WRITE_REG(u8Addr, u8Reg, (uint8_t *)&u16Data, 2));
}


// ADS1115读取寄存器
static bool ADS1115_ReadREG(uint8_t u8Addr, uint8_t u8Reg, uint16_t *pu16Buff)
{
	uint8_t bRet;

	// 发送命令
	bRet = ADS1115_READ_REG(u8Addr, u8Reg, (uint8_t *)pu16Buff, 2);
	
	*pu16Buff = ((*pu16Buff) >> 8) | ((*pu16Buff) << 8);
	
	return (!bRet);
}


// 配置ADS1115
bool ADS1115_Configure(stcADS1115_Handler_t *pstcHandler)
{
	if (pstcHandler == NULL)
	{
		return false;
	}

	return (bool)(ADS1115_WriteREG(pstcHandler->u8DeviceAddr, ADS1115_CONF_REG, pstcHandler->uniConfigReg.u16All));
}


// 设置电压比较寄存器的值
bool ADS1115_SetComparatorValue(stcADS1115_Handler_t *pstcHandler, uint16_t u16HiThresh, uint16_t u16LoThresh)
{
	bool bRet;
	
	if (pstcHandler->u8ConverReadyOutputEnable)
	{
		u16HiThresh = 0x8000;
		u16LoThresh = 0x7FFF;
	}
	
	bRet = ADS1115_WriteREG(pstcHandler->u8DeviceAddr, ADS1115_LOTHRESH_REG, u16LoThresh);
	bRet = ADS1115_WriteREG(pstcHandler->u8DeviceAddr, ADS1115_HITHRESH_REG, u16HiThresh);
	
	return bRet;
}


// 开启一次采样
bool ADS1115_ConverStart(stcADS1115_Handler_t *pstcHandler, uint8_t u8Channel)
{
	// 配置ADS1115
	pstcHandler->uniConfigReg.stcBit.MUX = pstcHandler->penChannelBuff[u8Channel];	// 通道设置
	pstcHandler->uniConfigReg.stcBit.OS = enADS1115_CfgOS_STATCONV;					// 开启ADC采样
	
	return (ADS1115_WriteREG(pstcHandler->u8DeviceAddr, ADS1115_CONF_REG, pstcHandler->uniConfigReg.u16All));
}


// 从总线上获取ADS1115警报响应
uint8_t ADS1115_GetSMBusAlertRespone(void)
{
	ADS1115_WRITE_BYTE(ADS1115_R_SMBUS_ALERT_CMD);
	return ADS1115_READ_BYTE();
}


uint16_t ADS1115_GetConfig(stcADS1115_Handler_t *pstcHandler)
{
	uint16_t u16ConfigReg = 0;
	
	ADS1115_ReadREG(pstcHandler->u8DeviceAddr, ADS1115_CONF_REG, &u16ConfigReg);
	
	return u16ConfigReg;
}


// 获取当前转换完成通道的采样值
uint16_t ADS1115_GetValue(stcADS1115_Handler_t *pstcHandler)
{
	uint16_t u16ADValue = 0;
	
	ADS1115_ReadREG(pstcHandler->u8DeviceAddr, ADS1115_CONV_REG, &u16ADValue);
	
	u16ADValue ^= 0x8000;
	
	return u16ADValue;
}


// ADS1115进入断电模式
void ADS1115_Reset(void)
{
	ADS1115_WRITE_BYTE(0x00);
	ADS1115_WRITE_BYTE(ADS1115_RESET_CMD);
}


// ADS1115的配置寄存器设置缺省值
void ADS1115_DeInit(uniADS1115_ConfReg_t *puniConfigReg)
{
	if (puniConfigReg != NULL)
	{
		puniConfigReg->stcBit.COMP_QUE = enADS1115_CfgCOMPQUE_DISABLE;
		puniConfigReg->stcBit.COMP_LAT = enADS1115_CfgCOMPLAT_NONLATCH;
		puniConfigReg->stcBit.COMP_POL = enADS1115_CfgCOMPPOL_LOW;
		puniConfigReg->stcBit.COMP_MODE = enADS1115_CfgCOMPMODE_TRADITIONAL;
		puniConfigReg->stcBit.DR = enADS1115_CfgDR_SPS128;
		puniConfigReg->stcBit.MODE = enADS1115_CfgMODE_SINGLE;
		puniConfigReg->stcBit.PGA = enADS1115_CfgPGA_FSR2;
		puniConfigReg->stcBit.MUX = enADS1115_CfgMUX_AIN0P_AIN1N;
		puniConfigReg->stcBit.OS = enADS1115_CfgOS_STATCONV;
	}
}
