#ifndef __I2C_H__
#define __I2C_H__


/**
* @include
**/
#include <string.h>
#include "delay.h"
#include "stm32f4xx.h"
#include "sys.h"


/**
*	@define
**/
// I2C为发送模式
#define IIC_Mode_Tx() {GPIOB->MODER &= ~(3<<22);GPIOB->MODER |= 1<<22;}
// I2C为接收模式
#define IIC_Mode_Rx() {GPIOB->MODER &= ~(3<<22);}
// GPIOB引脚映射IIC线
#define IIC_SCL     		PBout(10)	// IIC时钟线
#define IIC_SDA_OUT     PBout(11)	// IIC数据输出线
#define IIC_SDA_IN			PBin(11)	// IIC数据读入线
#define IIC_DATA_SIZE_MAX 32			// IIC接收数据最大长度（字节）


/**
*	@typedef
**/
typedef enum {	True = 1, False = 0 } bool;	// 类型重定义，BOOL类型

#endif
