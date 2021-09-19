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
#define IIC_SDA_OUT     PBout(11)	// IIC���������
#define IIC_SDA_IN			PBin(11)	// IIC���ݶ�����
#define IIC_DATA_SIZE_MAX 32			// IIC����������󳤶ȣ��ֽڣ�


/**
*	@typedef
**/
typedef enum {	True = 1, False = 0 } bool;	// �����ض��壬BOOL����

#endif
