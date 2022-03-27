#ifndef __LWIP_COMM__
#define __LWIP_COMM__

#include <stdint.h>

#include "malloc.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/etharp.h"

#include "ethernetif.h"

#include "netif/ethernet.h"

#include "lan8720.h"

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
} __lwip_dev;


extern __lwip_dev lwipdev;


uint8_t lwip_comm_mem_malloc(void);
uint8_t lwip_comm_init(void);

void lwip_comm_mem_free(void);
void lwip_comm_default_ip_set(__lwip_dev *lwipx);
void lwip_pkt_handle(void);
void lwip_periodic_handle(void);
void lwip_dhcp_process_handle(void);

#endif
