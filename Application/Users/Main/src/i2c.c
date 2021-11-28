/* ���IICģ�飨��֤ͨ���� */
#include "i2c.h"


void IIC_Init(void) 
{
	/* ���ýṹ�嶨�� */
	GPIO_InitTypeDef GPIO_InitStructure;						// ����һ��GPIO���ýṹ��
	
	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		// ʹ��GPIOBʱ��
	
	/* ����GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				// GPIO���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// �������
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;				// ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		// ����Pin8~9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// ����ٶ�Ϊ100MHz
	
	/* ��ʼ��GPIO */
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// Ӧ��GPIOB����
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);				// ��ʼ����SCK��Pin8����SDA��Pin9��
}


// ������ʼ�źţ�SCK�ߵ�ƽ�£�SDA�ɸ߱��
void IIC_Start(void) 
{
	IIC_Mode_Tx();     //sda�����
	IIC_SDA_OUT = 1;	  	  
	IIC_SCL = 1;
	Delay_Us(4);
 	IIC_SDA_OUT = 0;//START:when CLK is high,DATA change form high to low 
	Delay_Us(4);
	IIC_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

    
// ���ͽ����źţ�SCK�ߵ�ƽ�£�SDA�ɵͱ��
void IIC_Stop(void) 
{
	IIC_Mode_Tx();//sda�����
	IIC_SCL = 0;
	IIC_SDA_OUT = 0;//STOP:when CLK is high DATA change form low to high
 	Delay_Us(4);
	IIC_SCL = 1; 
	IIC_SDA_OUT = 1;//����I2C���߽����ź�
	Delay_Us(4);	
}


// ������Ӧ�źţ�Ӧ�𡢷�Ӧ�𣩣����������ݺ�SDA����
static void IIC_TxReply(uint8_t Res)
{ 
	IIC_SCL = 0;
	IIC_Mode_Tx();
	
	IIC_SDA_OUT = (1 - Res);	// ��SDA������Ӧ�𣬷���Ӧ��
	Delay_Us(2);
	
	IIC_SCL = 1;
	Delay_Us(2);
	
	IIC_SCL = 0;
}


// ������Ӧ�źţ�Ӧ�𡢷�Ӧ�𣩣����������ݺ�SDA����
static bool IIC_RxReply(void)
{ 
	uint8_t ReplyTime;
	
	ReplyTime = 0;

	IIC_Mode_Rx();
	IIC_SDA_OUT = 1;
	Delay_Us(1);	   
	IIC_SCL=1;
	Delay_Us(1);
	while(IIC_SDA_IN)
	{
		ReplyTime++;
		if(ReplyTime>250)
		{
			IIC_Stop();
			return False;
		}
	}
	
	IIC_SCL = 0;

	return True;	

}


// ����8λ����
bool IIC_Send_Byte(uint8_t Data) 
{
	uint8_t Bit;
	
	IIC_Mode_Tx();	// ��IIC����Ϊ����ģʽ
	
	for(Bit=0;Bit<8;Bit++) 
	{
		IIC_SDA_OUT = (Data&0x80)>>7;
		Data<<=1;
		Delay_Us(1);
		
		IIC_SCL = 1;
		Delay_Us(2);
		
		IIC_SCL = 0;		// ����SCL��ƽ��׼�����͵�һ֡����
		Delay_Us(1);

	}
	return IIC_RxReply();
}


// ����8λ����
uint8_t IIC_Read_Byte(uint8_t IsAck) 
{
	
	uint8_t RxData, RxIndex;
	
	IIC_Mode_Rx();
	
	for (RxIndex=0;RxIndex<8;RxIndex++) 
	{
		IIC_SCL = 0;
		Delay_Us(2);
		
		IIC_SCL = 1;
		RxData <<= 1;
		if (IIC_SDA_IN) RxData++;
		Delay_Us(1);
	}
	
	IIC_TxReply(IsAck);
	
	return RxData;
}
