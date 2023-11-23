#include "spi.h"



char Spi1_ReceiveData[50];

uint8_t Spi3_DmaWriteData[SPI3_RTX_BUFF_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t Spi3_DmaReadData[SPI3_RTX_BUFF_SIZE];

#if 0
// 配置SPI通信
void SPI1_Init(void) {

	/* 配置结构体定义 */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 时钟配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* GPIO配置 */
	//配置引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5 复用为 SPI1
	// 配置引脚号
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	// 设置GPIO为复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// 设置IO口为推挽输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// 设置端口为上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// 设置输出速度50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* 配置SPI1 */
	// 初始化SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位 SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位 SPI1
	// 设置SPI时钟分频
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// 设置在时钟的第二个跳变沿采集数据
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// 设置时钟线在低电平时为休眠状态
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// 设置SPI1的通信模式为主机模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	// 设置数据传输从MSB位开始
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// 设置NSS信号由软件控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCR校验多项式，大于1即可
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1传输的位数为8位
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// 设置SPI1具有收发功能
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* 应用配置 */
	// 初始化GPIOA5、6、7配置
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 初始化SPI1配置	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* 使能/失能 */
	// 使能接收中断
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	
	// 使能SPI1
	SPI_Cmd(SPI1, ENABLE);
}
#endif

// 配置SPI通信
void SPI1_Init(void) {

	/* 配置结构体定义 */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 时钟配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* GPIO配置 */
	//配置引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5 复用为 SPI1
	// 配置引脚号
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	// 设置GPIO为复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// 设置IO口为推挽输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// 设置端口为上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// 设置输出速度50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* 配置SPI1 */
	// 初始化SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位 SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位 SPI1
	// 设置SPI时钟分频
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// 设置在时钟的第二个跳变沿采集数据
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// 设置时钟线在低电平时为休眠状态
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// 设置SPI1的通信模式为主机模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	// 设置数据传输从MSB位开始
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// 设置NSS信号由软件控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCR校验多项式，大于1即可
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1传输的位数为8位
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// 设置SPI1具有收发功能
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* 应用配置 */
	// 初始化GPIOA5、6、7配置
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 初始化SPI1配置	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_SSOutputCmd(SPI3, DISABLE);
	
	/* 使能/失能 */
	// 使能接收中断
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	
	// 使能SPI1
	SPI_Cmd(SPI1, ENABLE);
}


void SPI3_Init(void)
{
	/* 配置结构体定义 */
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 时钟配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/* GPIO配置 */

	// 配置引脚号
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
	// 设置GPIO为复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	// 设置IO口为推挽输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// 设置端口为上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// 设置输出速度50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 初始化GPIOA5、6、7配置
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//配置引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); //PC10 复用为 SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); //PC11 复用为 SPI3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3); //PC12 复用为 SPI3
	
	/* 配置SPI1 */
	// 初始化SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//复位 SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//停止复位 SPI1
	// 设置SPI时钟分频
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	// 设置在时钟的第二个跳变沿采集数据
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// 设置时钟线在低电平时为休眠状态
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	// 设置SPI1的通信模式为主机模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	// 设置数据传输从MSB位开始
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// 设置NSS信号由软件控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// RCR校验多项式，大于1即可
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI1传输的位数为8位
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// 设置SPI1具有收发功能
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	
	/* 应用配置 */
	// 初始化SPI1配置	
	SPI_Init(SPI3, &SPI_InitStructure);
	
	// 使能SPI DMA收发功能
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
	
	// 使能SPI1
	SPI_Cmd(SPI3, ENABLE);
}


// 使用SPI1发送数据
bool SPI1_Send(char* SendData) {
	
	// 在没有遇到结束字符时一直发送
	while(*SendData != '\0') {
		// 等待发送缓冲区清空
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		
		SPI_I2S_SendData(SPI1, *(SendData++));
	}
	return true;
}


// SPI1通信<中断函数>
void SPI1_IRQHandler() {
	
	u8 RxData;				// 定义一个接收每个字节的变量
	static u8 RxIndex = 0;	// 接受数组的索引
	// SPI接收缓冲区非空标志
	FlagStatus ReadFlag = SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE);
	
	// 进入中断后，当接受缓冲区不为空
	if (ReadFlag != RESET) {
		RxData = SPI_I2S_ReceiveData(SPI1);	// 获取接收缓冲区的值
		Spi1_ReceiveData[RxIndex++] = RxData;	// 存入接收数据数组中
		
		// 当遇到换行字符后终止接收
		if (RxData == '\n') {
			RxIndex = 0;
			Spi1_ReceiveData[RxIndex-1] = '\0'; // 将最后位设置为结束字符
			return;
		}
	}
}
