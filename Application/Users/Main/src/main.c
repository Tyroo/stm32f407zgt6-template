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

	Nvic_Init(2);				// 中断分组，中断分组2
	Led_Init();					// 初始化LED模块
	Delay_Init();				// 初始化延时模块
	Uart1_Init(115200);	// 初始化USAER1模块
	Timer3_Init(999, 839);
	FsmcSram_Init();
	my_mem_init(SRAMIN);	//初始化内部内存池

	LAN8720_Init();
	
	TFTLCD_Init();	// 初始化LCDTFT模块
	
	// LWIP初始化
	lwip_app_init();
	// LWIP UDP通信初始化
	stcLwipObject.udpstatus = lwip_app_udp_init();
    
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
		
//		if (USB_Manage.State == 0)
//		{
//			USBH_Process(&USB_OTG_Core, &USB_Host);
//		}
//		else 
//		{
//			Led_Control(0);
//		}
		lwip_app_udp_client();
	}
}
