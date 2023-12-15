#include "usart.h"
#include "crc.h"
#include "modbus_rtu.h"


stcModbusRTU_Handler_t gstcModbusRTU_Handler = 
{
	.u8LocalAddr = 0x00,
	
	/*.au8SendDataBuff = USART1_Data.u8TxBuff[0],
	.au8RecvDataBuff = USART1_Data.u8RxBuff[0],
	
	.Recv = UART1_ReceiveData,
	.Send = UART1_TransmitData,*/
	
	.GetCRC16 = CRC16,
	
	.pstcParaTable = gastcModbusPara_Table,
	.u16ParaTableSize = MODBUS_RTU_PARA_TABLE_SIZE,
};


static uint8_t ModbusRTU_ADDR_Check(stcModbusRTU_Handler_t * pstcHandler)
{
	pstcHandler->u8RemoteAddr = pstcHandler->au8RecvDataBuff[0];
	
#if MODBUS_RTU_SLAVE_ENABLE
	if (pstcHandler->u8LocalAddr)
	{
		return (pstcHandler->u8RemoteAddr == pstcHandler->u8LocalAddr) || (pstcHandler->u8RemoteAddr == 0xFF);
	}
#endif
	
#if MODBUS_RTU_MASTER_ENABLE
	
	uint8_t u8ReqAddr = pstcHandler->au8SendDataBuff[pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketOffset];
	
	return (pstcHandler->u8RemoteAddr == u8ReqAddr);
#else
	return 0;
#endif
}


static void ModbusRTU_REQ_Check(stcModbusRTU_Handler_t * pstcHandler, 
								uint16_t u16PacketOffset, uint16_t u16PacketSize)
{
	if ((pstcHandler->u8State > enModbusRTU_State_Ok) && (pstcHandler->u8State < enModbusRTU_State_Timeout))
	{
		pstcHandler->au8SendDataBuff[u16PacketOffset + 1] += 0x80;
		pstcHandler->au8SendDataBuff[u16PacketOffset + 2] = pstcHandler->u8State;
		u16PacketSize = 3;
	}
	
	uint16_t u16CRC16 = pstcHandler->GetCRC16(&pstcHandler->au8SendDataBuff[u16PacketOffset], u16PacketSize);
	
	(*(uint16_t *)&pstcHandler->au8SendDataBuff[u16PacketOffset + u16PacketSize]) = u16CRC16;
	
	u16PacketSize += 2;
	
	pstcHandler->Send(&pstcHandler->au8SendDataBuff[u16PacketOffset], &u16PacketSize);
}


static uint8_t ModbusRTU_CRC_Check(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16CheckCode = (pstcHandler->au8RecvDataBuff[pstcHandler->u16RecvDataLength - 1] << 8) + \
		pstcHandler->au8RecvDataBuff[pstcHandler->u16RecvDataLength - 2];
	
	if (u16CheckCode != pstcHandler->GetCRC16(pstcHandler->au8RecvDataBuff, pstcHandler->u16RecvDataLength - 2))
	{
		pstcHandler->u8State = enModbusRTU_State_CrcErr;
		return 0;
	}
	
	return 1;
}


static uint8_t ModbusRTU_CMD_Check(stcModbusRTU_Handler_t * pstcHandler)
{
	uint8_t u8CheckCmd = pstcHandler->au8RecvDataBuff[1];
	
	if ((u8CheckCmd != enModbusRTU_Command_03) && (u8CheckCmd != enModbusRTU_Command_04) && \
		(u8CheckCmd != enModbusRTU_Command_06) && (u8CheckCmd != enModbusRTU_Command_10))
	{
		pstcHandler->u8State = enModbusRTU_State_CmdErr;
		return 0;
	}
	
	return 1;
}


#if MODBUS_RTU_SLAVE_ENABLE

static uint8_t ModbusRTU_ReadRegister(stcModbusRTU_Handler_t * pstcHandler, 
									  uint16_t u16RegAddr, uint8_t * au8DataBuff, uint16_t * pu16RegCount, uint8_t u8Rw)
{
	uint8_t u8ParaSize;
	uint16_t u16ParaIndex;
	uint32_t u32ParaValue;
	
	void * pvPara;
	
	const stcModbusPara_t * pstcPara = ModbusPara_Find(u16RegAddr, &u16ParaIndex, pstcHandler->pstcParaTable, pstcHandler->u16ParaTableSize);
	
	if ((pstcPara == NULL) || ((*pu16RegCount + u16ParaIndex) > pstcHandler->u16ParaTableSize)) 
	{
		pstcHandler->u8State = enModbusRTU_State_RegErr;
		return 0;
	}
	
	u16ParaIndex = *pu16RegCount;
	*pu16RegCount = 0;
	
	while(u16ParaIndex--)
	{
		if (pstcPara->stcParaAttr.u8Rw != u8Rw)
		{
			pstcHandler->u8State = enModbusRTU_State_DataErr;
			return 0;
		}
		
		pvPara = pstcPara->pvPara;
		
		if (pvPara == NULL) 
		{
			u32ParaValue = 0;
			pvPara = &u32ParaValue;
		}
		
		switch(pstcPara->stcParaAttr.u8Bit)
		{
			case enValueType_Bit8:
				u8ParaSize = 2;
				u32ParaValue = (*(uint8_t *)pvPara);
				break;
			case enValueType_Bit16:
				u8ParaSize = 2;
				u32ParaValue = (*(uint16_t *)pvPara);
				break;
			case enValueType_Bit32:
				if (u16ParaIndex)
				{
					u8ParaSize = 4;
					u32ParaValue = *((uint32_t *)pvPara);
					u16ParaIndex--;
				}
				else
				{
					u8ParaSize = 2;
					u32ParaValue = (*(uint16_t *)pvPara);
				}
				break;
			default:
				u8ParaSize = 2;
				u32ParaValue = (*(uint16_t *)pvPara);
				break;
		}
		
		if ((pstcPara->ParaCallbackFunc != NULL) && (pstcPara->ParaCallbackFunc(enReadOnly, &u32ParaValue) == false))
		{
			pstcHandler->u8State = enModbusRTU_State_DataErr;
			return 0;
		}
		
		if (u8ParaSize == 4)
		{
			au8DataBuff[0] = (u32ParaValue >> 24) & 0xFF;
			au8DataBuff[1] = (u32ParaValue >> 16) & 0xFF;
			au8DataBuff[2] = (u32ParaValue >> 8) & 0xFF;
			au8DataBuff[3] = u32ParaValue & 0xFF;
		}
		else
		{
			au8DataBuff[0] = (u32ParaValue >> 8) & 0xFF;
			au8DataBuff[1] = u32ParaValue & 0xFF;
		}
		
		pstcPara++;
		(*pu16RegCount) += (u8ParaSize >> 1);
		au8DataBuff += u8ParaSize;
	}
	
	return 1;
}

#endif

static uint8_t ModbusRTU_WriteRegister(stcModbusRTU_Handler_t * pstcHandler, 
									   uint16_t u16RegAddr, uint8_t * au8DataBuff, uint16_t * pu16RegCount, uint8_t u8Rw)
{
	uint8_t u8ParaSize, u8RegSize;
	uint16_t u16ParaIndex;
	uint32_t u32ParaValue;
	
	const stcModbusPara_t * pstcPara = ModbusPara_Find(u16RegAddr, &u16ParaIndex, pstcHandler->pstcParaTable, pstcHandler->u16ParaTableSize);
	
	if ((pstcPara == NULL) || ((u16ParaIndex + *pu16RegCount) > pstcHandler->u16ParaTableSize)) 
	{
		pstcHandler->u8State = enModbusRTU_State_RegErr;
		return 0;
	}
	
	u16ParaIndex = *pu16RegCount;
	*pu16RegCount = 0;
	
	while(u16ParaIndex--)
	{
		if (pstcPara->stcParaAttr.u8Rw != u8Rw)
		{
			pstcHandler->u8State = enModbusRTU_State_DataErr;
			return 0;
		}
		
		switch(pstcPara->stcParaAttr.u8Bit)
		{
			case enValueType_Bit8:
				u8ParaSize = 1;
				u8RegSize = 2;
				u32ParaValue = au8DataBuff[1];
				break;
			case enValueType_Bit16:
				u8ParaSize = 2;
				u8RegSize = 2;
				u32ParaValue = (au8DataBuff[0] << 8) + au8DataBuff[1];
				break;
			case enValueType_Bit32:
				if (u16ParaIndex)
				{
					u8ParaSize = 4;
					u8RegSize = 4;
					u32ParaValue = (au8DataBuff[0] << 24) + (au8DataBuff[1] << 16) + (au8DataBuff[2] << 8) + au8DataBuff[3];
					u16ParaIndex--;
				}
				else
				{
					u8ParaSize = 2;
					u8RegSize = 2;
					u32ParaValue = (au8DataBuff[0] << 8) + au8DataBuff[1];
				}
				break;
			default:
				u32ParaValue = (au8DataBuff[0] << 8) + au8DataBuff[1];
				u8ParaSize = 2;
				u8RegSize = 2;
				break;
		}
		
		if ((pstcPara->ParaCallbackFunc != NULL) && (pstcPara->ParaCallbackFunc(enWriteOnly, &u32ParaValue) == false))
		{
			pstcHandler->u8State = enModbusRTU_State_DataErr;
			return 0;
		}
		
		if (pstcPara->pvPara != NULL)
		{
			memcpy(pstcPara->pvPara, &u32ParaValue, u8ParaSize);
		}
		
		pstcPara++;
		(*pu16RegCount) += (u8RegSize >> 1);
		au8DataBuff += u8RegSize;
	}
	
	return 1;
}


#if MODBUS_RTU_SLAVE_ENABLE


// 读取保持寄存器
static void ModbusRTU_Slave_ResCmd03(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16RegAddr = ((pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3]);
	uint16_t u16RegCount = ((pstcHandler->au8RecvDataBuff[4] << 8) + pstcHandler->au8RecvDataBuff[5]);
	
	if (ModbusRTU_ReadRegister(pstcHandler, u16RegAddr, &pstcHandler->au8SendDataBuff[3], &u16RegCount, enReadWrite))	// 读取的数据
	{
		pstcHandler->au8SendDataBuff[2] = 2 * u16RegCount;	// 字节个数
		pstcHandler->u16SendDataLength = 3 + pstcHandler->au8SendDataBuff[2];
	}
}


// 读取输入寄存器
static void ModbusRTU_Slave_ResCmd04(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16RegAddr = ((pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3]);
	uint16_t u16RegCount = ((pstcHandler->au8RecvDataBuff[4] << 8) + pstcHandler->au8RecvDataBuff[5]);
	
	if (ModbusRTU_ReadRegister(pstcHandler, u16RegAddr, &pstcHandler->au8SendDataBuff[3], &u16RegCount, enReadOnly))	// 读取的数据
	{
		pstcHandler->au8SendDataBuff[2] = 2 * u16RegCount;	// 字节个数
		pstcHandler->u16SendDataLength = 3 + pstcHandler->au8SendDataBuff[2];
	}
}


// 写单个保持寄存器
static void ModbusRTU_Slave_ResCmd06(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16RegAddr = ((pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3]);
	uint16_t u16RegCount = 1;
	
	if (ModbusRTU_WriteRegister(pstcHandler, u16RegAddr, &pstcHandler->au8RecvDataBuff[7], &u16RegCount, enReadWrite))	// 读取的数据
	{
		pstcHandler->au8SendDataBuff[2] = u16RegAddr >> 8;		// 寄存器地址高位
		pstcHandler->au8SendDataBuff[3] = u16RegAddr & 0xFF;	// 寄存器地址低位
		
		pstcHandler->au8SendDataBuff[4] = pstcHandler->au8RecvDataBuff[7];	// 寄存器值高位
		pstcHandler->au8SendDataBuff[5] = pstcHandler->au8RecvDataBuff[8];	// 寄存器值高低位
		
		pstcHandler->u16SendDataLength = 6;
	}
}


// 写多个连续的保持寄存器
static void ModbusRTU_Slave_ResCmd10(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16RegAddr = ((pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3]);
	uint16_t u16RegCount = ((pstcHandler->au8RecvDataBuff[4] << 8) + pstcHandler->au8RecvDataBuff[5]);
	
	if (ModbusRTU_WriteRegister(pstcHandler, u16RegAddr, &pstcHandler->au8RecvDataBuff[7], &u16RegCount, enReadWrite))	// 读取的数据
	{
		pstcHandler->au8SendDataBuff[2] = u16RegAddr >> 8;		// 寄存器地址高位
		pstcHandler->au8SendDataBuff[3] = u16RegAddr & 0xFF;	// 寄存器地址低位
		
		pstcHandler->au8SendDataBuff[4] = u16RegCount >> 8;		// 寄存器值高位
		pstcHandler->au8SendDataBuff[5] = u16RegCount & 0xFF;	// 寄存器值高低位
		
		pstcHandler->u16SendDataLength = 6;
	}
}


static void ModbusRTU_Slave_ResCmd(stcModbusRTU_Handler_t * pstcHandler)
{
	uint8_t u8CmdCode = pstcHandler->au8RecvDataBuff[1];
	
	switch(u8CmdCode)
	{
		case enModbusRTU_Command_03:
			ModbusRTU_Slave_ResCmd03(pstcHandler);
			break;
		case enModbusRTU_Command_04:
			ModbusRTU_Slave_ResCmd04(pstcHandler);
			break;
		case enModbusRTU_Command_06:
			ModbusRTU_Slave_ResCmd06(pstcHandler);
			break;
		case enModbusRTU_Command_10:
			ModbusRTU_Slave_ResCmd10(pstcHandler);
			break;
		default:
			return;
	}
	
	ModbusRTU_REQ_Check(pstcHandler, 0, pstcHandler->u16SendDataLength);
}

#endif


#if MODBUS_RTU_MASTER_ENABLE

static void ModbusRTU_Master_ResCmd03(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketOffset;
	
	uint16_t u16ResRegCount = pstcHandler->au8RecvDataBuff[2] >> 1;
	uint16_t u16ReqRegAddr = (pstcHandler->au8SendDataBuff[u16PacketOffset + 2] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 3];
	uint16_t u16ReqRegCount = (pstcHandler->au8SendDataBuff[u16PacketOffset + 4] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 5];
	
	if (u16ReqRegCount == u16ResRegCount)
	{
		ModbusRTU_WriteRegister(pstcHandler, u16ReqRegAddr, &pstcHandler->au8RecvDataBuff[3], &u16ResRegCount, enReadWrite);
	}
}


static void ModbusRTU_Master_ResCmd04(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketOffset;
	
	uint16_t u16ResRegCount = pstcHandler->au8RecvDataBuff[2] >> 1;
	uint16_t u16ReqRegAddr = (pstcHandler->au8SendDataBuff[u16PacketOffset + 2] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 3];
	uint16_t u16ReqRegCount = (pstcHandler->au8SendDataBuff[u16PacketOffset + 4] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 5];
	
	if (u16ReqRegCount == u16ResRegCount)
	{
		ModbusRTU_WriteRegister(pstcHandler, u16ReqRegAddr, &pstcHandler->au8RecvDataBuff[3], &u16ResRegCount, enReadOnly);
	}
}


static void ModbusRTU_Master_ResCmd06(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketOffset;
	
	uint16_t u16ResRegAddr = (pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3];
	uint16_t u16ReqRegAddr = (pstcHandler->au8SendDataBuff[u16PacketOffset + 2] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 3];
	uint16_t u16RegCount = 1;
	
	if (u16ReqRegAddr == u16ResRegAddr)
	{
		if (ModbusRTU_WriteRegister(pstcHandler, u16ResRegAddr, &pstcHandler->au8RecvDataBuff[4], &u16RegCount, enReadWrite) == false)
		{
			ModbusRTU_WriteRegister(pstcHandler, u16ResRegAddr, &pstcHandler->au8RecvDataBuff[4], &u16RegCount, enWriteOnly);
		}
	}
}


static void ModbusRTU_Master_ResCmd10(stcModbusRTU_Handler_t * pstcHandler)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketIndex].u16PacketOffset;
	
	uint16_t u16ResRegAddr = (pstcHandler->au8RecvDataBuff[2] << 8) + pstcHandler->au8RecvDataBuff[3];
	uint16_t u16ResRegCount = (pstcHandler->au8RecvDataBuff[4] << 8) + pstcHandler->au8RecvDataBuff[5];
	
	uint16_t u16ReqRegAddr = (pstcHandler->au8SendDataBuff[u16PacketOffset + 2] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 3];
	uint16_t u16ReqRegCount = (pstcHandler->au8SendDataBuff[u16PacketOffset + 4] << 8) + pstcHandler->au8SendDataBuff[u16PacketOffset + 5];
	
	if ((u16ReqRegAddr == u16ResRegAddr) && (u16ReqRegCount == u16ResRegCount))
	{
		if (ModbusRTU_WriteRegister(pstcHandler, u16ResRegAddr, &pstcHandler->au8SendDataBuff[7], &u16ResRegCount, enReadWrite) == false)
		{
			ModbusRTU_WriteRegister(pstcHandler, u16ResRegAddr, &pstcHandler->au8SendDataBuff[7], &u16ResRegCount, enWriteOnly);
		}
	}
}


static void ModbusRTU_Master_ResCmd(stcModbusRTU_Handler_t * pstcHandler)
{
	uint8_t u8CmdCode = pstcHandler->au8RecvDataBuff[1];
	
	switch(u8CmdCode)
	{
		case enModbusRTU_Command_03:
			ModbusRTU_Master_ResCmd03(pstcHandler);
			break;
		case enModbusRTU_Command_04:
			ModbusRTU_Master_ResCmd04(pstcHandler);
			break;
		case enModbusRTU_Command_06:
			ModbusRTU_Master_ResCmd06(pstcHandler);
			break;
		case enModbusRTU_Command_10:
			ModbusRTU_Master_ResCmd10(pstcHandler);
			break;
		default:
			return;
	}
}


static void ModbusRTU_Master_ReqCmd(stcModbusRTU_Handler_t * pstcHandler, 
									uint16_t u16PacketOffset, uint16_t u16PacketSize, uint16_t u16Timeout)
{
	if (pstcHandler->u8PacketEndIndex < (MODBUS_RTU_PACKETS_SIZE - 1))
	{
		pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketSize = u16PacketSize;
		pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset = u16PacketOffset;
		pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketTimeout = u16Timeout;
		
		pstcHandler->u8PacketEndIndex++;
		pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset = u16PacketOffset + u16PacketSize + 2;
	}
}


// 读保持寄存器
void ModbusRTU_Master_ReqCmd03(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegCount, uint16_t u16Timeout)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset;
	
	// 发送设备地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 0] = u8DeviceAddr;
	
	// 发送命令码
	pstcHandler->au8SendDataBuff[u16PacketOffset + 1] = enModbusRTU_Command_03;
	
	// 发送寄存器地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 2] = u16RegAddr >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 3] = u16RegAddr & 0xFF;
	
	// 发送寄存器个数
	pstcHandler->au8SendDataBuff[u16PacketOffset + 4] = u16RegCount >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 5] = u16RegCount & 0xFF;
	
	ModbusRTU_Master_ReqCmd(pstcHandler, u16PacketOffset, 6, u16Timeout);
}


// 读输入寄存器
void ModbusRTU_Master_ReqCmd04(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegCount, uint16_t u16Timeout)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset;
	
	// 发送设备地址
	pstcHandler->au8SendDataBuff[0] = u8DeviceAddr;
	
	// 发送命令码
	pstcHandler->au8SendDataBuff[1] = enModbusRTU_Command_04;
	
	// 发送寄存器地址
	pstcHandler->au8SendDataBuff[2] = u16RegAddr >> 8;
	pstcHandler->au8SendDataBuff[3] = u16RegAddr & 0xFF;
	
	// 发送寄存器个数
	pstcHandler->au8SendDataBuff[4] = u16RegCount >> 8;
	pstcHandler->au8SendDataBuff[5] = u16RegCount & 0xFF;
	
	ModbusRTU_Master_ReqCmd(pstcHandler, u16PacketOffset, 6, u16Timeout);
}


// 写单个保持寄存器
void ModbusRTU_Master_ReqCmd06(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t u16RegValue, uint16_t u16Timeout)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset;
	
	// 发送设备地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 0] = u8DeviceAddr;
	
	// 发送命令码
	pstcHandler->au8SendDataBuff[u16PacketOffset + 1] = enModbusRTU_Command_06;
	
	// 发送寄存器地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 2] = u16RegAddr >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 3] = u16RegAddr & 0xFF;
	
	// 发送寄存器值
	pstcHandler->au8SendDataBuff[u16PacketOffset + 4] = u16RegValue >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 5] = u16RegValue & 0xFF;
	
	ModbusRTU_Master_ReqCmd(pstcHandler, u16PacketOffset, 6, u16Timeout);
}


// 写多个连续的保持寄存器
void ModbusRTU_Master_ReqCmd10(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8DeviceAddr,
							   uint16_t u16RegAddr, uint16_t * au16RegValueBuff, uint16_t u16RegCount, uint16_t u16Timeout)
{
	uint16_t u16PacketOffset = pstcHandler->astcPackets[pstcHandler->u8PacketEndIndex].u16PacketOffset;
	
	// 发送设备地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 0] = u8DeviceAddr;
	
	// 发送命令码
	pstcHandler->au8SendDataBuff[u16PacketOffset + 1] = enModbusRTU_Command_06;
	
	// 发送寄存器地址
	pstcHandler->au8SendDataBuff[u16PacketOffset + 2] = u16RegAddr >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 3] = u16RegAddr & 0xFF;
	
	// 发送寄存器个数
	pstcHandler->au8SendDataBuff[u16PacketOffset + 4] = u16RegCount >> 8;
	pstcHandler->au8SendDataBuff[u16PacketOffset + 5] = u16RegCount & 0xFF;
	
	// 发送字节数
	pstcHandler->au8SendDataBuff[u16PacketOffset + 6] = (u16RegCount << 1);
	
	memcpy(&pstcHandler->au8SendDataBuff[u16PacketOffset + 7], au16RegValueBuff, (u16RegCount << 1));
	
	ModbusRTU_Master_ReqCmd(pstcHandler, u16PacketOffset, 7 + (u16RegCount << 1), u16Timeout);
}

#endif


void ModbusRTU_Process(stcModbusRTU_Handler_t * pstcHandler)
{
	// 数据请求
	if ((pstcHandler->u8State != enModbusRTU_State_MBusy) && \
		(pstcHandler->u8PacketBeginIndex != pstcHandler->u8PacketEndIndex))
	{
		pstcHandler->u8State = enModbusRTU_State_MBusy;
		pstcHandler->u8PacketIndex = pstcHandler->u8PacketBeginIndex;
		
		ModbusRTU_REQ_Check(pstcHandler, 
							pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketOffset,
							pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketSize);
		
		pstcHandler->u16Timeout = pstcHandler->astcPackets[pstcHandler->u8PacketBeginIndex].u16PacketTimeout;
		pstcHandler->u32TimeoutStartTime = MODBUS_RTU_GET_TICK();
		
		if ((++pstcHandler->u8PacketBeginIndex) >= pstcHandler->u8PacketEndIndex)
		{
			pstcHandler->u8PacketBeginIndex = pstcHandler->u8PacketEndIndex = 0;
		}
	}
	
	// 数据响应
	if (pstcHandler->Recv(&pstcHandler->au8RecvDataBuff, &pstcHandler->u16RecvDataLength))
	{
		// 地址检验、CRC校验、命令校验
		if (!ModbusRTU_ADDR_Check(pstcHandler))
		{
			return;
		}
		
		pstcHandler->u16Timeout = 0;
		pstcHandler->u8State = enModbusRTU_State_Ok;						// 将当前通信状态设置为OK
		pstcHandler->au8SendDataBuff[0] = pstcHandler->au8RecvDataBuff[0];	// 返回设备地址
		pstcHandler->au8SendDataBuff[1] = pstcHandler->au8RecvDataBuff[1];	// 返回命令码
		
		// CRC校验、命令校验通过
		if (ModbusRTU_CRC_Check(pstcHandler) && ModbusRTU_CMD_Check(pstcHandler))
		{
			// 如果当前为从机
			if (pstcHandler->u8LocalAddr)
			{
			#if MODBUS_RTU_SLAVE_ENABLE
				ModbusRTU_Slave_ResCmd(pstcHandler);
			#endif
			}
			// 如果当前为主机
			else
			{
			#if MODBUS_RTU_MASTER_ENABLE
				ModbusRTU_Master_ResCmd(pstcHandler);
			#endif
			}
		}
		// 校验未通过
		else
		{
		#if MODBUS_RTU_SLAVE_ENABLE
			if (pstcHandler->u8LocalAddr)
			{
				ModbusRTU_REQ_Check(pstcHandler, 0, 0);
			}
		#endif
		}
	}
	// 通信超时处理
	else if ((pstcHandler->u16Timeout) && ((uint32_t)(MODBUS_RTU_GET_TICK() - pstcHandler->u32TimeoutStartTime) >= pstcHandler->u16Timeout))
	{
		pstcHandler->u16Timeout = 0;
		pstcHandler->u8State = enModbusRTU_State_Timeout;
	}
}


// MODBUS本机地址设置
void ModbusRTU_SetLocalAddr(stcModbusRTU_Handler_t * pstcHandler, uint8_t u8Addr)
{
	pstcHandler->u8LocalAddr = u8Addr;
	pstcHandler->u16Timeout = 0;
	pstcHandler->u8PacketBeginIndex = pstcHandler->u8PacketEndIndex = 0;
	pstcHandler->u8State = enModbusRTU_State_Ok;
}


// MODBUS参数表设置
void ModbusRTU_SetParaTable(stcModbusRTU_Handler_t * pstcHandler, const stcModbusPara_t astcParaTable[], uint16_t u16ParaTableSize)
{
	if ((astcParaTable == NULL) || (u16ParaTableSize == 0))
	{
		return;
	}
	
	pstcHandler->pstcParaTable = astcParaTable;
	pstcHandler->u16ParaTableSize = u16ParaTableSize;
}
