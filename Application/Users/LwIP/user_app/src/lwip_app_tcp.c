#include "user_app/inc/lwip_app_comm.h"


const char *tcp_app_client_send_buff = "hello world!\r\n";	//UDP接收数据缓冲区
uint8_t tcp_app_client_recv_buff[TCP_APP_CLIENT_RX_BUFF_SIZE];	//UDP接收数据缓冲区

struct tcp_pcb *pstcLwipTcpPcb;

ip_addr_t stcRemoteIpAddr;

uint8_t tcpStatus = 0;


// TCP通信初始化
err_t lwip_app_tcp_client_init(void)
{
	err_t err;
	
	uint16_t numUdpPort = 8089;
	
	// 设置默认IP地址
	lwip_app_remote_ip_set(192, 168, 0, 101);
	
	// 新建一个TCP_PCB对象
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


//lwIP TCP连接建立后调用回调函数
err_t lwip_app_tcp_client_connect(void *arg, struct tcp_pcb *pTcpPcb, err_t err)
{
	// 当连接成功后
	if (err == ERR_OK)
	{
		tcp_arg(pTcpPcb, &tcpStatus);        			// 使用es更新tpcb的callback_arg
		tcp_recv(pTcpPcb, lwip_app_tcp_client_recv);  	// 初始化LwIP的tcp_recv回调功能   
		tcp_err(pTcpPcb, lwip_app_tcp_client_error); 	// 初始化tcp_err()回调函数
		tcp_sent(pTcpPcb, lwip_app_tcp_client_sent);	// 初始化LwIP的tcp_sent回调功能
		tcp_poll(pTcpPcb, lwip_app_tcp_client_poll, 1); // 初始化LwIP的tcp_poll回调功能
		tcpStatus = enTcpClientConn;					// 设置TCP状态为已连接成功
		err = ERR_OK;
	}
	else
	{
		err = ERR_CONN;
	}
	
	return err;
	
}


// lwIP tcp_recv()函数的回调函数
err_t lwip_app_tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p, err_t err)
{
	struct pbuf *pData;
	uint16_t numSize = 0;
	uint8_t *pStatus;
	err_t retErr; 
	
	LWIP_ASSERT("arg != NULL", arg != NULL);
	pStatus = (uint8_t *)arg; 
	
	// 如果从服务器接收到空的数据帧就关闭连接
	if(p != NULL)
	{
		// 接收的数据正确
		if (err == ERR_OK)
		{
			// 当收到数据正确，且连接依然建立时
			if (*pStatus > enTcpClientClose)
			{
				memset(tcp_app_client_recv_buff, 0, TCP_APP_CLIENT_RX_BUFF_SIZE);  //数据接收缓冲区清零
				
				for(pData=p; pData!=NULL; pData=pData->next)  //遍历完整个pbuf链表
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
				tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
				retErr = ERR_OK;
			}
			// 当收到数据正确，但连接已经断开
			else
			{
				//用于获取接收数据,通知LWIP可以获取更多数据
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
	
	pbuf_free(p);  	//释放内存
	
	return retErr;
}


//lwIP tcp_err函数的回调函数
void lwip_app_tcp_client_error(void *arg, err_t err)
{  
	// PASS...
}


//lwIP tcp_poll的回调函数
err_t lwip_app_tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t err = ERR_OK;
	struct pbuf *pBuff = NULL;
	uint16_t length = 0;
	uint8_t *pStatus;
	
	length = strlen(tcp_app_client_send_buff);
	pStatus = (uint8_t *)arg;
	
	// 数据接收完成
	switch(tcpStatus)
	{
		// 接收数据成功
		case enTcpClientRecvOK:
			// 申请内存
			pBuff = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);
			// 将tcp_app_client_send_buff[]中的数据拷贝到pBuff中
			pbuf_take(pBuff, tcp_app_client_send_buff, length);
			// 将tcp_app_client_send_buff[]里面复制给pbuf的数据发送出去
			lwip_app_tcp_client_send_data(tpcb, pBuff, pStatus);
			// 设置tcpStatus状态为应答等待
			*pStatus = enTcpClientAckWait;
			// 释放内存
			pbuf_free(pBuff);
			break;
		// 接收数据出错
		case enTcpClientRecvErr:
			break;
		// 发送失败
		case enTcpClientSendErr:
			break;
		// 关闭连接
		case enTcpClientClose:
			lwip_app_tcp_client_conn_close(pstcLwipTcpPcb);
			err = ERR_ABRT;
			break;
		default:
			break;
	}
	
	return err;
}


//lwIP tcp_sent的回调函数(当从远端主机接收到ACK信号后发送数据)
err_t lwip_app_tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	tcpStatus = enTcpClientSendOK;
	return ERR_OK;
}


//此函数用来发送数据
void lwip_app_tcp_client_send_data(struct tcp_pcb *pTcpPcb, struct pbuf *pData, uint8_t *pStatus)
{
	struct pbuf *pBuff;
 	err_t err = ERR_OK;
	
	while((err == ERR_OK) && (pData != NULL) && (pData->len <= tcp_sndbuf(pTcpPcb))) //将要发送的数据加入到发送缓冲队列中
	{
		pBuff = pData;
		
		err = tcp_write(pTcpPcb, pBuff->payload, pBuff->len, 1);
		
		if(err == ERR_OK)
		{  
			pData = pBuff->next;		// 指向下一个pbuf
			
			if(pData) pbuf_ref(pData);	// pbuf的ref加一
			pbuf_free(pBuff);			// 释放上一个pbuf
		}
		else if(err == ERR_MEM)
			pData = pBuff;
	} 
}


//关闭与服务器的连接
void lwip_app_tcp_client_conn_close(struct tcp_pcb *pTcpPcb)
{
	//移除回调
	tcp_abort(pTcpPcb);//终止连接,删除pcb控制块
	tcp_arg(pTcpPcb, NULL);  
	tcp_recv(pTcpPcb, NULL);
	tcp_sent(pTcpPcb, NULL);
	tcp_err(pTcpPcb, NULL);
	tcp_poll(pTcpPcb, NULL, 0);  
}
