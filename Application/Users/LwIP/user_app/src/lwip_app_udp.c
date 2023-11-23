#include "user_app/inc/lwip_app_comm.h"



const char *udp_app_send_buff = "hello world!\r\n";	//UDP接收数据缓冲区
uint8_t udp_app_recv_buff[UDP_APP_RX_BUFF_SIZE];	//UDP接收数据缓冲区

struct udp_pcb *pstcLwipUdpPcb;


// UDP通信初始化
err_t lwip_app_udp_init(void)
{
	err_t UdpState = ERR_OK;
	ip_addr_t stcRemoteIpAddr;
	ip_addr_t stcLocalIpAddr;
	uint16_t numUdpPort = 8089;

	// 设置远端IP地址
	lwip_app_remote_ip_set(192,168,0,101);
	
	// IP转换
	IP4_ADDR(
		&stcRemoteIpAddr,
		stcLwipObject.remoteip[0],
		stcLwipObject.remoteip[1],
		stcLwipObject.remoteip[2],
		stcLwipObject.remoteip[3]
	);
	
	IP4_ADDR(
		&stcLocalIpAddr,
		stcLwipObject.ip[0],
		stcLwipObject.ip[1],
		stcLwipObject.ip[2],
		stcLwipObject.ip[3]
	);
	
	// 创建一个UDP对象
	pstcLwipUdpPcb = udp_new();
	
	if (pstcLwipUdpPcb == NULL)
	{
		return enUdpObjCrtErr;
	}
	
	// 绑定本地服务
	UdpState = udp_bind(pstcLwipUdpPcb, &stcLocalIpAddr, numUdpPort);
	
	if (UdpState != enUdpInitOK)
	{
		return enUdpBindErr;
	}
		
	// 连接远程UDP服务端
	UdpState = udp_connect(pstcLwipUdpPcb, &stcRemoteIpAddr, numUdpPort);
	
	if (UdpState != enUdpInitOK)
	{
		return enUdpConnErr;
	}
	
	// 注册接收回调函数
	udp_recv(pstcLwipUdpPcb, lwip_app_udp_recv_data, NULL);
	
	return enUdpInitOK;
}


// UDP客户端进程
void lwip_app_udp_client(void)
{
	if (stcLwipObject.udpstatus == enUdpRecvOK)
	{
		stcLwipObject.udpstatus = enUdpInitOK;
		
		lwip_app_udp_send_data(pstcLwipUdpPcb, udp_app_send_buff, strlen(udp_app_send_buff));
	}
	else if (stcLwipObject.udpstatus > enUdpRecvOK)
	{
		lwip_app_udp_conn_close(pstcLwipUdpPcb);
	}
}


// UDP接收数据
void lwip_app_udp_recv_data(void *arg, struct udp_pcb *pUdpPcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port)
{
	struct pbuf *pData;
	uint16_t numSize = 0;
	
	if (p != NULL)
	{
		stcLwipObject.udpstatus = enUdpRecvOK;
		
		memset(udp_app_recv_buff, 0, UDP_APP_RX_BUFF_SIZE);
		
		// 当接收到的数据不为空时，出列接收到的数据
		for (pData=p; pData!=NULL; pData=pData->next)
		{
			if (pData->len > (UDP_APP_RX_BUFF_SIZE - numSize))
			{
				memcpy(udp_app_recv_buff + numSize, pData->payload, UDP_APP_RX_BUFF_SIZE - numSize);
				numSize = UDP_APP_RX_BUFF_SIZE;
				break;
			}
			else
			{
				memcpy(udp_app_recv_buff + numSize, pData->payload, pData->len);
				numSize += pData->len;
			}
		}

		stcLwipObject.remoteip[0] = pUdpPcb->remote_ip.addr & 0xff; 	 //IADDR4
		stcLwipObject.remoteip[1] = (pUdpPcb->remote_ip.addr>>8) & 0xff; //IADDR3
		stcLwipObject.remoteip[2] = (pUdpPcb->remote_ip.addr>>16) & 0xff;//IADDR2
		stcLwipObject.remoteip[3] = (pUdpPcb->remote_ip.addr>>24) & 0xff;//IADDR1 
	}
	
	//释放内存
	pbuf_free(p);
	
}


// UDP发送数据
void lwip_app_udp_send_data(struct udp_pcb *pUdpPcb, const char *pData, uint16_t length)
{
	struct pbuf *pBuff = NULL;
	
	stcLwipObject.udpstatus = enUdpSendErr;
	
	//申请内存
	pBuff = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL); 
	
	if (pBuff != NULL)
	{
		// 填充缓冲区数据
        if (pbuf_take(pBuff, pData, strlen(pData)) == ERR_OK)
		{
			// UDP发送数据
			if (udp_send(pUdpPcb, pBuff) == ERR_OK)
			{
				stcLwipObject.udpstatus = enUdpSendOK;
			}
		}
	}
	
	// 释放内存
	pbuf_free(pBuff);
}


// UDP关闭连接
void lwip_app_udp_conn_close(struct udp_pcb *pUdpPcb)
{
	udp_disconnect(pUdpPcb); 	// 断开连接
	udp_remove(pUdpPcb);		// 删除连接
}
