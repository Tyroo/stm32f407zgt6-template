#ifndef __MX25L64_H__
#define __MX25L64_H__


#include "spi.h"

typedef unsigned char	INT8;
typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;

#define MX25L64_SECTOR_BUFF_NUM			( 0x01 )

#define MX25L64_SECTOR_SIZE				( 4096 )
#define MX25L64_SECTOR_NUMBER			( 2048 )
#define MX25L64_SECTOR_PAGE_SIZE		( 256  )
#define MX25L64_SECTOR_PAGE_NUMBER		( 16   )

#define MX25L64_POWER_DOWN_DELAY		( 0x0A )

#ifdef __cplusplus
extern "C" {
#endif

// MX25L64 SPI Configure
void MX25L64_ChipSelect(UINT8 u8Select);
void MX25L64_SendBytes(UINT8 au8Buff[], UINT16 u16Size);
void MX25L64_RecvBytes(UINT8 au8Buff[], UINT16 u16Size);
UINT8 MX25L64_SendByte(UINT8 u8Byte);

// Reading and writing MX25L64 data
void MX25L64_SectorErase(UINT32 u32SectorAddr, UINT16 u16SectorNumber);
void MX25L64_SectorWrite(UINT32 u32SectorAddr, UINT16 u16SectorNumber, UINT8 au8Buff[]);
void MX25L64_SectorRead(UINT32 u32SectorAddr, UINT16 u16SectorNumber, UINT8 au8Buff[]);

void MX25L64_SectorErase(UINT32 u32SectorAddr, UINT16 u16SectorNumber);
void MX25L64_Write(UINT32 u32Addr, UINT8 au8Buff[], UINT16 u16Size);
void MX25L64_Read(UINT32 u32Addr, UINT8 au8Buff[], UINT16 u16Size);
void MX25L64_Sync(void);
void MX25L64_Erase(void);

// Set MX25L64 status
void MX25L64_SetPower(UINT8 u8State);

// Get MX25L64 information
UINT32 MX25L64_GetId(void);
UINT32 MX25L64_GetCapacity(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
