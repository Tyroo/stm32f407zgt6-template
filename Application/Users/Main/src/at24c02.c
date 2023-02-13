#include "at24c02.h"


// ����һ���ַ��������豸
bool AT24C02_TxData(uint8_t* Data,uint8_t Addr, uint8_t TxLen) 
{
	bool 	Res = true;								// Ӧ����Ӧ
	uint8_t Index = 0;								// �����ֽ�������
	
	while((Index < TxLen) && (Res == true)) 
	{	
		IIC_Start();								// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA0);					// ���ͼĴ����ߵ�ַ
		Res = IIC_Send_Byte((Addr + Index) % 256);	// ���ͼĴ����͵�ַ
		Res = IIC_Send_Byte(Data[Index++]);			// ����һ���ֽ�����
		IIC_Stop();									// ����IIC�����ź�
		Delay_Ms(5);
	}
	return Res;
}


// ����һ���ַ���
bool AT24C02_RxData(uint8_t* Buff, uint8_t Addr, uint8_t RxLen) 
{
	bool 	Res = true;								// Ӧ����Ӧ
	uint8_t Index = 0;								// ������������
	
	while((Index < RxLen) && (Res == true)) 
	{
		IIC_Start();								// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA0);					// ���ͼĴ�����ַ��λ
		Res = IIC_Send_Byte((Addr + Index) % 256);	// ���ͼĴ�����ַ��λ
		
		IIC_Start();								// ����IIC��ʼ�ź�
		Res = IIC_Send_Byte(0xA1); 					// ���Ͷ�����
		Buff[Index++] = IIC_Read_Byte(0);			// ��ȡ���ݣ�������Ӧ���ź�
		IIC_Stop();									// ֹͣ����
		Delay_Ms(5);
	}
	
	return Res;
}
