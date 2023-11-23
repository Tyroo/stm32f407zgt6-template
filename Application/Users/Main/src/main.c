#include "main.h"


int main() 
{
	Nvic_Init(2);		// 中断分组，中断分组2
	
	Led_Init();			// 初始化LED模块
	
	Uart1_Init(115200);	// 初始化USAER1模块
	Timer3_Init(99, 839);
	FsmcSram_Init();
	
	DMA1_C0S0_5_Init();
	SPI3_Init();
	
	LAN8720_Init();
	TFTLCD_Init();		// 初始化LCDTFT模块
	// LWIP初始化
	lwip_app_init();
	// LWIP UDP通信初始化
	stcLwipObject.udpstatus = lwip_app_udp_init();
	// LWIP TCP通信初始化
	lwip_app_tcp_client_init();
	
	USBD_Init(&USB_Core, USB_OTG_FS_CORE_ID, &USBD_Msc, &USBD_MSC_cb, &USBD_Usr_cb);
	
	my_mem_init(SRAMIN);//初始化内部内存池
	Delay_Init();		// 初始化延时模块
	
	Keyboard_Init();
	
	for( ; ; )
	{
		
	}
}
