#ifndef __LWIP_APP_UDP_H__
#define __LWIP_APP_UDP_H__


#define UDP_APP_RX_BUFF_SIZE  (200)


enum enUdpState
{
	enUdpInitOK,
	enUdpSendOK,
	enUdpRecvOK,
	enUdpObjCrtErr,
	enUdpConnErr,
	enUdpBindErr,
	enUdpSendErr,
	enUdpRecvErr,
	
};


extern struct udp_pcb stcLwipUdpPcb;


err_t lwip_app_udp_init(void);

void lwip_app_udp_client(void);

void lwip_app_udp_recv_data(void *arg, struct udp_pcb *pcb, struct pbuf *p,
							const ip_addr_t *addr, u16_t port);

void lwip_app_udp_send_data(struct udp_pcb *pUdpPcb, const char *pData, uint16_t length);

void lwip_app_udp_conn_close(struct udp_pcb *pUdpPcb);

#endif
