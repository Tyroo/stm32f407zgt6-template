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
// I2CΪ����ģʽ
#define IIC_Mode_Tx() {GPIOB->MODER &= ~(3<<22);GPIOB->MODER |= 1<<22;}
// I2CΪ����ģʽ
#define IIC_Mode_Rx() {GPIOB->MODER &= ~(3<<22);}
// GPIOB����ӳ��IIC��
#define IIC_SCL     		PBout(10)	// IICʱ����
#define IIC_SDA_OUT     	PBout(11)	// IIC���������
#define IIC_SDA_IN			PBin(11)	// IIC���ݶ�����
#define IIC_DATA_SIZE_MAX 	32			// IIC����������󳤶ȣ��ֽڣ�


/**
*	@typedef
**/
typedef enum {	False, True } bool;	// �����ض��壬BOOL����


static void IIC_TxReply(uint8_t Res);
static uint8_t IIC_RxReply(void);

static void IIC_Stop(void);
static void IIC_Start(void);

void IIC_Send_Byte(uint8_t Data);
uint8_t IIC_Read_Byte(uint8_t IsAck);

bool IIC_Send_String(char *Str, uint8_t DeviceAddr, uint16_t RegisterAddr);
bool IIC_Read_String(uint8_t DeviceAddr, uint16_t RegisterAddr, uint8_t RxLen);

#endif
