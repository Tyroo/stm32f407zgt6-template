#ifndef __AT_COMMAND_H__
#define __AT_COMMAND_H__


#include "at_command_instruct.h"


#define AT_COMMAND_GET_TICK()		0	//HAL_GetTick()
#define AT_COMMAND_PACKET_SIZE		(10)

typedef enum
{
	enAtCommand_State_Ok,
	enAtCommand_State_Err,
	enAtCommand_State_Timeout,
	enAtCommand_State_Busy,
} enAtCommand_State_t;

typedef struct
{
	uint16_t u16PacketOffset;
	uint16_t u16PacketSize;
	uint16_t u16PacketInstruct;
	uint16_t u16PacketTimeout;
} stcAtCommand_Packet_t;

typedef struct
{
	uint8_t u8State;
	uint16_t u16Timeout;
	uint16_t u32TimeoutStartTime;
	uint16_t u16InstructTableSize;
	
	uint8_t * pu8RecvDataBuff;
	uint8_t * pu8SendDataBuff;
	
	uint16_t u16RecvDataSize;
	uint16_t u16SendDataSize;
	
	uint8_t u8PacketBeginIndex;
	uint8_t u8PacketEndIndex;
	uint8_t u8PacketIndex;
	
	stcAtCommand_Packet_t astcPackets[AT_COMMAND_PACKET_SIZE];
	
	uint8_t (*Recv)(uint8_t ** au8Data, uint16_t * pu16Size);
	uint8_t (*Send)(uint8_t * au8Data, uint16_t * pu16Size);
	
	const stcAtCommand_Instruct_t * pstcInstructTable;
	
} stcAtCommand_Handler_t;


extern stcAtCommand_Handler_t gstcAtCommand_Handler;


void AtCommand_Process(stcAtCommand_Handler_t * pstcHandler);

void AtCommand_SetInstructTable(stcAtCommand_Handler_t * pstcHandler, 
								stcAtCommand_Instruct_t astcInstructTable[], uint16_t u16InstructTableSize);

uint8_t AtCommand_Check(const char * ps8CheckData, const char * ps8CheckString);

uint8_t AtCommand_Send(stcAtCommand_Handler_t * pstcHandler, 
						enAtCommand_Instruct_t enInstruct, const char * ps8ParaString, uint16_t u16Timeout);

#endif
