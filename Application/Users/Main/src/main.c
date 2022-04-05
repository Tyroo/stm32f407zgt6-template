#include "main.h"


uint16_t Fsmc_Rx_Buff[1] = {0x61};


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

int USBH_USR_Application()
{
	/* USB用户应用程序 */
	return 0;
}
extern USBH_HOST              USB_Host;
extern USB_OTG_CORE_HANDLE    USB_OTG_Core;
extern USB_ManageType		  USB_Manage;



int main() {
	
	uint8_t KeyCode;					// 按钮状态定义
	
//	CanTxMsg CANTxMessage;

	uint8_t EleBoxIndex;
	
	Ele_Box_PropType BoxProp;
	Ele_Box_ObjType  BoxObj;
	
	BoxProp = (Ele_Box_PropType){20, 200, 0xFF0000, 1, { 10, 10, 0, 0xFFFF00 }};
	
	CreateBoxObject(&BoxObj, BoxProp);
	
	Nvic_Init(2);				// 中断分组，中断分组2
	Delay_Init();				// 初始化延时模块
	Led_Init();					// 初始化LED模块
	Key_Init();					// 按键模块初始化
	Uart1_Init(115200);	// 初始化USAER1模块
	FsmcSram_Init();
	
//	CAN1_Config();			// 初始化CAN1
//	SPI1_Init();			// SPI1初始化
//	
//	setNodeId(&TestSlave_Data, 1);
//	setState(&TestSlave_Data, Initialisation);
	
	
//	IIC_Init();		// 初始化IIC模块
	
	Delay_Ms(100);
	
	Timer3_Init(999, 839);
	
	my_mem_init(SRAMIN);	//初始化内部内存池
	my_mem_init(SRAMEX);	//初始化外部内存池
	my_mem_init(SRAMCCM);	//初始化CCM内存池
	
	TFTLCD_Init();	// 初始化LCDTFT模块
	

	
	if(lwip_comm_init()) //lwip 初始化
	{
		Led_Control(1);
	}

	
    TFTLCD_SetWindow(10, 11, 10, 11);
	BoxObj.SetBg(0xFF0000, 80, 80, 1, &BoxObj);
    BoxObj.SetBd(0xFF0000, 5, 10, 0, &BoxObj);
    BoxProp.Ele_Box_Color = 0x0000FF;
    BoxObj.SetBg(0xFFFF00, 80, 80, 1, &BoxObj);
	BoxObj.Display(40, 40, &BoxObj);
    
    
	
//	for(EleBoxIndex=0;EleBoxIndex<6;EleBoxIndex++) {
//		EleBox.Ele_BoxProp.Ele_Box_Height -= 35;
//		DrawBox(EleBoxIndex*40, 20, EleBox);
//	}
	
//	CANTxMessage.DLC = 2;					// 数据长度
//	CANTxMessage.Data[0] = 0x01;	// 数据低位
//	CANTxMessage.Data[1] = 0x02;	// 数据高位
//	CANTxMessage.IDE = 0;					// 标注标识符模式
//	CANTxMessage.StdId = 0x12;
//	CANTxMessage.ExtId = 0x12;		// 扩展标识符为0
//	CANTxMessage.RTR = 0;					// 数据帧
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
//		printf("文件系统挂载成功！\r\n");
//	}
//	else
//	{
//		printf("文件系统挂载失败！\r\n");
//		printf("失败原因：%d\r\n", F_Sate);
//	}
//	
//	if (F_Sate == FR_NO_FILESYSTEM)
//	{
//		printf("正在格式化文件系统...\r\n");
//		F_Sate = f_mkfs("2:", 0, Work, sizeof(Work));
//		if (F_Sate == FR_OK)
//		{
//			F_Sate = f_mount(NULL, "2:", 1);
//			F_Sate = f_mount(Fs, "2:", 1);
//			printf("格式化文件系统完成...\r\n");
//		}
//	}

//	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_USBH_cb);
	
	Led_Control(0);

//	FatfsResult = f_open(&File, "2:123.txt", FA_OPEN_EXISTING|FA_READ);
//	FatfsResult = f_read(&File, ReadTextBuff, f_size(&File), &FatfsReadSize);
	
//	if (FatfsResult != FR_OK)
//	{
//		Uart1_Send("文件系统挂载失败！");
//	}
//	else
//	{
////		Uart1_Send(ReadTextBuff);
//		Uart1_Send("aaaa");
//	}


	while(1) {
		
		KeyCode = Key_Scan(0);	// 按键扫描
		
		if (KeyCode == 1) 
		{
			Led_Control(1);
//			F_Sate = f_open(Fi, "2:/123.txt", FA_READ|FA_WRITE);


//			F_Sate = f_write(Fi, WriteTextBuff, 16, &FatfsWriteSize);
//			F_Sate = f_read(Fi, ReadTextBuff, 8, &FatfsReadSize);
//			
//			Delay_Ms(10);
//			
//			if (F_Sate == FR_OK)
//			{
//				Uart1_Send("操作文件成功\r\n");
//				Uart1_Send("文件内容：\r\n");
//				Uart1_Send((char*)ReadTextBuff);
//				F_Sate = f_mkdir("2:/NewDir");
//				if (!F_Sate) Uart1_Send("创建文件夹成功！\r\n");
//				
//			}
//			else
//			{
//				Uart1_Send("读取文件失败\r\n");
//				printf("错误原因：%d", F_Sate);
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
//			Uart1_Send((char*)&SendChar)
			
		}
		else if (KeyCode == 2)
		{
			Led_Control(0);
		}
//		if (USB_Manage.State == 0)
//		{
//			USBH_Process(&USB_OTG_Core, &USB_Host);
//		}
//		else 
//		{
//			Led_Control(0);
//		}
		LWIP_Appliction();
	}
}
