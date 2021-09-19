#include "key.h"
#include "delay.h"


//按键初始化函数
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, 
	ENABLE);//使能 GPIOA,GPIOE 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	//KEY0 KEY1 KEY2 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化 GPIOE2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP 对应引脚 PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 GPIOA0
} 



// 按键扫描函数，mode：0不支持连续按，1支持连续按
u8 Key_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	
	if(mode) key_up=1; //支持连按 
	
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)) {
		Delay_Ms(10);//去抖动
		key_up=0;
		if(KEY0==0) return 1;
		else if(KEY1==0) return 2;
		else if(KEY2==0) return 3;
		else if(WK_UP==1) return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0) {
		key_up=1; 
	}else {}
	return 0;// 无按键按下
}


// 自锁按钮KEY0
u8 Key0_Scan() {
	
	static u8 KeyUp = 1;
	static u8 KeyCode = 0;
	
	if (KeyUp && (KEY0 == 0)) {
		Delay_Ms(10);	//去抖动
		if (KEY0 == 0) {
			KeyUp = 0;
			KeyCode += 1;
			KeyCode %= 2;
		}
		
	} else if (KEY0 == 1) {
		KeyUp = 1;
	}
	return KeyCode;
}
