#ifndef __MODBUS_RTU_PARA_H__
#define __MODBUS_RTU_PARA_H__


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define MODBUS_RTU_PARA_TABLE_SIZE			(1)	// 参数表中的参数个数

/*--------------------------------------类型定义-----------------------------------*/

typedef enum
{
	enValueType_Bit8 = 1,
	enValueType_Bit16 = 2,
	enValueType_Bit32 = 4,
} enValueType_t;


typedef enum
{
	enWriteOnly = 1,
	enReadOnly,
	enReadWrite,
} enReadWrite_t;


typedef struct
{
	uint8_t u8Bit:3;		// @ref enValueType_t
	uint8_t u8Rw:2;			// @ref enReadWrite_t
	uint8_t u8Reserved:3;
} stcParaAttr_t;

typedef struct
{
	uint16_t u16RegAddr;
	
	void * pvPara;
	
	stcParaAttr_t stcParaAttr;
	
	bool (*ParaCallbackFunc)(uint8_t u8Rw, uint32_t * pu32ParaValue);
	
} stcModbusPara_t;

/*----------------------------------------------------------------------------------*/

extern const stcModbusPara_t gastcModbusPara_Table[MODBUS_RTU_PARA_TABLE_SIZE];

const stcModbusPara_t * ModbusPara_Find(uint16_t u16RegAddr, uint16_t * pu16ParaIndex, 
										const stcModbusPara_t * astcParaTable, uint16_t u16ParaTableSize);

#endif
