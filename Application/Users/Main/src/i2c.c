/* ���IICģ�飨δ��֤�� */
#include "i2c.h"

u8 RxData[IIC_DATA_SIZE_MAX];															// ����һ��ȫ�ֱ��������洢���յ��ַ���

void IIC_Init() {
	
	/* ���ýṹ�嶨�� */
	GPIO_InitTypeDef GPIO_InitStructure;											// ����һ��GPIO���ýṹ��
	
	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		// ʹ��GPIOBʱ��
	
	/* ����GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						// GPIO���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					// �������
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;						// ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;// ����Pin10~11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// ����ٶ�Ϊ50MHz
	
	/* ��ʼ��GPIO */
	GPIO_Init(GPIOB, &GPIO_InitStructure);										// Ӧ��GPIOB����
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);					// ��ʼ����SCK��Pin10����SDA��Pin11��
}


// ������ʼ�źţ�SCK�ߵ�ƽ�£�SDA�ɸ߱��
void IIC_Start() {
	
	IIC_Mode_Tx();	// ����I2CΪ����ģʽ
	
	IIC_SCL = 1, IIC_SDA_OUT = 1;		// ����SCL��SDA��׼��������ʼ�ź�
	
	Delay_Us(4);
	IIC_SDA_OUT = 0;								// ��SCL�ߵ�ƽʱ������SDA��������ʼ�ź�
	
	Delay_Us(4);
	IIC_SCL = 0;										// ��SCL���ͣ�׼����������
	
}


// ���ͽ����źţ�SCK�ߵ�ƽ�£�SDA�ɵͱ��
void IIC_Stop() {
	
	IIC_Mode_Tx();									// ����I2CΪ����ģʽ
	
	IIC_SCL = 0, IIC_SDA_OUT = 0;		// ����SCL������SDA
	
	Delay_Us(4);
	IIC_SCL = 1, IIC_SDA_OUT = 1;		// ����SDA�����������ź�
	
	Delay_Us(4);
}


// ����Ӧ���źţ����������ݺ�SDA����
void IIC_SendAck(void){ 
	
	IIC_Mode_Tx();
	
	IIC_SCL = 0;
	
	IIC_SDA_OUT = 0;
	
	Delay_Us(2);
	IIC_SCL = 1;
	
//	Delay_Us(2);
//	IIC_SCL = 0;
}


// ������Ӧ���źţ����������ݺ�SDA����
void IIC_NotAck(){ 
	
	IIC_Mode_Tx();
	
	IIC_SCL = 0;
	
	IIC_SDA_OUT = 1;
	
	Delay_Us(2);
	IIC_SCL = 1;
	
//	Delay_Us(2);
//	IIC_SCL = 0;
}


void IIC_WaitAck(bool *TimeoutFlag) {
	
	u8 TimeoutCount = 0;	// ���豸Ӧ��ʱ����
	
	if (*TimeoutFlag == False) return;
	
	IIC_Mode_Rx(); 				// SDA����Ϊ���� 
	
	// ��SDA��SCL��Ϊ�ߵ�ƽ���ȴ����豸����SDAΪ�͵�ƽ��Ӧ���ź�
	IIC_SDA_OUT = 1;
	Delay_Us(1);
	
	IIC_SCL=1;
	Delay_Us(1);
	
	// �ȴ����豸��SDA����
	while(IIC_SDA_IN) {
		
		TimeoutCount++;
		
		// ������Ӧ��ȴ�ʱ�䳬��250��ʱ�����ں���Ϊͨ��ʧ��
		if(TimeoutCount>250) { 
			IIC_Stop();	// ����ͨ��ֹͣ�źŸ����豸
			*TimeoutFlag = False;
			return;
		}
	}
	
	IIC_SCL = 0; 		//ʱ����� 0 
	
	*TimeoutFlag = True;
	return; 
} 


// ����8λ����
void IIC_Send_Byte(u8 Data) {
	
	int i = 0;
	
	IIC_Mode_Tx();	// ��IIC����Ϊ����ģʽ
	
	
	
	for(i=7;i>=0;i--) {
		
		IIC_SCL = 0;		// ����SCL��ƽ��׼�����͵�һ֡����
		
		if (Data&(1<<i)) IIC_SDA_OUT = 1;
		else IIC_SDA_OUT = 0;
		Delay_Us(2);
		
		IIC_SCL = 1;
		Delay_Us(2);
		
		
	}
}


// ����8λ����
u8 IIC_Read_Byte(short IsAck) {
	
	static u8 RxData = 0;
	short RxIndex;
	
	IIC_Mode_Rx();
	
	for (RxIndex=0;RxIndex<8;RxIndex++) {
		
		IIC_SCL = 0;
		
		Delay_Us(2);
		IIC_SCL = 1;
		
		RxData <<= 1;
		if (IIC_SDA_IN) RxData |= 0x01;
		
		Delay_Us(2);
	}
	
	IIC_SendAck();
	
	return RxData;
}


// ����һ���ַ��������豸
bool IIC_Send_String(char *Str, u8 DeviceAddr, u16 RegisterAddr) {
	
	bool WaitAckFlag = True;							// Ӧ���־λ
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ������ʧ��
	if (strlen(Str)>IIC_DATA_SIZE_MAX ||
		DeviceAddr>127) return False;
	
	IIC_Start();													// ����IIC��ʼ�ź�
	
	IIC_Send_Byte((DeviceAddr<<1)&0xfe);	// �����豸��ַ
	IIC_WaitAck(&WaitAckFlag);						// �ȴ�Ӧ��
	
	IIC_Send_Byte(RegisterAddr>>8);				// ���ͼĴ�����ַ��λ
	IIC_WaitAck(&WaitAckFlag);						// �ȴ�Ӧ��
	
	IIC_Send_Byte(RegisterAddr%256); 			// ���ͼĴ����͵�ַ
	IIC_WaitAck(&WaitAckFlag); 						// �ȴ�Ӧ��
	
	// �����������ַ�����Ӧ��ʧЧ��ֹͣ����
	while(*Str != '\n' && WaitAckFlag == True) {
		IIC_Send_Byte(*Str++);							// ����һ���ַ�
		IIC_WaitAck(&WaitAckFlag); 					// �ȴ�Ӧ��
	}
		
	IIC_Stop();														// ����IIC�����ź�
	
	return WaitAckFlag;
}


// ����һ���ַ���
void IIC_Read_String(u8 DeviceAddr, u16 RegisterAddr, u8 ReadLen) {
	
	u8 DataIndex = 0;												// ������������
	bool WaitAckFlag;												// Ӧ���־λ
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ���˳�����
	if (ReadLen>IIC_DATA_SIZE_MAX || 
		DeviceAddr > 127) return;
	
	while(DataIndex<ReadLen && WaitAckFlag == True) {
		
		IIC_Start();													// ����IIC��ʼ�ź�
		
		IIC_Send_Byte((DeviceAddr<<1)|0x01); 	// �����豸��ַ
		IIC_WaitAck(&WaitAckFlag);
		
		IIC_Start(); 													// ���Ϳ�ʼ�ź�
		
		IIC_Send_Byte(RegisterAddr>>8); 			// ���ͼĴ�����ַ��λ
		IIC_WaitAck(&WaitAckFlag);
		
		IIC_Send_Byte((RegisterAddr++)%256); 	// ���ͼĴ�����ַ��λ
		IIC_WaitAck(&WaitAckFlag);
		
		// �������ݴ�������, �������һ���ֽں�����Ӧ��
		RxData[DataIndex] = IIC_Read_Byte((DataIndex < ReadLen - 1));
		
		IIC_Stop();														// ֹͣ����
		DataIndex++;
	}
	
	RxData[DataIndex] = '\0';
}
