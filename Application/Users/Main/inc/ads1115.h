#ifndef __ADS1115_H__
#define __ADS1115_H__


#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"


#define ADS1115_MAX_AD_VALUE		(65536)

#define ADS1115_WRITE_REG			I2C_WriteRegister
#define ADS1115_READ_REG			I2C_ReadRegister
#define ADS1115_WRITE_BYTE			I2C_WriteByte
#define ADS1115_READ_BYTE			I2C_ReadByte

/* ADS1115命令 */
#define ADS1115_R_SMBUS_ALERT_CMD	(0x19)	// 读SMBus警报状态命令
#define ADS1115_RESET_CMD			(0x06)	// ADC重置命令

/* ADS1115寄存器 */
#define ADS1115_CONV_REG			(0x00)	// ADC采样寄存器
#define ADS1115_CONF_REG			(0x01)	// ADC配置寄存器
#define ADS1115_LOTHRESH_REG		(0x02)	// ADCLo_thresh寄存器
#define ADS1115_HITHRESH_REG		(0x03)	// ADCHi_thresh寄存器


/* Config register field structural */

typedef union 
{
	uint16_t u16All;
	struct
	{
		uint16_t COMP_QUE:2;
		uint16_t COMP_LAT:1;
		uint16_t COMP_POL:1;
		uint16_t COMP_MODE:1;
		uint16_t DR:3;
		uint16_t MODE:1;
		uint16_t PGA:3;
		uint16_t MUX:3;	// (ADS1115 only)
		uint16_t OS:1;
	} stcBit;
} uniADS1115_ConfReg_t;

/* Config register field enumerator values */

// Config Register Field: OS
typedef enum
{
	enADS1115_CfgOS_NOEFFECT,
	enADS1115_CfgOS_STATCONV
} enADS1115_CfgOS_t;

// Config Register Field: MUX (ADS1115 only)
typedef enum
{
	enADS1115_CfgMUX_AIN0P_AIN1N,
	enADS1115_CfgMUX_AIN0P_AIN3N,
	enADS1115_CfgMUX_AIN1P_AIN3N,
	enADS1115_CfgMUX_AIN2P_AIN3N,
	enADS1115_CfgMUX_AIN0P_GNDN,
	enADS1115_CfgMUX_AIN1P_GNDN,
	enADS1115_CfgMUX_AIN2P_GNDN,
	enADS1115_CfgMUX_AIN3P_GNDN,
	enADS1115_CfgMUX_Bottom,
} enADS1115_CfgMUX_t;

// Config Register Field: PGA
typedef enum
{
	enADS1115_CfgPGA_FSR0,	// 采样电压范围：±6.144 V
	enADS1115_CfgPGA_FSR1,	// 采样电压范围：±4.096 V
	enADS1115_CfgPGA_FSR2,	// 采样电压范围：±2.048 V
	enADS1115_CfgPGA_FSR3,	// 采样电压范围：±1.024 V
	enADS1115_CfgPGA_FSR4,	// 采样电压范围：±0.512 V
	enADS1115_CfgPGA_FSR5,	// 采样电压范围：±0.256 V
	enADS1115_CfgPGA_FSR6,	// 采样电压范围：±0.256 V
	enADS1115_CfgPGA_FSR7	// 采样电压范围：±0.256 V
} enADS1115_CfgPGA_t;

// Config Register Field: MODE
typedef enum
{
	enADS1115_CfgMODE_CONTINUE,
	enADS1115_CfgMODE_SINGLE
} enADS1115_CfgMODE_t;

// Config Register Field: DR
typedef enum
{
	enADS1115_CfgDR_SPS8,
	enADS1115_CfgDR_SPS16,
	enADS1115_CfgDR_SPS32,
	enADS1115_CfgDR_SPS64,
	enADS1115_CfgDR_SPS128,
	enADS1115_CfgDR_SPS250,
	enADS1115_CfgDR_SPS475,
	enADS1115_CfgDR_SPS860
} enADS1115_CfgDR_t;

// Config Register Field: COMP_MODE
typedef enum
{
	enADS1115_CfgCOMPMODE_TRADITIONAL,
	enADS1115_CfgCOMPMODE_WINDOW
} enADS1115_CfgCOMPMODE_t;

// Config Register Field: COMP_POL
typedef enum
{
	enADS1115_CfgCOMPPOL_LOW,
	enADS1115_CfgCOMPPOL_HIGH
} enADS1115_CfgCOMPPOL_t;

// Config Register Field: COMP_LAT
typedef enum
{
	enADS1115_CfgCOMPLAT_NONLATCH,
	enADS1115_CfgCOMPLAT_LATCH
} enADS1115_CfgCOMPLAT_t;

// Config Register Field: COMP_QUE
typedef enum
{
	enADS1115_CfgCOMPQUE_ONE,
	enADS1115_CfgCOMPQUE_TWO,
	enADS1115_CfgCOMPQUE_FOUR,
	enADS1115_CfgCOMPQUE_DISABLE
} enADS1115_CfgCOMPQUE_t;


typedef struct
{
	uint8_t u8DeviceAddr;
	uint8_t u8ConverReadyOutputEnable;
	uniADS1115_ConfReg_t uniConfigReg;
	const enADS1115_CfgMUX_t * penChannelBuff;
} stcADS1115_Handler_t;


void ADS1115_Reset(void);
void ADS1115_Init(void);
void ADS1115_DeInit(uniADS1115_ConfReg_t *puniConfigReg);

bool ADS1115_Configure(stcADS1115_Handler_t *pstcHandler);
bool ADS1115_ConverStart(stcADS1115_Handler_t *pstcHandler, uint8_t u8Channel);
bool ADS1115_SetComparatorValue(stcADS1115_Handler_t *pstcHandler, uint16_t u16HiThresh, uint16_t u16LoThresh);

void ADS1115_ConverReadyProcess(stcADS1115_Handler_t *pstcHandler);
void ADS1115_ConverReadyCallback(stcADS1115_Handler_t *pstcHandler);

uint8_t ADS1115_GetSMBusAlertRespone(void);
uint16_t ADS1115_GetValue(stcADS1115_Handler_t *pstcHandler);
uint16_t ADS1115_GetConfig(stcADS1115_Handler_t *pstcHandler);

#endif
