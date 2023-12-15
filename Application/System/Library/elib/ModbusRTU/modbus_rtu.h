#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#include <stdint.h>
#include <string.h>

#include "modbus_rtu_para.h"


#define MODBUS_RTU_GET_TICK()		0	//HAL_GetTick()

#define MODBUS_RTU_SLAVE_ENABLE		(1)
#define MODBUS_RTU_MASTER_ENABLE	(1)

#define MODBUS_RTU_PACKETS_SIZE		(10)


typedef struct
{
	uint16_t u16PacketOffset;
	uint16_t u16PacketSize;
	uint16_t u16PacketTimeout;
} stcModbusPacket_t;


typedef struct
{
	uint8_t u8State;
	
	uint8_t u8LocalAddr;		// 本机地址
	uint8_t u8RemoteAddr;		// 远端地址
	
#if MODBUS_RTU_MASTER_ENABLE
	
	uint8_t u8PacketIndex;
	uint8_t u8PacketBeginIndex;
	uint8_t u8PacketEndIndex;
	
	stcModbusPacket_t astcPackets[MODBUS_RTU_PACKETS_SIZE];
	
#endif
	
	uint32_t u32TimeoutStartTime;
	uint16_t u16Timeout;
	uint16_t u16ParaTableSize;
	
	uint16_t u16SendDataLength;
	uint16_t u16RecvDataLength;
	
	uint8_t * au8SendDataBuff;
	uint8_t * au8RecvDataBuff;
	
	uint8_t (*Recv)(uint8_t ** au8Data, uint16_t * pu16Size);
	uint8_t (*Send)(uint8_t * au8Data, uint16_t * pu16Size);
	
	uint16_t (*GetCRC16)(uint8_t * au8Data, uint16_t u16Size);
	
	const stcModbusPara_t * pstcParaTable;
	
} stcModbusRTU_Handler_t;


typedef enum
{
	enModbusRTU_State_Ok,
	enModbusRTU_State_CmdErr,
	enModbusRTU_State_RegErr,
	enModbusRTU_State_CrcErr,
	enModbusRTU_State_DataErr = 0x11,
	enModbusRTU_State_Timeout,
	enModbusRTU_State_MBusy,
} enModbusRTU_State_t;


typedef enum
{
	enModbusRTU_Command_03 = 0x03,
	enModbusRTU_Command_04 = 0x04,
	enModbusRTU_Command_06 = 0x06,
	enModbusRTU_Command_10 = 0x10,
} enModbusRTU_Command_t;


extern stcModbusRTU_Handler_t gstcModbusRTU_Handler;


void ModbusRTU_Process(stcModbusRTU_Handler_t * pstcHandler);

void ModbusRTU_SetLocalAddr(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8Addr);

void ModbusRTU_SetParaTable(stcModbusRTU_Handler_t * pstcHandler, const stcModbusPara_t astcParaTable[], uint16_t u16ParaTableSize);

#if MODBUS_RTU_MASTER_ENABLE

void ModbusRTU_Master_ReqCmd03(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegCount, uint16_t u16Timeout);

void ModbusRTU_Master_ReqCmd04(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegCount, uint16_t u16Timeout);

void ModbusRTU_Master_ReqCmd06(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegValue, uint16_t u16Timeout);

void ModbusRTU_Master_ReqCmd10(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t * au16RegValueBuff, uint16_t u16RegCount, uint16_t u16Timeout);

#endif

#endif
