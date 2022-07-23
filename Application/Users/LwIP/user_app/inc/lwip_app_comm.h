#ifndef __LWIP_APP_COMM_H__
#define __LWIP_APP_COMM_H__

#include <stdint.h>

#include "malloc.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/etharp.h"
#include "lwip/netif.h"

#include "lwipopts.h"
#include "ethernetif.h"


#include "netif/ethernet.h"

#include "lan8720.h"
#include "uart.h"

#include "user_app/inc/lwip_app_udp.h"





#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

typedef volatile unsigned char vuint8_t;


typedef struct 
{
	uint8_t mac[6]; 		//MAC 地址
	uint8_t remoteip[4]; 	//远端主机 IP 地址
	uint8_t ip[4]; 			//本机 IP 地址
	uint8_t netmask[4]; 	//子网掩码
	uint8_t gateway[4]; 	//默认网关的 IP 地址
	vuint8_t dhcpstatus; 	// DHCP状态：{ 0：维护偶DHCP地址；1：进入DHCP获取状态；2：成功获取DHCP地址；0xff：获取DHCP地址失败 }
	
	int8_t udpstatus;
} LwipObjectTypeDef;


extern struct netif lwip_netif;
extern LwipObjectTypeDef stcLwipObject;



void lwip_app_init(void);

void lwip_app_default_ip_set(LwipObjectTypeDef *lwipx);

void lwip_periodic_handle(void);

void lwip_dhcp_process_handle(void);

#endif
