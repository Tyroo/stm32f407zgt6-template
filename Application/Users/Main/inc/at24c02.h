#ifndef __AT24C02_H__
#define __AT24C02_H__


#include "i2c.h"


bool AT24C02_TxData(uint8_t* Data, uint8_t Addr, uint8_t TxLen);

bool AT24C02_RxData(uint8_t* Buff, uint8_t Addr, uint8_t RxLen);


#endif

