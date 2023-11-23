#ifndef __AT24C02_H__
#define __AT24C02_H__


#include <stdbool.h>
#include "i2c.h"


bool AT24C02_WriteData(uint8_t u8Addr, uint8_t * au8Buff, uint8_t u8Size);
bool AT24C02_ReadData(uint8_t u8Addr, uint8_t * au8Buff, uint8_t u8Size);


#endif

