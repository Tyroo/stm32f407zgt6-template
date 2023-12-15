#include "usart.h"
#include "crc.h"
#include "at_command.h"


stcAtCommand_Handler_t gstcAtCommand_Handler = 
{
	/*.pu8SendDataBuff = USART1_Data.u8TxBuff[0],
	.pu8RecvDataBuff = USART1_Data.u8RxBuff[0],
	
	.Recv = UART1_ReceiveData,
	.Send = UART1_TransmitData,*/
	
	.pstcInstructTable = gastcAtCommand_Instruct_Table,
	.u16InstructTableSize = enAtCommand_Instruct_Bottom,
};


// AT命令处理
void AtCommand_Process(stcAtCommand_Handler_t * pstcHandler)
{
	// 发送指令
	if ((pstcHandler->u8State != enAtCommand_State_Busy) && \
		(pstcHandler->u8PacketBeginIndex != pstcHandler->u8PacketEndIndex))
	{
		pstcHandler->u8State = enAtCommand_State_Busy;
		pstcHandler->u8PacketIndex = pstcHandler->u8PacketBeginIndex;
		
		uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketOffset;
		uint16_t u16PacketSize = pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketSize;
		
		pstcHandler->Send(&pstcHandler->pu8SendDataBuff[u16PacketOffset], &u16PacketSize);
		
		pstcHandler->u16Timeout = pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketTimeout;
		pstcHandler->u32TimeoutStartTime = AT_COMMAND_GET_TICK();
		
		pstcHandler->u8PacketIndex = pstcHandler->u8PacketBeginIndex;
		
		if ((++pstcHandler->u8PacketBeginIndex) >= pstcHandler->u8PacketEndIndex)
		{
			pstcHandler->u8PacketBeginIndex = pstcHandler->u8PacketEndIndex = 0;
		}
	}
	
	// 接收数据
	if (pstcHandler->Recv(&pstcHandler->pu8RecvDataBuff, &pstcHandler->u16RecvDataSize))
	{
		pstcHandler->u16Timeout = 0;
		pstcHandler->u8State = enAtCommand_State_Ok;
		
		uint16_t u16Instruct = pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketInstruct;
		
		if ((u16Instruct < enAtCommand_Instruct_Bottom) && (pstcHandler->pstcInstructTable[u16Instruct].pvInstructCallbackFunc != NULL))
		{
			if (pstcHandler->pstcInstructTable[u16Instruct].pvInstructCallbackFunc((const char *)pstcHandler->pu8RecvDataBuff, pstcHandler->u16RecvDataSize) == false)
			{
				pstcHandler->u8State = enAtCommand_State_Err;
			}
		}
	}
	// 响应超时处理
	else if ((pstcHandler->u16Timeout) && ((uint32_t)(AT_COMMAND_GET_TICK() - pstcHandler->u32TimeoutStartTime) >= pstcHandler->u16Timeout))
	{
		pstcHandler->u16Timeout = 0;
		pstcHandler->u8State = enAtCommand_State_Timeout;
	}
}


// 检查AT响应字符串与指定字符串是否相等
uint8_t AtCommand_Check(const char * ps8CheckData, const char * ps8CheckString)
{
	return ((strlen(ps8CheckData) == strlen(ps8CheckString)) && \
			(strcmp(ps8CheckData, ps8CheckString) == 0));
}


// 发送AT命令
uint8_t AtCommand_Send(stcAtCommand_Handler_t * pstcHandler, 
					   enAtCommand_Instruct_t enInstruct, const char * ps8ParaString, uint16_t u16Timeout)
{
	// 命令超出参数表命令范围
	if (enInstruct >= enAtCommand_Instruct_Bottom) return 0;
	
	// 单次发送指令数目超出
	if (pstcHandler->u8PacketEndIndex >= (AT_COMMAND_PACKET_SIZE - 1)) return 0;
	
	uint16_t u16CommandSize = strlen(pstcHandler->pstcInstructTable[enInstruct].ps8Command);
	uint16_t u16PacketSize, u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset;
	
	// 该命令带参数
	if ((u16CommandSize > 0) && (pstcHandler->pstcInstructTable[enInstruct].ps8Command[u16CommandSize - 1] == '='))
	{
		u16PacketSize = sprintf((char *)&pstcHandler->pu8SendDataBuff[u16PacketOffset], "AT%s%s\r\n", 
								pstcHandler->pstcInstructTable[enInstruct].ps8Command, ps8ParaString);
	}
	// 该命令不带参数
	else
	{
		u16PacketSize = sprintf((char *)&pstcHandler->pu8SendDataBuff[u16PacketOffset], "AT%s\r\n",
								pstcHandler->pstcInstructTable[enInstruct].ps8Command);
	}
	
	pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketSize = u16PacketSize;
	pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketTimeout = u16Timeout;
	pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketInstruct = enInstruct;
	
	pstcHandler->u8PacketEndIndex++;
	pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset = u16PacketOffset + u16PacketSize;
	
	return 1;
}


// 设置AT指令表
void AtCommand_SetInstructTable(stcAtCommand_Handler_t * pstcHandler, 
								stcAtCommand_Instruct_t astcInstructTable[], uint16_t u16InstructTableSize)
{
	if ((astcInstructTable == NULL) || (u16InstructTableSize == 0))
	{
		return;
	}
	
	pstcHandler->pstcInstructTable = astcInstructTable;
	pstcHandler->u16InstructTableSize = u16InstructTableSize;
}
