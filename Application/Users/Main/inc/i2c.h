#ifndef __I2C_H__
#define __I2C_H__


#include "stm32f4xx_gpio.h"
#include "delay.h"

// SDA和SCL需要配置为开漏输出模式

#define I2C_SDA_R()					((GPIOB->IDR & GPIO_Pin_9) != Bit_RESET)

#define I2C_SDA_H()					(GPIOB->BSRR = (uint32_t)GPIO_Pin_9)
#define I2C_SDA_L()					(GPIOB->BSRR = (uint32_t)GPIO_Pin_9 << 16U)

#define I2C_SCL_H()					(GPIOB->BSRR = (uint32_t)GPIO_Pin_8)
#define I2C_SCL_L()					(GPIOB->BSRR = (uint32_t)GPIO_Pin_8 << 16U)

#define I2C_DELAY_US(t)				Delay_Us(t)

#define I2C_DATA_HOLD_TIME_US		(2)
#define I2C_DATA_PREP_TIME_US		(2)

#if (I2C_DATA_HOLD_TIME_US < 2) || (I2C_DATA_PREP_TIME_US < 2)
	#error "Data preparation time or hold time is too short!"
#endif

unsigned char I2C_ReadByte(void);
unsigned char I2C_WriteByte(unsigned char u8Data);

unsigned char I2C_ReadRegister(unsigned char u8Addr, unsigned char u8Reg, unsigned char * au8DataBuff, unsigned short u16Size);
unsigned char I2C_WriteRegister(unsigned char u8Addr, unsigned char u8Reg, unsigned char * au8DataBuff, unsigned short u16Size);

unsigned char I2C_DeviceScan(unsigned char au8AddrTab[], unsigned char u8ScanNum);

#endif
