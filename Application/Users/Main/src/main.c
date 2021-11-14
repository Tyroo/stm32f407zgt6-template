#include "main.h"


uint16_t Fsmc_Rx_Buff[1] = {0x61};


const uint8_t IIC_Tx_Buff[] = {"AABBCCDD ^0^"};

uint8_t SendChar;

uint8_t WordsArr[32] = {0x04,0x80,0x0E,0xA0,0x78,0x90,0x08,0x90,
      0x08,0x84,0xFF,0xFE,0x08,0x80,0x08,0x90,
      0x0A,0x90,0x0C,0x60,0x18,0x40,0x68,0xA0,
      0x09,0x20,0x0A,0x14,0x28,0x14,0x10,0x0C};

extern FRESULT F_Sate;
extern FATFS* Fs;
extern FIL*   Fi;
extern UINT FatfsReadSize;
extern UINT FatfsWriteSize;
extern BYTE* ReadTextBuff;
extern char* WriteTextBuff;
extern BYTE Work[FF_MAX_SS];

//int USBH_USR_Application()
//{
//	/* USB�û�Ӧ�ó��� */
//	return 0;
//}
extern USBH_HOST              USB_Host;
extern USB_OTG_CORE_HANDLE    USB_OTG_Core;
extern USB_ManageType		  USB_Manage;
	  
#define SIZE sizeof(IIC_Tx_Buff)
uint8_t IIC_Rx_Buff[SIZE];



int main() {
	
	uint8_t KeyCode;					// ��ť״̬����
	bool IIC_Flag;
	uint16_t LastAddr = 255;
	uint8_t  LastData = 0x55;
	
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
//	my_mem_init(SRAMIN);
	FsmcSram_Init();
//	CAN1_Config();			// ��ʼ��CAN1
//	SPI1_Init();			// SPI1��ʼ��
//	
//	setNodeId(&TestSlave_Data, 1);
//	setState(&TestSlave_Data, Initialisation);
	
	TFTLCD_Init();	// ��ʼ��LCDTFTģ��
	IIC_Init();		// ��ʼ��IICģ��
	
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
//	FatfsWriteSize = 0;
//	FatfsReadSize = 0;
//	Fs = (FATFS*)mymalloc(SRAMIN, sizeof(FATFS));
//	Fi = (FIL*)mymalloc(SRAMIN, sizeof(FIL));
//	ReadTextBuff = (BYTE*)mymalloc(SRAMIN, 512);
//	WriteTextBuff = "wrty2y55ytuu68766tytiiook\r\n";
//	
//	F_Sate = f_mount(Fs, "2:", 0);
//	
//	if (F_Sate == FR_OK)
//	{
//		printf("�ļ�ϵͳ���سɹ���\r\n");
//	}
//	else
//	{
//		printf("�ļ�ϵͳ����ʧ�ܣ�\r\n");
//		printf("ʧ��ԭ��%d\r\n", F_Sate);
//	}
//	
//	if (F_Sate == FR_NO_FILESYSTEM)
//	{
//		printf("���ڸ�ʽ���ļ�ϵͳ...\r\n");
//		F_Sate = f_mkfs("2:", 0, Work, sizeof(Work));
//		if (F_Sate == FR_OK)
//		{
//			F_Sate = f_mount(NULL, "2:", 1);
//			F_Sate = f_mount(Fs, "2:", 1);
//			printf("��ʽ���ļ�ϵͳ���...\r\n");
//		}
//	}

//	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_USBH_cb);
	
	Led_Control(0);

//	FatfsResult = f_open(&File, "2:123.txt", FA_OPEN_EXISTING|FA_READ);
//	FatfsResult = f_read(&File, ReadTextBuff, f_size(&File), &FatfsReadSize);
	
//	if (FatfsResult != FR_OK)
//	{
//		Uart1_Send("�ļ�ϵͳ����ʧ�ܣ�");
//	}
//	else
//	{
////		Uart1_Send(ReadTextBuff);
//		Uart1_Send("aaaa");
//	}
	
	IIC_Flag = AT24C02_Rx_Data(IIC_Rx_Buff, LastAddr, 1);
	if ((*IIC_Rx_Buff) != 0x55)
	{
		IIC_Flag = AT24C02_Tx_Data(&LastData, LastAddr, 1);
		if (IIC_Flag)
			Uart1_Send("AT24CXX��ʼ���ɹ���");
		else
			Uart1_Send("AT24CXX��ʼ��ʧ�ܣ�");
	}
	else
	{
		Uart1_Send("AT24CXX����ɳ�ʼ����");
	}


	while(1) {
		
		KeyCode = Key_Scan(0);	// ����ɨ��
		
		if (KeyCode == 1) 
		{
			Uart1_Send("������AT24CXXд������...\r\n"); 
			IIC_Flag = AT24C02_Tx_Data((uint8_t*)IIC_Tx_Buff, 0, SIZE);
			if (IIC_Flag == True)
			{
				Uart1_Send("д��ɹ���\r\n"); 
			}
			else
			{
				Uart1_Send("д��ʧ�ܣ�\r\n");
			}
//			Led_Control(1);
//			F_Sate = f_open(Fi, "2:/123.txt", FA_READ|FA_WRITE);


//			F_Sate = f_write(Fi, WriteTextBuff, 16, &FatfsWriteSize);
//			F_Sate = f_read(Fi, ReadTextBuff, 8, &FatfsReadSize);
//			
//			Delay_Ms(10);
//			
//			if (F_Sate == FR_OK)
//			{
//				Uart1_Send("�����ļ��ɹ�\r\n");
//				Uart1_Send("�ļ����ݣ�\r\n");
//				Uart1_Send((char*)ReadTextBuff);
//				F_Sate = f_mkdir("2:/NewDir");
//				if (!F_Sate) Uart1_Send("�����ļ��гɹ���\r\n");
//				
//			}
//			else
//			{
//				Uart1_Send("��ȡ�ļ�ʧ��\r\n");
//				printf("����ԭ��%d", F_Sate);
//			}
//			f_close(Fi);
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
		else if (KeyCode == 2)
		{
			Uart1_Send("���ڶ�ȡAT24CXX����...\r\n");
			IIC_Flag = AT24C02_Rx_Data(IIC_Rx_Buff, 0, SIZE);
			if (IIC_Flag == True)
			{
//				IIC_Rx_Buff[6] = '\0';
				Uart1_Send("��ȡ�ɹ�:");
				Uart1_Send((char*)IIC_Rx_Buff);
				Uart1_Send("\r\n");
			}
			else
			{
				Uart1_Send("��ȡʧ�ܣ�\r\n");
			}
		}
//		if (USB_Manage.State == 0)
//		{
//			USBH_Process(&USB_OTG_Core, &USB_Host);
//		}
//		else 
//		{
//			Led_Control(0);
//		}
		
	}
}
