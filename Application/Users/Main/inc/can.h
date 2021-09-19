#ifndef __CAN_H__
#define __CAN_H__

#include "stm32f4xx_can.h"
#include "nvic.h"
#include "TestSlave.h"


void CAN1_Config(void);

unsigned char CAN1_Send_Msg(CanTxMsg* TxMsg);

//unsigned char CAN1_Send_Msg(Message* Msg);

#endif
