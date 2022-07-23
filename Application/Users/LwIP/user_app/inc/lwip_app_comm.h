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





#define LWIP_MAX_DHCP_TRIES		4   //DHCP������������Դ���

typedef volatile unsigned char vuint8_t;


typedef struct 
{
	uint8_t mac[6]; 		//MAC ��ַ
	uint8_t remoteip[4]; 	//Զ������ IP ��ַ
	uint8_t ip[4]; 			//���� IP ��ַ
	uint8_t netmask[4]; 	//��������
	uint8_t gateway[4]; 	//Ĭ�����ص� IP ��ַ
	vuint8_t dhcpstatus; 	// DHCP״̬��{ 0��ά��żDHCP��ַ��1������DHCP��ȡ״̬��2���ɹ���ȡDHCP��ַ��0xff����ȡDHCP��ַʧ�� }
	
	int8_t udpstatus;
} LwipObjectTypeDef;


extern struct netif lwip_netif;
extern LwipObjectTypeDef stcLwipObject;



void lwip_app_init(void);

void lwip_app_default_ip_set(LwipObjectTypeDef *lwipx);

void lwip_periodic_handle(void);

void lwip_dhcp_process_handle(void);

#endif
