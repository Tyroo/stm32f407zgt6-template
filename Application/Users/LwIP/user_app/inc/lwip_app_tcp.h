#ifndef __LWIP_APP_TCP_H__
#define __LWIP_APP_TCP_H__

#define TCP_APP_CLIENT_RX_BUFF_SIZE  (200)

//tcp服务器连接状态
enum enTcpClientConnStatus
{
	enTcpClientNone,	//没有连接
	enTcpClientClose,	//关闭连接
	enTcpClientConn,	//连接成功
	enTcpClientRecvOK,
	enTcpClientSendOK,
	enTcpClientAckWait,
	enTcpClientRecvErr,
	enTcpClientSendErr,
}; 


err_t lwip_app_tcp_client_init(void);

err_t lwip_app_tcp_client_connect(void *arg, struct tcp_pcb *pTcpPcb, err_t err);

err_t lwip_app_tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err);

void lwip_app_tcp_client_error(void *arg,err_t err);

err_t lwip_app_tcp_client_poll(void *arg, struct tcp_pcb *tpcb);

err_t lwip_app_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

void lwip_app_tcp_client_send_data(struct tcp_pcb *tpcb, struct pbuf *pData, uint8_t *pStatus);

void lwip_app_tcp_client_conn_close(struct tcp_pcb *pTcpPcb);

#endif
