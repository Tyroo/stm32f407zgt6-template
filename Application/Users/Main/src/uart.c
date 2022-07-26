/** USART�շ�����ģ�飨��֤ͨ���� **/

#include "uart.h"

// USART1���յ�����
char UsartReceiveData[50];

#if USE_PRINTF_FUNC        
#pragma import(__use_no_semihosting)             

//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
};


FILE __stdout;       

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 

}

void _ttywrch(int ch)
{
	ch = ch;
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);	//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif


// ��ʼ��UART
void Uart1_Init(uint32_t Baud) {
	
	/* ���ýṹ��ָ�붨�� */
	USART_InitTypeDef USART_InitStructre;					// ����һ��USART���ýṹ��
	GPIO_InitTypeDef	GPIO_InitStructre;					// ����һ��GPIO ���ýṹ��
	
	/* ʱ������ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// ʹ��GPIOA�˿�ʱ�ӣ�ʹ��UART1�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// ʹ��USART1ʱ��
	
	/* GPIOA9 IO�ڸ���USART1���� */
	USART_DeInit(USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);		//PA9 ����Ϊ USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	//PA10����ΪUSART1
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		// ��������PA9��PA10
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_AF;					// ����GPIOλ�����������
	GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;				// �������
	GPIO_InitStructre.GPIO_PuPd = GPIO_PuPd_UP;					// IO����
	GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;			// ����PA9���ŵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructre);						// ������Ӧ�ø�PA9
	
	/* USART1���� */
	USART_InitStructre.USART_BaudRate = Baud;					// ���ò�����
	USART_InitStructre.USART_WordLength = USART_WordLength_8b;		// ���ô��������λλ��
	USART_InitStructre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// ʹ��USART�߱��շ����ݵĹ���	
	USART_InitStructre.USART_StopBits = USART_StopBits_1;			// �����ڴ���֡��β����һ��λ��ֹͣλ
	USART_InitStructre.USART_Parity = USART_Parity_No;				// ʧ����żУ��λ
	USART_InitStructre.USART_HardwareFlowControl =					// ��Ӳ�����������ƣ������ô�λ����ֽ��ղ�������
	 USART_HardwareFlowControl_None; 	
	
	USART_Init(USART1, &USART_InitStructre);						// ��ʼ��USART1����
	
	/* ʹ�� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					// ʹ��USART1�Ľ����ж�
	USART_Cmd(USART1, ENABLE);										// ʹ�ܴ���
	
	/* NVIC���� */
	Nvic_Config(USART1_IRQn, 1, 1, 1);								// ��ռ���ȼ�Ϊ0����Ӧ���ȼ�Ϊ0����ʹ��
	
}


// UART1��������
void Uart1_Send(char *Data) {
	
	// �������ַ�����\0ʱֹͣ����
	while (*Data != '\0') {
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, *(Data++));							// ����USART_SendData���͵����ַ�
	}
}


// USART1��������<�жϺ���>
void USART1_IRQHandler() {
	
	uint8_t RxData;										// ����һ�����յ����ַ��ı���
	static int DataBit = 0;								// ����һ������λ
	
	// �����ռĴ����Ƿ�ǿգ��ǿ�Ϊ1����Ϊ0
	FlagStatus ReceiveFlag = USART_GetITStatus(USART1, USART_IT_RXNE);
	
	if (ReceiveFlag != RESET) {
		RxData = USART_ReceiveData(USART1);				// ��ȡ���յ�������
//		USART_SendData(USART1, RxData);

		UsartReceiveData[DataBit++] = RxData;			// �����ݴ�������ַ�����
		
		// �����յ�����������β�ַ�ʱ��ֹ����
		if (RxData == '\n') {
			UsartReceiveData[DataBit] = '\0';
			DataBit = 0;								// �������λ
		}
	}
}
