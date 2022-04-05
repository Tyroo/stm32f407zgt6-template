#include "lwip_app.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"


void LWIP_Appliction(void)
{
#if LWIP_DHCP   //使用DHCP
	while((lwipdev.dhcpstatus != 2) && (lwipdev.dhcpstatus != 0XFF))//等待DHCP获取成功/超时溢出
#else
	while(1)
#endif
	{
		lwip_periodic_handle();	//LWIP内核需要定时处理的函数
	}
}
