#include "main.h"


u16 Fsmc_Rx_Buff[1] = {0x61};
u8 SendChar;

uint8_t WordsArr[32] = {0x04,0x80,0x0E,0xA0,0x78,0x90,0x08,0x90,
      0x08,0x84,0xFF,0xFE,0x08,0x80,0x08,0x90,
      0x0A,0x90,0x0C,0x60,0x18,0x40,0x68,0xA0,
      0x09,0x20,0x0A,0x14,0x28,0x14,0x10,0x0C};

TFTLCD_Word_Font WordFont = { 16, 0, 0xFF0000,0x0000FF };




int main() {
	
	u8 KeyCode;					// ��ť״̬����
//	CanTxMsg CANTxMessage;

//	uint8_t EleBoxIndex;
	
//	TFTLCD_Ele_ObjType EleObj;
//	TFTLCD_Ele_PropType EleProp;
	
//	Ele_Box_PropType BoxProp;
	Ele_Box_ObjType  BoxObj;
	Ele_Box_PropType BoxProp;
	
	BoxProp = (Ele_Box_PropType){20, 200, 0xFF0000, 1, { 10, 10, 0, 0xFFFF00 }};
	
	CreateBoxObject(&BoxObj, BoxProp);
	
	Nvic_Init(2);				// �жϷ��飬�жϷ���2
	Delay_Init();				// ��ʼ����ʱģ��
	Led_Init();					// ��ʼ��LEDģ��
	Key_Init();					// ����ģ���ʼ��
	Led_Control(1);
	Uart1_Init(115200);	// ��ʼ��USAER1ģ��
//	FsmcSram_Init();
//	CAN1_Config();			// ��ʼ��CAN1
//	SPI1_Init();			// SPI1��ʼ��
//	
//	setNodeId(&TestSlave_Data, 1);
//	setState(&TestSlave_Data, Initialisation);
	
	TFTLCD_Init();	// ��ʼ��LCDTFTģ��
	
	Delay_Ms(100);
	
//	TFTLCD_Word(50, 50, WordsArr, WordFont);
	
//	EleObj = CreateEleObj(ELE_BOX_OBJ, &EleProp);
	
//	BoxProp = EleProp.Ele_BoxProp;
//	BoxObj = EleObj.Ele_Box_Obj;
    TFTLCD_SetWindow(10, 11, 10, 11);
//	BoxObj.SetBg(0xFF0000, 80, 80, 1, &BoxObj);
//    BoxObj.SetBd(0xFF0000, 5, 10, 0, &BoxObj);
    BoxProp.Ele_Box_Color = 0x0000FF;
    BoxObj.SetBg(0xFFFF00, 80, 80, 1, &BoxObj);
	BoxObj.Display(40, 40, &BoxObj);
    
    
	
//	for(EleBoxIndex=0;EleBoxIndex<6;EleBoxIndex++) {
//		EleBox.Ele_BoxProp.Ele_Box_Height -= 35;
//		DrawBox(EleBoxIndex*40, 20, EleBox);
//	}
	
	
	
//	CANTxMessage.DLC = 2;					// ���ݳ���
//	CANTxMessage.Data[0] = 0x01;	// ���ݵ�λ
//	CANTxMessage.Data[1] = 0x02;	// ���ݸ�λ
//	CANTxMessage.IDE = 0;					// ��ע��ʶ��ģʽ
//	CANTxMessage.StdId = 0x12;
//	CANTxMessage.ExtId = 0x12;		// ��չ��ʶ��Ϊ0
//	CANTxMessage.RTR = 0;					// ����֡

	while(1) {
		
		KeyCode = Key_Scan(0);	// ����ɨ��
		
		if (KeyCode == 1) {
//            Color += 500;
//            if(Color > 0xFFFFFF)
//                Color = 0x0000FF;
//            BoxObj.SetBg(Color, 80, 80, 1, &BoxObj);
//            BoxObj.Display(40, 20, &BoxObj);
//			CAN1_Send_Msg(&CANTxMessage);
//			FsmcSram_Write(Fsmc_Rx_Buff, 0, 1);
//			Led_Control(1);
//			SendChar = (u8)FsmcSram_Read(0);
//			Uart1_Send((char*)&SendChar);
			
		} 
		else {
			Led_Control(1);
		}
		
	}
}
