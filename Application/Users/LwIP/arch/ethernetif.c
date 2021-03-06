/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */
#include "user_app/inc/lwip_app_comm.h"

#include "netif/ethernetif.h"
#include "netif/etharp.h"

#include "lan8720.h"
#include "string.h"

//??ethernetif_init()??????????????????
//netif:?????????????? 
//??????:ERR_OK,????
//       ????,????
static err_t low_level_init(struct netif *netif)
{
#ifdef CHECKSUM_BY_HARDWARE
	int i; 
#endif 
	netif->hwaddr_len = ETHARP_HWADDR_LEN; //????MAC????????,??6??????
	//??????MAC????,??????????????????????????,????????????????????????MAC????????
	netif->hwaddr[0] = stcLwipObject.mac[0]; 
	netif->hwaddr[1] = stcLwipObject.mac[1]; 
	netif->hwaddr[2] = stcLwipObject.mac[2];
	netif->hwaddr[3] = stcLwipObject.mac[3];
	netif->hwaddr[4] = stcLwipObject.mac[4];
	netif->hwaddr[5] = stcLwipObject.mac[5];
	netif->mtu = 1500; //????????????????,????????????????ARP????

	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
	
	ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); //??STM32F4??MAC????????????????MAC????
	ETH_DMATxDescChainInit(DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);
	ETH_DMARxDescChainInit(DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);
#ifdef CHECKSUM_BY_HARDWARE 	//??????????????
	for(i=0;i<ETH_TXBUFNB;i++)	//????TCP,UDP??ICMP????????????,TCP,UDP??ICMP??????????????DMA????????
	{
		ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
	}
#endif
	ETH_Start(); //????MAC??DMA				
	return ERR_OK;
} 
//??????????????????????????(lwip????netif->linkoutput??????????)
//netif:??????????????
//p:pbuf??????????????
//??????:ERR_OK,????????
//       ERR_MEM,????????
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	u8 res;
	struct pbuf *q;
	int l = 0;
	u8 *buffer=(u8 *)ETH_GetCurrentTxBuffer(); //????????????????DMA????????????????????
	for(q=p;q!=NULL;q=q->next) 
	{
		memcpy((u8_t*)&buffer[l], q->payload, q->len);
		l=l+q->len;
	} 
	res=ETH_Tx_Packet(l); //????ETH_Tx_Packet????????????
	if(res==ETH_ERROR)return ERR_MEM;//????????????
	return ERR_OK;
}  
///??????????????????????????
//neitif:??????????????
//??????:pbuf??????????????
static struct pbuf * low_level_input(struct netif *netif)
{  
	struct pbuf *p, *q;
	u16_t len;
	int l =0;
	FrameTypeDef frame;
	u8 *buffer;
	p = NULL;
	frame=ETH_Rx_Packet();
	len=frame.length;//??????????
	buffer=(u8 *)frame.buffer;//?????????????? 
	p=pbuf_alloc(PBUF_RAW,len,PBUF_POOL);//pbufs??????????pbuf
	if(p!=NULL)
	{
		for(q=p;q!=NULL;q=q->next)
		{
			memcpy((u8_t*)q->payload,(u8_t*)&buffer[l], q->len);
			l=l+q->len;
		}    
	}
	frame.descriptor->Status=ETH_DMARxDesc_OWN;//????Rx??????OWN??,buffer????ETH DMA 
	if((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)//??Rx Buffer????????(RBUS)????????????,??????.????????
	{ 
		ETH->DMASR=ETH_DMASR_RBUS;//????ETH DMA RBUS?? 
		ETH->DMARPDR=0;//????DMA????
	}
	return p;
}
//????????????(lwip????????)
//netif:??????????????
//??????:ERR_OK,????????
//       ERR_MEM,????????
err_t ethernetif_input(struct netif *netif)
{
	err_t err;
	struct pbuf *p;
	
	p = low_level_input(netif);   //????low_level_input????????????
	
	if(p == NULL) return ERR_MEM;
	
	err = netif->input(p, netif); //????netif??????????input????(????????)????????????
	
	if(err != ERR_OK)
	{
		pbuf_free(p);
		p = NULL;
	} 
	return err;
} 
//????low_level_init()????????????????
//netif:??????????????
//??????:ERR_OK,????
//       ????,????
err_t ethernetif_init(struct netif *netif)
{
#if LWIP_NETIF_HOSTNAME			//LWIP_NETIF_HOSTNAME 
	netif->hostname="STM32F407-Explorer Borad";  	//??????????
#endif 
	netif->name[0]=IFNAME0; 	//??????????netif??name????
	netif->name[1]=IFNAME1; 	//??????????????????????????????
	netif->output=etharp_output;//IP????????????????
	netif->linkoutput=low_level_output;//ARP??????????????????
	low_level_init(netif); 		//??????????????????
	return ERR_OK;
}















