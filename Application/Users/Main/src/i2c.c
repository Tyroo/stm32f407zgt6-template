/* ���IICģ�飨δ��֤�� */
#include "i2c.h"


uint8_t RxData[IIC_DATA_SIZE_MAX];									// ����һ��ȫ�ֱ��������洢���յ��ַ���


void IIC_Init() 
{
	/* ���ýṹ�嶨�� */
	GPIO_InitTypeDef GPIO_InitStructure;						// ����һ��GPIO���ýṹ��
	
	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		// ʹ��GPIOBʱ��
	
	/* ����GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				// GPIO���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;				// ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	// ����Pin10~11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// ����ٶ�Ϊ50MHz
	
	/* ��ʼ��GPIO */
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// Ӧ��GPIOB����
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);				// ��ʼ����SCK��Pin10����SDA��Pin11��
}


// ������ʼ�źţ�SCK�ߵ�ƽ�£�SDA�ɸ߱��
static void IIC_Start(void) 
{
	
	IIC_Mode_Tx();	// ����I2CΪ����ģʽ
	
	IIC_SCL = 1; 
	IIC_SDA_OUT = 1;	// ����SCL��SDA��׼��������ʼ�ź�
	
	Delay_Us(4);
	IIC_SDA_OUT = 0;	// ��SCL�ߵ�ƽʱ������SDA��������ʼ�ź�
	
	Delay_Us(4);
	IIC_SCL = 0;		// ��SCL���ͣ�׼����������
	
}


// ���ͽ����źţ�SCK�ߵ�ƽ�£�SDA�ɵͱ��
static void IIC_Stop(void) 
{
	IIC_Mode_Tx();					// ����I2CΪ����ģʽ
	
	IIC_SDA_OUT = 0;				// ����SCL������SDA
	Delay_Us(4);
	
	IIC_SCL = 1;
	IIC_SDA_OUT = 1;	// ����SCL��ͬʱ����SDA�����������ź�
	
	Delay_Us(4);
}


// ������Ӧ�źţ�Ӧ�𡢷�Ӧ�𣩣����������ݺ�SDA����
inline static void IIC_TxReply(uint8_t Res)
{ 
	IIC_Mode_Tx();
	
	IIC_SDA_OUT = Res;	// ��SDA������Ӧ�𣬷���Ӧ��
	Delay_Us(2);
	
	IIC_SCL = 1;
	Delay_Us(2);
	
	IIC_SCL = 0;
}


// ������Ӧ�źţ�Ӧ�𡢷�Ӧ�𣩣����������ݺ�SDA����
inline static uint8_t IIC_RxReply(void)
{ 
	uint8_t Res;
	
	IIC_Mode_Rx();
	Delay_Us(2);
	
	IIC_SCL = 1;
	Res = IIC_SDA_IN;
	
	Delay_Us(2);
	IIC_SCL = 0;
	
	return Res;
}


// ����8λ����
void IIC_Send_Byte(uint8_t Data) 
{
	
	int i;
	
	IIC_Mode_Tx();	// ��IIC����Ϊ����ģʽ
	
	for(i=7;i>=0;i--) 
	{
		if (Data&(1<<i))
		{
			IIC_SDA_OUT = 1;
		}	
		else
		{
			IIC_SDA_OUT = 0;
		}
		Delay_Us(2);
		
		IIC_SCL = 1;
		Delay_Us(2);
		
		IIC_SCL = 0;		// ����SCL��ƽ��׼�����͵�һ֡����
	}
}


// ����8λ����
uint8_t IIC_Read_Byte(uint8_t IsAck) 
{
	
	uint8_t RxData, RxIndex;
	
	IIC_Mode_Rx();
	
	for (RxIndex=0;RxIndex<8;RxIndex++) 
	{
		Delay_Us(2);
		IIC_SCL = 1;
		
		RxData <<= 1;
		if (IIC_SDA_IN) RxData |= 0x01;
		
		Delay_Us(2);
		IIC_SCL = 0;
	}
	
	IIC_TxReply(IsAck);
	
	return RxData;
}


// ����һ���ַ��������豸
bool IIC_Send_String(char *Str, uint8_t DeviceAddr, 
		uint16_t RegisterAddr) 
{
	
	uint8_t Res;							// Ӧ���־λ
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ������ʧ��
	if (strlen(Str)>IIC_DATA_SIZE_MAX ||
		DeviceAddr>127) return False;
	
	IIC_Start();													// ����IIC��ʼ�ź�
	
	IIC_Send_Byte((DeviceAddr<<1)&0xfe);	// �����豸��ַ
	Res = IIC_RxReply();					// �ȴ�Ӧ��
	
	IIC_Send_Byte(RegisterAddr>>8);			// ���ͼĴ�����ַ��λ
	Res = IIC_RxReply();					// �ȴ�Ӧ��
	
	IIC_Send_Byte(RegisterAddr%256); 		// ���ͼĴ����͵�ַ
	Res = IIC_RxReply(); 					// �ȴ�Ӧ��
	
	// �����������ַ�����Ӧ��ʧЧ��ֹͣ����
	while(*Str != '\n' && Res == True) {
		IIC_Send_Byte(*Str++);				// ����һ���ַ�
		Res = IIC_RxReply(); 				// �ȴ�Ӧ��
	}

	IIC_Stop();								// ����IIC�����ź�
	
	return (bool)Res;
}


// ����һ���ַ���
bool IIC_Read_String(uint8_t DeviceAddr, 
		uint16_t RegisterAddr, uint8_t RxLen) 
{
	
	uint8_t DataIndex, Res;	// ��������������Ӧ����Ӧ
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ���˳�����
	if (RxLen>IIC_DATA_SIZE_MAX || DeviceAddr > 127) 
		return False;
	
	Res = 1, DataIndex = 0;
	IIC_Start();								// ����IIC��ʼ�ź�
	
	while(DataIndex<RxLen && Res == True) 
	{
		
		IIC_Send_Byte((DeviceAddr<<1)|0x01); 	// �����豸��ַ
		Res = IIC_RxReply(); 					// �ȴ�Ӧ��
		
		IIC_Start(); 							// ���Ϳ�ʼ�ź�
		
		IIC_Send_Byte(RegisterAddr>>8); 		// ���ͼĴ�����ַ��λ
		Res = IIC_RxReply(); 					// �ȴ�Ӧ��
		
		IIC_Send_Byte((RegisterAddr++)%256); 	// ���ͼĴ�����ַ��λ
		Res = IIC_RxReply(); 					// �ȴ�Ӧ��
		
		/* �������ݴ�������, �������һ���ֽں�����Ӧ�� */
		RxData[DataIndex] = IIC_Read_Byte(1);
		DataIndex++;
	}
	
	IIC_Stop();									// ֹͣ����
	RxData[DataIndex] = '\0';
	
	return (bool)Res;
}
