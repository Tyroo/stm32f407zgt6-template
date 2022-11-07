#include "at24c02.h"


// ����һ���ַ��������豸
bool AT24C02_Tx_Data(uint8_t* Data,
		uint16_t Addr, uint8_t TxLen) 
{
	bool 	Res;						// Ӧ����Ӧ
	uint8_t Index;						// �����ֽ�������
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ������ʧ��
	if (sizeof(Data)>IIC_DATA_SIZE_MAX) 
		return false;
	
	Res = true, Index = 0;
	
	// �����������ַ�����Ӧ��ʧЧ��ֹͣ����

	while((Index < TxLen) && (Res == true)) 
	{	
		IIC_Start();						// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA0+(((Addr+Index)/256)<<1));// ���ͼĴ����ߵ�ַ
		Res = IIC_Send_Byte((Addr+Index)%256);		// ���ͼĴ����͵�ַ
		Res = IIC_Send_Byte(*(Data+Index));	// ����һ���ֽ�����
		IIC_Stop();							// ����IIC�����ź�
		Delay_Ms(10);
		Index++;
	}
	return (bool)Res;
}


// ����һ���ַ���
bool AT24C02_Rx_Data(uint8_t* Buff, 
		uint16_t Addr, uint8_t RxLen) 
{
	bool 	Res;	// Ӧ����Ӧ
	uint8_t Index;	// ������������
	
	// �����յ����ݳ���������ƻ����豸��ַ����127ʱ���˳�����
	if (RxLen>IIC_DATA_SIZE_MAX) 
		return false;
	
	Res = true, Index = 0;
	
	while((Index < RxLen) && (Res == true)) 
	{
		IIC_Start();						// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA0+(((Addr+Index)/256)<<1));// ���ͼĴ�����ַ��λ
		Res = IIC_Send_Byte((Addr+Index)%256);// ���ͼĴ�����ַ��λ
		
		/* �������ݴ�������, �������һ���ֽں�����Ӧ�� */
		IIC_Start();						// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA1); 			// ���Ͷ�����
		*(Buff+Index) = IIC_Read_Byte(0);	// ��ȡ���ݣ�������Ӧ���ź�
		IIC_Stop();							// ֹͣ����
		Index++;
	}
	
	return (bool)Res;
}
