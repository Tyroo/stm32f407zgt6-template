#include "user_app/inc/lwip_app_comm.h"


LwipObjectTypeDef stcLwipObject;
struct netif lwip_netif;		//定义一个全局的网络接口
struct dhcp* dhcp;				//定义一个DHCP全局指针

uint32_t TCPTimer = 0;			//TCP查询计时器
uint32_t ARPTimer = 0;			//ARP查询计时器
uint32_t lwip_localtime;		//lwip本地时间计数器,单位:ms

#if LWIP_DHCP
uint32_t DHCPfineTimer = 0;		//DHCP精细处理计时器
uint32_t DHCPcoarseTimer = 0;	//DHCP粗糙处理计时器
#endif


// lwip APP初始化
void lwip_app_init(void)
{
	ip_addr_t ipaddr;  					// IP地址
	ip_addr_t netmask; 					// 子网掩码
	ip_addr_t gw;      					// 默认网关

	/* Initilialize the LwIP stack without RTOS */
	lwip_init();
	/* Default MAC address，IP address, gateway, subnet mask settings */
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
	
	stcLwipObject.dhcpstatus = 0;	// DHCP标记为0

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
	sn0 = *(vu32*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	
	// 默认远端IP为:192.168.1.100
	pObject->remoteip[0] = 192;	
	pObject->remoteip[1] = 168;
	pObject->remoteip[2] = 0;
	pObject->remoteip[3] = 100;
	
	// MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	// 高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	pObject->mac[0] = 2;
	pObject->mac[1] = 0;
	pObject->mac[2] = 0;
	pObject->mac[3] = (sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	pObject->mac[4] = (sn0>>8)&0XFFF;;
	pObject->mac[5] = sn0&0XFF; 
	
	//默认本地IP为:192.168.1.30
	pObject->ip[0] = 192;	
	pObject->ip[1] = 168;
	pObject->ip[2] = 0;
	pObject->ip[3] = 99;
	
	// 默认子网掩码:255.255.255.0
	pObject->netmask[0] = 255;	
	pObject->netmask[1] = 255;
	pObject->netmask[2] = 255;
	pObject->netmask[3] = 0;
	
	// 默认网关:192.168.1.1
	pObject->gateway[0] = 192;	
	pObject->gateway[1] = 168;
	pObject->gateway[2] = 0;
	pObject->gateway[3] = 1;
}


void lwip_periodic_handle(void)
{
#if LWIP_TCP
	//每250ms调用一次tcp_tmr()函数
	if (lwip_localtime - TCPTimer >= TCP_TMR_INTERVAL)
	{
		TCPTimer =  lwip_localtime;
		tcp_tmr();
	}
	
#endif
	
	//ARP每5s周期性调用一次
	if ((lwip_localtime - ARPTimer) >= ARP_TMR_INTERVAL)
	{
		ARPTimer = lwip_localtime;
		etharp_tmr();
	}

#if LWIP_DHCP
	//每500ms调用一次dhcp_fine_tmr()
	if (lwip_localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
	{
		DHCPfineTimer =  lwip_localtime;
		dhcp_fine_tmr();
		lwip_dhcp_process_handle();  //DHCP处理
	}

	//每60s执行一次DHCP粗糙处理
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
	case 0: //开启 DHCP
		dhcp_start(&lwip_netif);
		stcLwipObject.dhcpstatus = 1; //等待通过 DHCP 获取到的地址
		break;
	case 1: //等待获取到 IP 地址
	{
		ip = lwip_netif.ip_addr.addr; 		//读取新 IP 地址
		netmask = lwip_netif.netmask.addr;	//读取子网掩码
		gw = lwip_netif.gw.addr; 			//读取默认网关
		
		if(ip != 0) //正确获取到 IP 地址的时候
		{
			stcLwipObject.dhcpstatus = 2; //DHCP 成功

			//解析出通过 DHCP 获取到的 IP 地址
			stcLwipObject.ip[3]=(uint8_t)(ip>>24); 
			stcLwipObject.ip[2]=(uint8_t)(ip>>16);
			stcLwipObject.ip[1]=(uint8_t)(ip>>8);
			stcLwipObject.ip[0]=(uint8_t)(ip);

			//解析通过 DHCP 获取到的子网掩码地址
			stcLwipObject.netmask[3]=(uint8_t)(netmask>>24);
			stcLwipObject.netmask[2]=(uint8_t)(netmask>>16);
			stcLwipObject.netmask[1]=(uint8_t)(netmask>>8);
			stcLwipObject.netmask[0]=(uint8_t)(netmask);

			//解析出通过 DHCP 获取到的默认网关
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
				//通过 DHCP 服务获取 IP 地址失败,且超过最大尝试次数
				stcLwipObject.dhcpstatus = 0XFF;//DHCP 超时失败.
				//使用静态 IP 地址
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
