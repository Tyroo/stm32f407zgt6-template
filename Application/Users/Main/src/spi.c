#include "spi.h"



char Spi1_ReceiveData[50];

uint8_t Spi3_DmaWriteData[SPI3_RTX_BUFF_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t Spi3_DmaReadData[SPI3_RTX_BUFF_SIZE];

#if 0
// ����SPIͨ��
void SPI1_Init(void) {

	/* ���ýṹ�嶨�� */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʱ������ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* GPIO���� */
	//�������Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3 ����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4 ����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5 ����Ϊ SPI1
	// �������ź�
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	// ����GPIOΪ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// ����IO��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// ���ö˿�Ϊ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// ��������ٶ�50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* ����SPI1 */
	// ��ʼ��SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λ SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λ SPI1
	// ����SPIʱ�ӷ�Ƶ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// ������ʱ�ӵĵڶ��������زɼ�����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// ����ʱ�����ڵ͵�ƽʱΪ����״̬
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// ����SPI1��ͨ��ģʽΪ����ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	// �������ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// ����NSS�ź����������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCRУ�����ʽ������1����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1�����λ��Ϊ8λ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// ����SPI1�����շ�����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* Ӧ������ */
	// ��ʼ��GPIOA5��6��7����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// ��ʼ��SPI1����	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* ʹ��/ʧ�� */
	// ʹ�ܽ����ж�
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	
	// ʹ��SPI1
	SPI_Cmd(SPI1, ENABLE);
}
#endif

// ����SPIͨ��
void SPI1_Init(void) {

	/* ���ýṹ�嶨�� */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʱ������ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* GPIO���� */
	//�������Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3 ����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4 ����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5 ����Ϊ SPI1
	// �������ź�
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	// ����GPIOΪ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// ����IO��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// ���ö˿�Ϊ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// ��������ٶ�50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* ����SPI1 */
	// ��ʼ��SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λ SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λ SPI1
	// ����SPIʱ�ӷ�Ƶ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// ������ʱ�ӵĵڶ��������زɼ�����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// ����ʱ�����ڵ͵�ƽʱΪ����״̬
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// ����SPI1��ͨ��ģʽΪ����ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	// �������ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// ����NSS�ź����������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCRУ�����ʽ������1����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1�����λ��Ϊ8λ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// ����SPI1�����շ�����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* Ӧ������ */
	// ��ʼ��GPIOA5��6��7����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// ��ʼ��SPI1����	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI3, DISABLE);
	
	/* ʹ��/ʧ�� */
	// ʹ�ܽ����ж�
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	
	// ʹ��SPI1
	SPI_Cmd(SPI1, ENABLE);
}


void SPI3_Init(void)
{
	/* ���ýṹ�嶨�� */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʱ������ */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/* GPIO���� */

	// �������ź�
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
	// ����GPIOΪ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// ����IO��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// ���ö˿�Ϊ����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// ��������ٶ�50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// ��ʼ��GPIOA5��6��7����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//�������Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); //PC10 ����Ϊ SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); //PC11 ����Ϊ SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3); //PC12 ����Ϊ SPI3
	
	/* ����SPI1 */
	// ��ʼ��SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//��λ SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//ֹͣ��λ SPI1
	// ����SPIʱ�ӷ�Ƶ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// ������ʱ�ӵĵڶ��������زɼ�����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// ����ʱ�����ڵ͵�ƽʱΪ����״̬
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	// ����SPI1��ͨ��ģʽΪ����ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	// �������ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// ����NSS�ź����������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCRУ�����ʽ������1����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1�����λ��Ϊ8λ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// ����SPI1�����շ�����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* Ӧ������ */
	// ��ʼ��SPI1����	
	SPI_Init(SPI3, &SPI_InitStructure);
	
	// ʹ��SPI DMA�շ�����
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
	
	// ʹ��SPI1
	SPI_Cmd(SPI3, ENABLE);
}


// ʹ��SPI1��������
bool SPI1_Send(char* SendData) {
	
	// ��û�����������ַ�ʱһֱ����
	while(*SendData != '\0') {
		// �ȴ����ͻ��������
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		
		SPI_I2S_SendData(SPI1, *(SendData++));
	}
	return true;
}


// SPI1ͨ��<�жϺ���>
void SPI1_IRQHandler() {
	
	u8 RxData;				// ����һ������ÿ���ֽڵı���
	static u8 RxIndex = 0;	// �������������
	// SPI���ջ������ǿձ�־
	FlagStatus ReadFlag = SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE);
	
	// �����жϺ󣬵����ܻ�������Ϊ��
	if (ReadFlag != RESET) {
		RxData = SPI_I2S_ReceiveData(SPI1);	// ��ȡ���ջ�������ֵ
		Spi1_ReceiveData[RxIndex++] = RxData;	// �����������������
		
		// �����������ַ�����ֹ����
		if (RxData == '\n') {
			RxIndex = 0;
			Spi1_ReceiveData[RxIndex-1] = '\0'; // �����λ����Ϊ�����ַ�
			return;
		}
	}
}
