#include "lwip_app.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"


void LWIP_Appliction(void)
{
#if LWIP_DHCP   //ʹ��DHCP
	if((lwipdev.dhcpstatus != 2) && (lwipdev.dhcpstatus != 0XFF))//�ȴ�DHCP��ȡ�ɹ�/��ʱ���
#endif
	{
		lwip_periodic_handle();	//LWIP�ں���Ҫ��ʱ����ĺ���
	}
}
