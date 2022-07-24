#include "user_app/inc/lwip_app_comm.h"


const char *tcp_app_client_send_buff = "hello world!\r\n";	//UDP�������ݻ�����
uint8_t tcp_app_client_recv_buff[TCP_APP_CLIENT_RX_BUFF_SIZE];	//UDP�������ݻ�����

struct tcp_pcb *pstcLwipTcpPcb;

ip_addr_t stcRemoteIpAddr;

uint8_t tcpStatus = 0;


// TCPͨ�ų�ʼ��
err_t lwip_app_tcp_client_init(void)
{
	err_t err;
	
	uint16_t numUdpPort = 8089;
	
	// ����Ĭ��IP��ַ
	lwip_app_remote_ip_set(192, 168, 0, 101);
	
	// �½�һ��TCP_PCB����
	pstcLwipTcpPcb = tcp_new();
	
	if (pstcLwipTcpPcb != NULL)
	{
		IP_ADDR4(
			&stcRemoteIpAddr, 
			stcLwipObject.remoteip[0],
			stcLwipObject.remoteip[1],
			stcLwipObject.remoteip[2],
			stcLwipObject.remoteip[3]
		);
		
		err = tcp_connect(pstcLwipTcpPcb, &stcRemoteIpAddr, numUdpPort, lwip_app_tcp_client_connect);
	}
	else
	{
		err = ERR_MEM;
	}
	
	return err;
}


//lwIP TCP���ӽ�������ûص�����
err_t lwip_app_tcp_client_connect(void *arg, struct tcp_pcb *pTcpPcb, err_t err)
{
	// �����ӳɹ���
	if (err == ERR_OK)
	{
		tcp_arg(pTcpPcb, &tcpStatus);        			// ʹ��es����tpcb��callback_arg
		tcp_recv(pTcpPcb, lwip_app_tcp_client_recv);  	// ��ʼ��LwIP��tcp_recv�ص�����   
		tcp_err(pTcpPcb, lwip_app_tcp_client_error); 	// ��ʼ��tcp_err()�ص�����
		tcp_sent(pTcpPcb, lwip_app_tcp_client_sent);	// ��ʼ��LwIP��tcp_sent�ص�����
		tcp_poll(pTcpPcb, lwip_app_tcp_client_poll, 1); // ��ʼ��LwIP��tcp_poll�ص�����
		tcpStatus = enTcpClientConn;					// ����TCP״̬Ϊ�����ӳɹ�
		err = ERR_OK;
	}
	else
	{
		err = ERR_CONN;
	}
	
	return err;
	
}


// lwIP tcp_recv()�����Ļص�����
err_t lwip_app_tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p, err_t err)
{
	struct pbuf *pData;
	uint16_t numSize = 0;
	uint8_t *pStatus;
	err_t retErr; 
	
	LWIP_ASSERT("arg != NULL", arg != NULL);
	pStatus = (uint8_t *)arg; 
	
	// ����ӷ��������յ��յ�����֡�͹ر�����
	if(p != NULL)
	{
		// ���յ�������ȷ
		if (err == ERR_OK)
		{
			// ���յ�������ȷ����������Ȼ����ʱ
			if (*pStatus > enTcpClientClose)
			{
				memset(tcp_app_client_recv_buff, 0, TCP_APP_CLIENT_RX_BUFF_SIZE);  //���ݽ��ջ���������
				
				for(pData=p; pData!=NULL; pData=pData->next)  //����������pbuf����
				{
					if (pData->len > (TCP_APP_CLIENT_RX_BUFF_SIZE - numSize))
					{
						memcpy(tcp_app_client_recv_buff + numSize, pData->payload, TCP_APP_CLIENT_RX_BUFF_SIZE - numSize);
						numSize = TCP_APP_CLIENT_RX_BUFF_SIZE;
						break;
					}
					else
					{
						memcpy(tcp_app_client_recv_buff + numSize, pData->payload, pData->len);
						numSize += pData->len;
					}
					
				}
				
				*pStatus = enTcpClientRecvOK;
				tcp_recved(tpcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
				retErr = ERR_OK;
			}
			// ���յ�������ȷ���������Ѿ��Ͽ�
			else
			{
				//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
				tcp_recved(tpcb, p->tot_len);
				retErr = ERR_OK;
			}
		}
		else
		{
			*pStatus = enTcpClientRecvErr;
			retErr = err;
		}
	}
	
	pbuf_free(p);  	//�ͷ��ڴ�
	
	return retErr;
}


//lwIP tcp_err�����Ļص�����
void lwip_app_tcp_client_error(void *arg, err_t err)
{  
	// PASS...
}


//lwIP tcp_poll�Ļص�����
err_t lwip_app_tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t err = ERR_OK;
	struct pbuf *pBuff = NULL;
	uint16_t length = 0;
	uint8_t *pStatus;
	
	length = strlen(tcp_app_client_send_buff);
	pStatus = (uint8_t *)arg;
	
	// ���ݽ������
	switch(tcpStatus)
	{
		// �������ݳɹ�
		case enTcpClientRecvOK:
			// �����ڴ�
			pBuff = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);
			// ��tcp_app_client_send_buff[]�е����ݿ�����pBuff��
			pbuf_take(pBuff, tcp_app_client_send_buff, length);
			// ��tcp_app_client_send_buff[]���渴�Ƹ�pbuf�����ݷ��ͳ�ȥ
			lwip_app_tcp_client_send_data(tpcb, pBuff, pStatus);
			// ����tcpStatus״̬ΪӦ��ȴ�
			*pStatus = enTcpClientAckWait;
			// �ͷ��ڴ�
			pbuf_free(pBuff);
			break;
		// �������ݳ���
		case enTcpClientRecvErr:
			break;
		// ����ʧ��
		case enTcpClientSendErr:
			break;
		// �ر�����
		case enTcpClientClose:
			lwip_app_tcp_client_conn_close(pstcLwipTcpPcb);
			err = ERR_ABRT;
			break;
		default:
			break;
	}
	
	return err;
}


//lwIP tcp_sent�Ļص�����(����Զ���������յ�ACK�źź�������)
err_t lwip_app_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	tcpStatus = enTcpClientSendOK;
	return ERR_OK;
}


//�˺���������������
void lwip_app_tcp_client_send_data(struct tcp_pcb *pTcpPcb, struct pbuf *pData, uint8_t *pStatus)
{
	struct pbuf *pBuff;
 	err_t err = ERR_OK;
	
	while((err == ERR_OK) && (pData != NULL) && (pData->len <= tcp_sndbuf(pTcpPcb))) //��Ҫ���͵����ݼ��뵽���ͻ��������
	{
		pBuff = pData;
		
		err = tcp_write(pTcpPcb, pBuff->payload, pBuff->len, 1);
		
		if(err == ERR_OK)
		{  
			pData = pBuff->next;		// ָ����һ��pbuf
			
			if(pData) pbuf_ref(pData);	// pbuf��ref��һ
			pbuf_free(pBuff);			// �ͷ���һ��pbuf
		}
		else if(err == ERR_MEM)
			pData = pBuff;
	} 
}


//�ر��������������
void lwip_app_tcp_client_conn_close(struct tcp_pcb *pTcpPcb)
{
	//�Ƴ��ص�
	tcp_abort(pTcpPcb);//��ֹ����,ɾ��pcb���ƿ�
	tcp_arg(pTcpPcb, NULL);  
	tcp_recv(pTcpPcb, NULL);
	tcp_sent(pTcpPcb, NULL);
	tcp_err(pTcpPcb, NULL);
	tcp_poll(pTcpPcb, NULL, 0);  
}
