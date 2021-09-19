#include "main.h"


u16 Fsmc_Rx_Buff[1] = {0x61};
u8 SendChar;


int main() {
	
	u8 KeyCode;					// ��ť״̬����
	CanTxMsg CANTxMessage;

	Nvic_Init(2);				// �жϷ��飬�жϷ���2
	Delay_Init();				// ��ʼ����ʱģ��
	Led_Init();					// ��ʼ��LEDģ��
	Key_Init();					// ����ģ���ʼ��
	Led_Control(1);
	Uart1_Init(115200);	// ��ʼ��USAER1ģ��
//	FsmcSram_Init();
	CAN1_Config();			// ��ʼ��CAN1
//	SPI1_Init();			// SPI1��ʼ��
//	
//	setNodeId(&TestSlave_Data, 1);
//	setState(&TestSlave_Data, Initialisation);
	
	CANTxMessage.DLC = 2;					// ���ݳ���
	CANTxMessage.Data[0] = 0x01;	// ���ݵ�λ
	CANTxMessage.Data[1] = 0x02;	// ���ݸ�λ
	CANTxMessage.IDE = 0;					// ��ע��ʶ��ģʽ
	CANTxMessage.StdId = 0x12;
	CANTxMessage.ExtId = 0x12;		// ��չ��ʶ��Ϊ0
	CANTxMessage.RTR = 0;					// ����֡
	
	while(1) {
		
		KeyCode = Key_Scan(0);	// ����ɨ��
		
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
