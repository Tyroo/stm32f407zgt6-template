#include "at24c02.h"


// 发送一个字符串给从设备
bool AT24C02_TxData(uint8_t* Data,uint8_t Addr, uint8_t TxLen) 
{
	bool 	Res = true;								// 应答响应
	uint8_t Index = 0;								// 发送字节数索引
	
	while((Index < TxLen) && (Res == true)) 
	{	
		IIC_Start();								// 发送IIC开始信号
		Res = IIC_Send_Byte(0xA0);					// 发送寄存器高地址
		Res = IIC_Send_Byte((Addr + Index) % 256);	// 发送寄存器低地址
		Res = IIC_Send_Byte(Data[Index++]);			// 发送一个字节数据
		IIC_Stop();									// 发送IIC结束信号
		Delay_Ms(5);
	}
	return Res;
}


// 接收一个字符串
bool AT24C02_RxData(uint8_t* Buff, uint8_t Addr, uint8_t RxLen) 
{
	bool 	Res = true;								// 应答响应
	uint8_t Index = 0;								// 接收数据索引
	
	while((Index < RxLen) && (Res == true)) 
	{
		IIC_Start();								// 发送IIC起始信号
		Res = IIC_Send_Byte(0xA0);					// 发送寄存器地址高位
		Res = IIC_Send_Byte((Addr + Index) % 256);	// 发送寄存器地址低位
		
		IIC_Start();								// 发送IIC起始信号
		Res = IIC_Send_Byte(0xA1); 					// 发送读命令
		Buff[Index++] = IIC_Read_Byte(0);			// 读取数据，不发送应答信号
		IIC_Stop();									// 停止发送
		Delay_Ms(5);
	}
	
	return Res;
}
