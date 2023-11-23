/* 延时模块（验证通过） */
#include "delay.h"


// 延时模块初始化函数
void Delay_Init() {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	// 设置SysTick的时钟(21MHz)
}


// 实现微秒级延时
void Delay_Us(uint32_t Us) {
	
	uint32_t Temp;
	uint32_t CountValue = Us * DELAY_ONE_US;	 // 计算出延时Us微秒需要计数的值
	
	SysTick->LOAD = CountValue; 						   // 时间加载 
	SysTick->VAL = 0x00; 											 // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 开始倒数

	// 当systick定时器递减计数器未减至0且减计数器处于开启时一直阻塞
	do {
		Temp = SysTick->CTRL;
	} while((Temp&0x01) && !(Temp&(1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 停止定时器
	SysTick->VAL = 0x00; 											 // 清空计数器
}


// 实现毫秒级延时
void Delay_Ms(uint32_t Ms) {
	
	uint32_t Temp;
	uint32_t CountValue = Ms * DELAY_ONE_MS;				 // 计算出延时Ms毫秒需要计数的值
	
	SysTick->LOAD = CountValue; 						   // 时间加载 -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SysTick->VAL = 0x00; 											 // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 开始倒数
	
	// 当systick定时器递减计数器未减至0且递减计数器处于开启时一直阻塞
	do {
		Temp = SysTick->CTRL;
	} while((Temp&0x01) && !(Temp&(1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 停止定时器
	SysTick->VAL = 0x00; 											 // 清空计数器
}

