#include "at24c02.h"


// 发送一个字符串给从设备
bool AT24C02_WriteData(uint8_t u8Addr, uint8_t * au8Buff, uint8_t u8Size)
{
	return (bool)(I2C_WriteRegister(0xA0, u8Addr, au8Buff, u8Size));
}


// 接收一个字符串
bool AT24C02_ReadData(uint8_t u8Addr, uint8_t * au8Buff, uint8_t u8Size)
{
	return (bool)(I2C_ReadRegister(0xA0, u8Addr, au8Buff, u8Size));
}
