#include "crc.h"


// CRC16校验
unsigned short CRC16(unsigned char au8Buff[], unsigned short u16BuffSize)
{
	unsigned short u16Temp;
	unsigned short u16CRC = 0xFFFF;
	
	for(unsigned short i = 0; i < u16BuffSize; i++)
	{
		u16Temp = *(au8Buff + i) & 0x00FF;
		u16CRC ^= u16Temp;
		
		for(unsigned char j = 0; j < 8; j++)
		{
			if(u16CRC & 0x0001)
			{
				u16CRC >>= 1;
				u16CRC ^= 0xA001;
			}
			else
			{
				u16CRC >>= 1;
			}
		}
	}
	return (u16CRC);
}
