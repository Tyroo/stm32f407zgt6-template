/*******************************************************************************
** 名称：MX25L64.c
** 功能：MX25L64驱动
** 版本：V1.0
** 作者：张嘉良
** 日期：2023年9月13日
*******************************************************************************/

#include <string.h>
#include <math.h>
#include "mx25l64.h"


#define MX25L64_SECTOR_CALC(addr)		( (addr) / MX25L64_SECTOR_SIZE )
#define MX25L64_PAGE_CALC(addr)			( (addr) / MX25L64_SECTOR_PAGE_SIZE )

#define MX25L64_CMD_WRITE_PAGE			( 0x02 )
#define MX25L64_CMD_READ				( 0x03 )
#define MX25L64_CMD_DUAL_READ			( 0x3B )
#define MX25L64_CMD_READ_ID				( 0x9F )
#define MX25L64_CMD_SECTOR_ERASE		( 0x20 )
#define MX25L64_CMD_BLOCK_ERASE			( 0x52 )
#define MX25L64_CMD_CHIP_ERASE			( 0x60 )
#define MX25L64_CMD_POWER_DOWN			( 0xB9 )
#define MX25L64_CMD_POWER_UP			( 0xAB )
#define MX25L64_CMD_READ_STATUS_REG		( 0x05 )
#define MX25L64_CMD_WRITE_ENABLE		( 0x06 )
#define MX25L64_CMD_WRITE_DISABLE		( 0x04 )
#define MX25L64_CMD_DUMMY_DATA			( 0xFF )

#define MX25L64_HDL 					( gstcMX25L64_Handler )

#ifdef MX25L64_SECTOR_BUFF_NUM
	#if (MX25L64_SECTOR_BUFF_NUM <= 0) || (MX25L64_SECTOR_BUFF_NUM > 4)
		#error "MX25L64_SECTOR_BUFF_NUM cannot exceed the range [1-4]."
	#endif
#endif

struct stcMX25L64_Handler_t
{
	UINT8 u8SectorUse;
	UINT16 au16SectorIndex[MX25L64_SECTOR_BUFF_NUM];
};

static UINT8 gau8MX25L64_SectorBuff[MX25L64_SECTOR_BUFF_NUM][MX25L64_SECTOR_SIZE];

static struct stcMX25L64_Handler_t gstcMX25L64_Handler = 
{
	.au16SectorIndex = 
	{
	#if MX25L64_SECTOR_BUFF_NUM > 0x00
		MX25L64_SECTOR_NUMBER,
	#endif
		
	#if MX25L64_SECTOR_BUFF_NUM > 0x01
		MX25L64_SECTOR_NUMBER,
	#endif
		
	#if MX25L64_SECTOR_BUFF_NUM > 0x02
		MX25L64_SECTOR_NUMBER,
	#endif
		
	#if MX25L64_SECTOR_BUFF_NUM > 0x03
		MX25L64_SECTOR_NUMBER,
	#endif
	},
};


/**
  * @brief 通过SPI接口，向MX25L64发送一个字节的数据。
  * @note 此函数一般用来向MX25L64发送命令，并接收一个字节的响应数据。
  * @param [u8Byte]，写入的8bit数据。
  * @retval 返回MX25L64回传的8bit数据。
  */
__weak UINT8 MX25L64_SendByte(UINT8 u8Byte)
{
	SPI_I2S_SendData(SPI1, u8Byte);
	return SPI_I2S_ReceiveData(SPI1);
}


/**
  * @brief 通过SPI接口，向MX25L64发送指定大小的字节流数据 。
  * @note 此函数一般用来向MX25L64写入需要存储的用户数据。
  * @param [au8Buff]，要写入的数据缓冲区。
  * @param [u16Size]，要写入的数据的个数。
  * @retval None
  */
__weak void MX25L64_SendBytes(UINT8 au8Buff[], UINT16 u16Size)
{
	for(UINT16 u16Index = 0; u16Index < u16Size; u16Index++)
	{
		SPI_I2S_SendData(SPI1, au8Buff[u16Index]);
	}
}


/**
  * @brief 通过SPI接口，向MX25L64接收指定大小的字节流数据。
  * @note 此函数一般用来读取存储在MX25L64中的用户数据。
  * @param [au8Buff]，读取的用户数据存放缓冲区。
  * @param [u16Size]，读取的用户数据的个数。
  * @retval None
  */
__weak void MX25L64_RecvBytes(UINT8 au8Buff[], UINT16 u16Size)
{
	for(UINT16 u16Index = 0; u16Index < u16Size; u16Index++)
	{
		au8Buff[u16Index] = SPI_I2S_ReceiveData(SPI1);
	}
}


/**
  * @brief SPI接口的片选信号控制，用于选中对应的MX25L64。
  * @note 当传入的参数Select为1时，对应的片选应该被选中（低电平）；
  *		  当传入的参数Select为0时，对应的片选应该取消选中（高电平）。
  * @param [u8Select]，MX25L64的选中状态，1:选中；0：未选中。
  * @retval None
  */
__weak void MX25L64_ChipSelect(UINT8 u8Select)
{
	GPIOB->BSRR = (((uint32_t)GPIO_Pin_12) << (u8Select << 4));
}


/**
  * @brief 向MX25L64发送写使能命令。
  * @note None
  * @param [Enable]，1：允许写入；0：禁止写入。
  * @retval None
  */
static void MX25L64_WriteCtrl(UINT8 u8Enable)
{
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(u8Enable ? MX25L64_CMD_WRITE_ENABLE : MX25L64_CMD_WRITE_DISABLE);

	MX25L64_ChipSelect(0);
}


/**
  * @brief 等待MX25L64，直到其状态为空闲。
  * @note None
  * @param None
  * @retval None
  */
static void MX25L64_WriteWait(void)
{
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(MX25L64_CMD_READ_STATUS_REG);
	
	while( MX25L64_SendByte(MX25L64_CMD_DUMMY_DATA) & 0x01 );
	
	MX25L64_ChipSelect(0);
}


/**
  * @brief 从MX25L64中读取指定大小的用户数据。
  * @note None
  * @param [u32Addr]，要读取的FLASH起始地址。
  * @param [u16Number]，要读取的数据大小。
  * @param [au8Buff]，读取的数据存入的缓冲区。
  * @retval None
  */
static void MX25L64_ReadBytes(UINT32 u32Addr, UINT16 u16Number, UINT8 au8Buff[])
{
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(MX25L64_CMD_READ);
	
	MX25L64_SendByte((u32Addr >> 16) & 0xFF);
	MX25L64_SendByte((u32Addr >> 8) & 0xFF);
	MX25L64_SendByte(u32Addr & 0xFF);
	
	MX25L64_RecvBytes(au8Buff, u16Number);
	
	MX25L64_ChipSelect(0);
}


/**
  * @brief 以页为单位向MX25L64中写入指定大小的用户数据。
  * @note None
  * @param [u32Addr]，要写入的FLASH起始地址。
  * @param [u16Number]，要写入的数据大小。
  * @param [au8Buff]，写入的数据来自的缓冲区。
  * @retval None
  */
static void MX25L64_WriteBytes(UINT32 u32Addr, UINT16 u16Number, UINT8 au8Buff[])
{
	if (u16Number == 0) return;
	
	UINT32 u32AddrFirst = u32Addr;
	UINT32 u32AddrLast = u32Addr + u16Number;
	
	UINT16 u16Page = MX25L64_PAGE_CALC(u32AddrFirst);
	UINT16 u16PageEnd = MX25L64_PAGE_CALC(u32AddrLast - 1) + 1;
	
	UINT16 u16PageSize;
	
	for( ; u16Page < u16PageEnd; u16Page++)
	{
		u16PageSize = (u16Page + 1) * MX25L64_SECTOR_PAGE_SIZE;
		
		if (u16PageSize < u32AddrLast)
		{
			u16PageSize = u16PageSize - u32AddrFirst;
		}
		else
		{
			u16PageSize = u32AddrLast - u32AddrFirst;
		}
		
		MX25L64_WriteCtrl(1);
		
		MX25L64_ChipSelect(1);
		
		MX25L64_SendByte(MX25L64_CMD_WRITE_PAGE);
		
		MX25L64_SendByte((u32AddrFirst >> 16) & 0xFF);
		MX25L64_SendByte((u32AddrFirst >> 8) & 0xFF);
		MX25L64_SendByte(u32AddrFirst & 0xFF);
		
		MX25L64_SendBytes(au8Buff, u16PageSize);
		
		MX25L64_ChipSelect(0);
		
		u32AddrFirst += u16PageSize;
		au8Buff += u16PageSize;
		
		MX25L64_WriteWait();
	}
}


/**
  * @brief 擦除多个MX25L64中连续的扇区。
  * @note None
  * @param [u32SectorAddr]，擦除扇区的起始地址。
  * @param [u16SectorNumber]，指定要擦除的连续扇区个数。
  * @retval None
  */
void MX25L64_SectorErase(UINT32 u32SectorAddr, UINT16 u16SectorNumber)
{
	while(u16SectorNumber--)
	{
		MX25L64_WriteCtrl(1);
		
		MX25L64_ChipSelect(1);
		
		MX25L64_SendByte(MX25L64_CMD_SECTOR_ERASE);
		MX25L64_SendByte((u32SectorAddr >> 16) & 0xFF);
		MX25L64_SendByte((u32SectorAddr >> 8 ) & 0xFF);
		MX25L64_SendByte(u32SectorAddr & 0xFF);

		MX25L64_ChipSelect(0);
		
		u32SectorAddr += MX25L64_SECTOR_SIZE;
		
		MX25L64_WriteWait();
	}
}


/**
  * @brief 写入多个MX25L64中连续的扇区。
  * @note None
  * @param [u32SectorAddr]，写入扇区的起始地址。
  * @param [u16SectorNumber]，指定要写入的连续扇区个数。
  * @param [au8Buff]，写入到扇区的数据来源。
  * @retval None
  */
void MX25L64_SectorWrite(UINT32 u32SectorAddr, UINT16 u16SectorNumber, UINT8 au8Buff[])
{
	MX25L64_WriteBytes(u32SectorAddr, u16SectorNumber * MX25L64_SECTOR_SIZE, au8Buff);
}


/**
  * @brief 读取多个MX25L64中连续的扇区。
  * @note None
  * @param [u32SectorAddr]，读取扇区的起始地址。
  * @param [u16SectorNumber]，指定要读取的连续扇区个数。
  * @param [au8Buff]，读取的扇区数据存放缓冲区。
  * @retval None
  */
void MX25L64_SectorRead(UINT32 u32SectorAddr, UINT16 u16SectorNumber, UINT8 au8Buff[])
{
	MX25L64_ReadBytes(u32SectorAddr, u16SectorNumber * MX25L64_SECTOR_SIZE, au8Buff);
}


/**
  * @brief 从MX25L64中写入指定大小的用户数据。
  * @note 此函数引入了扇区缓存，增加了写入效率、减少了扇区擦除次数。
  * @param [u32Addr]，要写入的FLASH起始地址。
  * @param [u16Size]，要写入的数据大小。
  * @param [au8Buff]，写入的数据来自的缓冲区。
  * @retval None
  */
void MX25L64_Write(UINT32 u32Addr, UINT8 au8Buff[], UINT16 u16Size)
{
	UINT16 u16Sector;
	UINT16 u16SectorEnd;
	
	UINT32 u32SectorFirstAddr = u32Addr, u32SectorLastAddr = u32Addr + u16Size;
	UINT32 u32SectorSize;
	
	if (u16Size == 0) return;
	
	// 获取扇区索引
	u16Sector = MX25L64_SECTOR_CALC(u32Addr);
	u16SectorEnd = MX25L64_SECTOR_CALC(u32SectorLastAddr - 1) + 1;
	
	for( ; u16Sector < u16SectorEnd; u16Sector++)
	{
		u32SectorSize = (u16Sector + 1) * MX25L64_SECTOR_SIZE;
		
		if (u32SectorSize < u32SectorLastAddr)
		{
			u32SectorSize = u32SectorSize - u32SectorFirstAddr;
		}
		else
		{
			u32SectorSize = u32SectorLastAddr - u32SectorFirstAddr;
		}
		
		for(UINT8 u8SectorUse = 0; u8SectorUse < MX25L64_SECTOR_BUFF_NUM; u8SectorUse++)
		{
			if (MX25L64_HDL.au16SectorIndex[u8SectorUse] == MX25L64_SECTOR_NUMBER)
			{
				// 读取扇区数据
				MX25L64_SectorRead(u16Sector * MX25L64_SECTOR_SIZE, 1, gau8MX25L64_SectorBuff[u8SectorUse]);
				MX25L64_HDL.au16SectorIndex[u8SectorUse] = u16Sector;
			}
			
			if (MX25L64_HDL.au16SectorIndex[u8SectorUse] == u16Sector)
			{
				// 修改扇区数据
				memcpy(&gau8MX25L64_SectorBuff[u8SectorUse][u32SectorFirstAddr - u16Sector * MX25L64_SECTOR_SIZE], \
					   &au8Buff[u32SectorFirstAddr - u32Addr], u32SectorSize);
				goto SectorNextBlock;
			}
		}
		
		// 扇区覆盖写入
		MX25L64_SectorErase(MX25L64_HDL.au16SectorIndex[MX25L64_HDL.u8SectorUse] * MX25L64_SECTOR_SIZE, 1);
		MX25L64_SectorWrite(MX25L64_HDL.au16SectorIndex[MX25L64_HDL.u8SectorUse] * MX25L64_SECTOR_SIZE, 1, gau8MX25L64_SectorBuff[MX25L64_HDL.u8SectorUse]);
		
		// 读取扇区数据
		MX25L64_SectorRead(u16Sector * MX25L64_SECTOR_SIZE, 1, gau8MX25L64_SectorBuff[MX25L64_HDL.u8SectorUse]);
		memcpy(&gau8MX25L64_SectorBuff[MX25L64_HDL.u8SectorUse][u32SectorFirstAddr - u16Sector * MX25L64_SECTOR_SIZE], \
			   &au8Buff[u32SectorFirstAddr - u32Addr], u32SectorSize);
		
		MX25L64_HDL.au16SectorIndex[MX25L64_HDL.u8SectorUse] = u16Sector;
		MX25L64_HDL.u8SectorUse = (MX25L64_HDL.u8SectorUse + 1) % MX25L64_SECTOR_BUFF_NUM;
		
	SectorNextBlock:
		u32SectorFirstAddr += u32SectorSize;
	}
}


/**
  * @brief 从MX25L64中读取指定大小的用户数据。
  * @note 此函数引入了扇区缓存，增加了读取效率。
  * @param [u32Addr]，要读取的FLASH起始地址。
  * @param [u16Size]，要读取的数据大小。
  * @param [au8Buff]，读取的数据存入的缓冲区。
  * @retval None
  */
void MX25L64_Read(UINT32 u32Addr, UINT8 au8Buff[], UINT16 u16Size)
{
	UINT16 u16Sector;
	UINT16 u16SectorEnd;
	
	UINT32 u32SectorFirstAddr = u32Addr;
	UINT32 u32SectorLastAddr = u32Addr + u16Size;
	
	UINT32 u32SectorSize;
	
	if (u16Size == 0) return;
	
	// 获取扇区索引
	u16Sector = MX25L64_SECTOR_CALC(u32Addr);
	u16SectorEnd = MX25L64_SECTOR_CALC(u32SectorLastAddr - 1) + 1;
	
	for( ; u16Sector < u16SectorEnd; u16Sector++)
	{
		u32SectorSize = (u16Sector + 1) * MX25L64_SECTOR_SIZE;
		
		if (u32SectorSize < u32SectorLastAddr)
		{
			u32SectorSize = u32SectorSize - u32SectorFirstAddr;
		}
		else
		{
			u32SectorSize = u32SectorLastAddr - u32SectorFirstAddr;
		}
		
		for(UINT8 u8SectorUse = 0; u8SectorUse < MX25L64_SECTOR_BUFF_NUM; u8SectorUse++)
		{
			if (MX25L64_HDL.au16SectorIndex[u8SectorUse] == u16Sector)
			{
				// 取出缓存数据
				memcpy((UINT8 *)&au8Buff[u32SectorFirstAddr - u32Addr], \
						&gau8MX25L64_SectorBuff[u8SectorUse][u32SectorFirstAddr - u16Sector * MX25L64_SECTOR_SIZE], u32SectorSize);
				goto SectorNextBlock;
			}
		}
		
		// 读取数据
		MX25L64_ReadBytes(u32Addr, u16Size, au8Buff);
		break;

	SectorNextBlock:
		u32SectorFirstAddr += u32SectorSize;
	}
}


/**
  * @brief 对MX25L64执行片擦除操作。
  * @note None
  * @param None
  * @retval None
  */
void MX25L64_Erase(void)
{
	MX25L64_WriteCtrl(1);
	
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(MX25L64_CMD_CHIP_ERASE);

	MX25L64_ChipSelect(0);
	
	MX25L64_WriteWait();
}


/**
  * @brief 将未保存的扇区缓存中的数据立即保存至MX25L64中。
  * @note 此函数一般用于需要立即执行保存操作的场合，如写入后设备会立即断电等，
  *		  此时MX25L64中可能存在未被立即写入的用户数据，如果断电之前未调用此函数，
  *		  那么可能会出现在断电之前写入的部分扇区数据未被真实写入FLASH中。
  * @param None
  * @retval None
  */
void MX25L64_Sync(void)
{
	for(UINT8 u8SectorUse = 0; u8SectorUse < MX25L64_SECTOR_BUFF_NUM; u8SectorUse++)
	{
		if (MX25L64_HDL.au16SectorIndex[u8SectorUse] != MX25L64_SECTOR_NUMBER)
		{
			// 擦除扇区
			MX25L64_SectorErase(MX25L64_HDL.au16SectorIndex[u8SectorUse] * MX25L64_SECTOR_SIZE, 1);
			// 写入扇区
			MX25L64_SectorWrite(MX25L64_HDL.au16SectorIndex[u8SectorUse] * MX25L64_SECTOR_SIZE, 1, gau8MX25L64_SectorBuff[u8SectorUse]);
			MX25L64_HDL.au16SectorIndex[u8SectorUse] = MX25L64_SECTOR_NUMBER;
		}
	}
	
	MX25L64_HDL.u8SectorUse = 0;
}


/**
  * @brief 设置MX25L64的电源状态。
  * @note 设置为断电模式后，读写FLASH操作无效。
  * @param [u8State]，电源状态，0:断开MX25L64的电源；1：开启MX25L64的电源。
  * @retval None
  */
void MX25L64_SetPower(UINT8 u8State)
{
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(u8State ? MX25L64_CMD_POWER_UP : MX25L64_CMD_POWER_DOWN);
	
	MX25L64_ChipSelect(0);
	
	// 延时一定时间，等待MX25L64完全进入/退出断电模式
	for(UINT32 u32NopCount = 0; u32NopCount < MX25L64_POWER_DOWN_DELAY; u32NopCount++)
	{
		__nop();
	}
}

/**
  * @brief 获取MX25L64的容量。
  * @note None
  * @param None
  * @retval 返回以字节为单位的容量值。
  */
UINT32 MX25L64_GetCapacity(void)
{
	UINT8 u8SizeCode = MX25L64_GetId() & 0x0F;
	UINT32 u32Size = 1024 * 1024;
	
	if (u8SizeCode >= 4)
	{
		u32Size *= pow(2, u8SizeCode - 4);
	}
	else
	{
		u32Size /= pow(2, 4 - u8SizeCode);
	}
	
	return u32Size;
}


/**
  * @brief 读取MX25L64的ID。
  * @note None
  * @param None
  * @retval 返回四个字节的ID。
  */
UINT32 MX25L64_GetId(void)
{
	UINT32 u32DeviceId = 0;
	
	MX25L64_ChipSelect(1);
	
	MX25L64_SendByte(MX25L64_CMD_READ_ID);
	
	u32DeviceId = MX25L64_SendByte(MX25L64_CMD_DUMMY_DATA);
	u32DeviceId <<= 8;
	u32DeviceId |= MX25L64_SendByte(MX25L64_CMD_DUMMY_DATA);
	u32DeviceId <<= 8;
	u32DeviceId |= MX25L64_SendByte(MX25L64_CMD_DUMMY_DATA);
	
	MX25L64_ChipSelect(0);
	
	return u32DeviceId;
}
