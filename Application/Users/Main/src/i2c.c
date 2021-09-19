/* 软件IIC模块（未验证） */
#include "i2c.h"

u8 RxData[IIC_DATA_SIZE_MAX];															// 定义一个全局变量用来存储接收的字符串

void IIC_Init() {
	
	/* 配置结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;											// 定义一个GPIO配置结构体
	
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		// 使能GPIOB时钟
	
	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						// GPIO输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					// 推挽输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;						// 上拉模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;// 配置Pin10~11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// 输出速度为50MHz
	
	/* 初始化GPIO */
	GPIO_Init(GPIOB, &GPIO_InitStructure);										// 应用GPIOB配置
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);					// 初始拉高SCK（Pin10）和SDA（Pin11）
}


// 发送起始信号，SCK高电平下，SDA由高变低
void IIC_Start() {
	
	IIC_Mode_Tx();	// 设置I2C为发送模式
	
	IIC_SCL = 1, IIC_SDA_OUT = 1;		// 拉高SCL和SDA，准备发送起始信号
	
	Delay_Us(4);
	IIC_SDA_OUT = 0;								// 在SCL高电平时，拉低SDA，发送起始信号
	
	Delay_Us(4);
	IIC_SCL = 0;										// 将SCL拉低，准备发送数据
	
}


// 发送结束信号，SCK高电平下，SDA由低变高
void IIC_Stop() {
	
	IIC_Mode_Tx();									// 设置I2C为发送模式
	
	IIC_SCL = 0, IIC_SDA_OUT = 0;		// 拉高SCL，拉低SDA
	
	Delay_Us(4);
	IIC_SCL = 1, IIC_SDA_OUT = 1;		// 拉高SDA，产生结束信号
	
	Delay_Us(4);
}


// 发送应答信号，接收完数据后将SDA拉高
void IIC_SendAck(void){ 
	
	IIC_Mode_Tx();
	
	IIC_SCL = 0;
	
	IIC_SDA_OUT = 0;
	
	Delay_Us(2);
	IIC_SCL = 1;
	
//	Delay_Us(2);
//	IIC_SCL = 0;
}


// 不发送应答信号，接收完数据后将SDA拉低
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
	
	u8 TimeoutCount = 0;	// 从设备应答超时计数
	
	if (*TimeoutFlag == False) return;
	
	IIC_Mode_Rx(); 				// SDA设置为输入 
	
	// 将SDA和SCL置为高电平，等待从设备发送SDA为低电平的应答信号
	IIC_SDA_OUT = 1;
	Delay_Us(1);
	
	IIC_SCL=1;
	Delay_Us(1);
	
	// 等待从设备将SDA拉低
	while(IIC_SDA_IN) {
		
		TimeoutCount++;
		
		// 当接受应答等待时间超过250个时钟周期后视为通信失败
		if(TimeoutCount>250) { 
			IIC_Stop();	// 发送通信停止信号给从设备
			*TimeoutFlag = False;
			return;
		}
	}
	
	IIC_SCL = 0; 		//时钟输出 0 
	
	*TimeoutFlag = True;
	return; 
} 


// 发送8位数据
void IIC_Send_Byte(u8 Data) {
	
	int i = 0;
	
	IIC_Mode_Tx();	// 将IIC设置为发送模式
	
	
	
	for(i=7;i>=0;i--) {
		
		IIC_SCL = 0;		// 拉低SCL电平，准备发送第一帧数据
		
		if (Data&(1<<i)) IIC_SDA_OUT = 1;
		else IIC_SDA_OUT = 0;
		Delay_Us(2);
		
		IIC_SCL = 1;
		Delay_Us(2);
		
		
	}
}


// 接收8位数据
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


// 发送一个字符串给从设备
bool IIC_Send_String(char *Str, u8 DeviceAddr, u16 RegisterAddr) {
	
	bool WaitAckFlag = True;							// 应答标志位
	
	// 当接收的数据超出最大限制或者设备地址超过127时，发送失败
	if (strlen(Str)>IIC_DATA_SIZE_MAX ||
		DeviceAddr>127) return False;
	
	IIC_Start();													// 发送IIC起始信号
	
	IIC_Send_Byte((DeviceAddr<<1)&0xfe);	// 发送设备地址
	IIC_WaitAck(&WaitAckFlag);						// 等待应答
	
	IIC_Send_Byte(RegisterAddr>>8);				// 发送寄存器地址高位
	IIC_WaitAck(&WaitAckFlag);						// 等待应答
	
	IIC_Send_Byte(RegisterAddr%256); 			// 发送寄存器低地址
	IIC_WaitAck(&WaitAckFlag); 						// 等待应答
	
	// 当遇到结束字符或者应答失效后停止发送
	while(*Str != '\n' && WaitAckFlag == True) {
		IIC_Send_Byte(*Str++);							// 发送一个字符
		IIC_WaitAck(&WaitAckFlag); 					// 等待应答
	}
		
	IIC_Stop();														// 发送IIC结束信号
	
	return WaitAckFlag;
}


// 接收一个字符串
void IIC_Read_String(u8 DeviceAddr, u16 RegisterAddr, u8 ReadLen) {
	
	u8 DataIndex = 0;												// 接收数据索引
	bool WaitAckFlag;												// 应答标志位
	
	// 当接收的数据超出最大限制或者设备地址超过127时，退出接收
	if (ReadLen>IIC_DATA_SIZE_MAX || 
		DeviceAddr > 127) return;
	
	while(DataIndex<ReadLen && WaitAckFlag == True) {
		
		IIC_Start();													// 发送IIC起始信号
		
		IIC_Send_Byte((DeviceAddr<<1)|0x01); 	// 发送设备地址
		IIC_WaitAck(&WaitAckFlag);
		
		IIC_Start(); 													// 发送开始信号
		
		IIC_Send_Byte(RegisterAddr>>8); 			// 发送寄存器地址高位
		IIC_WaitAck(&WaitAckFlag);
		
		IIC_Send_Byte((RegisterAddr++)%256); 	// 发送寄存器地址低位
		IIC_WaitAck(&WaitAckFlag);
		
		// 接收数据存入数组, 接收最后一个字节后无需应答
		RxData[DataIndex] = IIC_Read_Byte((DataIndex < ReadLen - 1));
		
		IIC_Stop();														// 停止发送
		DataIndex++;
	}
	
	RxData[DataIndex] = '\0';
}
