#include "user_app/inc/lwip_app_comm.h"


LwipObjectTypeDef stcLwipObject;
struct netif lwip_netif;		//����һ��ȫ�ֵ�����ӿ�
struct dhcp* dhcp;				//����һ��DHCPȫ��ָ��

uint32_t TCPTimer = 0;			//TCP��ѯ��ʱ��
uint32_t ARPTimer = 0;			//ARP��ѯ��ʱ��
uint32_t lwip_localtime;		//lwip����ʱ�������,��λ:ms

#if LWIP_DHCP
uint32_t DHCPfineTimer = 0;		//DHCP��ϸ�����ʱ��
uint32_t DHCPcoarseTimer = 0;	//DHCP�ֲڴ����ʱ��
#endif


// lwip APP��ʼ��
void lwip_app_init(void)
{
	ip_addr_t ipaddr;  					// IP��ַ
	ip_addr_t netmask; 					// ��������
	ip_addr_t gw;      					// Ĭ������

	/* Initilialize the LwIP stack without RTOS */
	lwip_init();
	/* Default MAC address��IP address, gateway, subnet mask settings */
	lwip_app_default_ip_set(&stcLwipObject);
	
	/* USER CODE BEGIN 0 */
	
#ifdef LWIP_UDP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
	IP4_ADDR(&ipaddr,stcLwipObject.ip[0],stcLwipObject.ip[1],stcLwipObject.ip[2],stcLwipObject.ip[3]);
	IP4_ADDR(&netmask,stcLwipObject.netmask[0],stcLwipObject.netmask[1] ,stcLwipObject.netmask[2],stcLwipObject.netmask[3]);
	IP4_ADDR(&gw,stcLwipObject.gateway[0],stcLwipObject.gateway[1],stcLwipObject.gateway[2],stcLwipObject.gateway[3]);
#endif
	
	/* USER CODE END 0 */
	
	/* add the network interface (IPv4/IPv6) without RTOS */
	netif_add(&lwip_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/* Registers the default network interface */
	netif_set_default(&lwip_netif);	
	
	if (netif_is_link_up(&lwip_netif))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&lwip_netif);
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&lwip_netif);
	}
	
	/* USER CODE BEGIN 1 */
	
	/* DHCP get ip address */
#ifdef LWIP_UDP
	
	stcLwipObject.dhcpstatus = 0;	// DHCP���Ϊ0

	while((stcLwipObject.dhcpstatus != 2) && (stcLwipObject.dhcpstatus != 0XFF))
	{
		lwip_periodic_handle();
	}

#endif /* LWIP_UDP */
	
	/* USER CODE END 1 */
}


void lwip_app_default_ip_set(LwipObjectTypeDef *pObject)
{
	uint32_t sn0;
	sn0 = *(vu32*)(0x1FFF7A10);//��ȡSTM32��ΨһID��ǰ24λ��ΪMAC��ַ�����ֽ�
	
	// Ĭ��Զ��IPΪ:192.168.1.100
	pObject->remoteip[0] = 192;	
	pObject->remoteip[1] = 168;
	pObject->remoteip[2] = 0;
	pObject->remoteip[3] = 100;
	
	// MAC��ַ����(�����ֽڹ̶�Ϊ:2.0.0,�����ֽ���STM32ΨһID)
	// �����ֽ�(IEEE��֮Ϊ��֯ΨһID,OUI)��ַ�̶�Ϊ:2.0.0
	pObject->mac[0] = 2;
	pObject->mac[1] = 0;
	pObject->mac[2] = 0;
	pObject->mac[3] = (sn0>>16)&0XFF;//�����ֽ���STM32��ΨһID
	pObject->mac[4] = (sn0>>8)&0XFFF;;
	pObject->mac[5] = sn0&0XFF; 
	
	//Ĭ�ϱ���IPΪ:192.168.1.30
	pObject->ip[0] = 192;	
	pObject->ip[1] = 168;
	pObject->ip[2] = 0;
	pObject->ip[3] = 99;
	
	// Ĭ����������:255.255.255.0
	pObject->netmask[0] = 255;	
	pObject->netmask[1] = 255;
	pObject->netmask[2] = 255;
	pObject->netmask[3] = 0;
	
	// Ĭ������:192.168.1.1
	pObject->gateway[0] = 192;	
	pObject->gateway[1] = 168;
	pObject->gateway[2] = 0;
	pObject->gateway[3] = 1;
}


void lwip_periodic_handle(void)
{
#if LWIP_TCP
	//ÿ250ms����һ��tcp_tmr()����
	if (lwip_localtime - TCPTimer >= TCP_TMR_INTERVAL)
	{
		TCPTimer =  lwip_localtime;
		tcp_tmr();
	}
	
#endif
	
	//ARPÿ5s�����Ե���һ��
	if ((lwip_localtime - ARPTimer) >= ARP_TMR_INTERVAL)
	{
		ARPTimer = lwip_localtime;
		etharp_tmr();
	}

#if LWIP_DHCP
	//ÿ500ms����һ��dhcp_fine_tmr()
	if (lwip_localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
	{
		DHCPfineTimer =  lwip_localtime;
		dhcp_fine_tmr();
		lwip_dhcp_process_handle();  //DHCP����
	}

	//ÿ60sִ��һ��DHCP�ֲڴ���
	if (lwip_localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
	{
		DHCPcoarseTimer =  lwip_localtime;
		dhcp_coarse_tmr();
	}  

#endif
}


void lwip_dhcp_process_handle(void)
{
	uint32_t ip = 0,netmask = 0,gw = 0;
	
	switch(stcLwipObject.dhcpstatus)
	{
	case 0: //���� DHCP
		dhcp_start(&lwip_netif);
		stcLwipObject.dhcpstatus = 1; //�ȴ�ͨ�� DHCP ��ȡ���ĵ�ַ
		break;
	case 1: //�ȴ���ȡ�� IP ��ַ
	{
		ip = lwip_netif.ip_addr.addr; 		//��ȡ�� IP ��ַ
		netmask = lwip_netif.netmask.addr;	//��ȡ��������
		gw = lwip_netif.gw.addr; 			//��ȡĬ������
		
		if(ip != 0) //��ȷ��ȡ�� IP ��ַ��ʱ��
		{
			stcLwipObject.dhcpstatus = 2; //DHCP �ɹ�

			//������ͨ�� DHCP ��ȡ���� IP ��ַ
			stcLwipObject.ip[3]=(uint8_t)(ip>>24); 
			stcLwipObject.ip[2]=(uint8_t)(ip>>16);
			stcLwipObject.ip[1]=(uint8_t)(ip>>8);
			stcLwipObject.ip[0]=(uint8_t)(ip);

			//����ͨ�� DHCP ��ȡ�������������ַ
			stcLwipObject.netmask[3]=(uint8_t)(netmask>>24);
			stcLwipObject.netmask[2]=(uint8_t)(netmask>>16);
			stcLwipObject.netmask[1]=(uint8_t)(netmask>>8);
			stcLwipObject.netmask[0]=(uint8_t)(netmask);

			//������ͨ�� DHCP ��ȡ����Ĭ������
			stcLwipObject.gateway[3]=(uint8_t)(gw>>24);
			stcLwipObject.gateway[2]=(uint8_t)(gw>>16);
			stcLwipObject.gateway[1]=(uint8_t)(gw>>8);
			stcLwipObject.gateway[0]=(uint8_t)(gw);
		}	
		else
		{
			dhcp = netif_dhcp_data(&lwip_netif); 
			
			if (dhcp->tries > LWIP_MAX_DHCP_TRIES)
			{
				//ͨ�� DHCP �����ȡ IP ��ַʧ��,�ҳ�������Դ���
				stcLwipObject.dhcpstatus = 0XFF;//DHCP ��ʱʧ��.
				//ʹ�þ�̬ IP ��ַ
				IP4_ADDR(&(lwip_netif.ip_addr),stcLwipObject.ip[0],stcLwipObject.ip[1],\
				stcLwipObject.ip[2],stcLwipObject.ip[3]);
				IP4_ADDR(&(lwip_netif.netmask),stcLwipObject.netmask[0],\
				stcLwipObject.netmask[1],stcLwipObject.netmask[2],stcLwipObject.netmask[3]);
				IP4_ADDR(&(lwip_netif.gw),stcLwipObject.gateway[0],stcLwipObject.gateway[1],\
				stcLwipObject.gateway[2],stcLwipObject.gateway[3]);
			}
		}
	}
	break;
	default : break;
	}
}
