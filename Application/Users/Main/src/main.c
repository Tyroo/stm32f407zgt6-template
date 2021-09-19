#include "main.h"


u16 Fsmc_Rx_Buff[1] = {0x61};
u8 SendChar;


int main() {
	
	u8 KeyCode;					// 按钮状态定义
	CanTxMsg CANTxMessage;

	Nvic_Init(2);				// 中断分组，中断分组2
	Delay_Init();				// 初始化延时模块
	Led_Init();					// 初始化LED模块
	Key_Init();					// 按键模块初始化
	Led_Control(1);
	Uart1_Init(115200);	// 初始化USAER1模块
//	FsmcSram_Init();
	CAN1_Config();			// 初始化CAN1
//	SPI1_Init();			// SPI1初始化
//	
//	setNodeId(&TestSlave_Data, 1);
//	setState(&TestSlave_Data, Initialisation);
	
	CANTxMessage.DLC = 2;					// 数据长度
	CANTxMessage.Data[0] = 0x01;	// 数据低位
	CANTxMessage.Data[1] = 0x02;	// 数据高位
	CANTxMessage.IDE = 0;					// 标注标识符模式
	CANTxMessage.StdId = 0x12;
	CANTxMessage.ExtId = 0x12;		// 扩展标识符为0
	CANTxMessage.RTR = 0;					// 数据帧
	
	while(1) {
		
		KeyCode = Key_Scan(0);	// 按键扫描
		
		if (KeyCode == 1) {
			CAN1_Send_Msg(&CANTxMessage);
//			FsmcSram_Write(Fsmc_Rx_Buff, 0, 1);
//			Led_Control(1);
//			SendChar = (u8)FsmcSram_Read(0);
//			Uart1_Send((char*)&SendChar);
			
		} 
		else {
			Led_Control(0);
		}
	}
}
