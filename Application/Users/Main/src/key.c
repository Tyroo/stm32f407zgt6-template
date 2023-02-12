#include "key.h"
#include "delay.h"


//按键初始化函数
void Key_Init(void)
{

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


// 三状态按钮扫描
u8 PressCount = 0;
u32 PointPressCount = 0;
u32 LongPressCount = 0;

extern uint32_t lwip_sys_timer;	//lwip本地时间计数器,单位:ms

enKeyPress ThreeStageKeyScan(void)
{
	enKeyPress enRet = enNoPress;
	
	static u8 u8KeyState = 3;
	
	static u8 u8KeyOld = 1;
	static u8 u8KeyNew = 1;
	static u32 u32LongPressDealyCnt = 0;
	static u32 u32OncePressDealyCnt = 0;
	
	static s8 s8KeyPressCnt = 0;

	static u16 u16OldTimeCnt = 0;
	static u16 u16TimerCntMs = 0;
	static u8 u8KeyStateLock = 0;

	
	/* 按键消抖 */
	if((KEY0 != u8KeyOld) && (u8KeyStateLock == 0))
	{
		u16OldTimeCnt = lwip_sys_timer;
		u8KeyStateLock = 1;
	}
	else if (u8KeyStateLock == 1)
	{
		u16TimerCntMs = lwip_sys_timer - u16OldTimeCnt;
		if (u16TimerCntMs > 9)
		{
			u8KeyStateLock = 0;
			u8KeyNew = KEY0;
		}
	}
	
	/* 按键状态获取 */
	u8KeyState = u8KeyOld + (u8KeyNew<<1);
	u8KeyOld = u8KeyNew;
	
	/* 按键动作处理 */
	// 当按钮被松开时
	if(u8KeyState == 2)
	{
		u32LongPressDealyCnt = 0;
		s8KeyPressCnt++;
	}
	// 当按钮一直处于按下状态时
	else if(u8KeyState == 0)
	{
		u32LongPressDealyCnt++;
		if (u32LongPressDealyCnt > 1700000)
		{
			s8KeyPressCnt = -1;
			enRet = enLongPress;
		}
	}
	// 按钮被松开一次后
	else if(s8KeyPressCnt > 0)
	{
		u32OncePressDealyCnt++;
		if (u32OncePressDealyCnt > 350000)
		{
			enRet = (s8KeyPressCnt == 1) ? enOncePress : enDoublePress;
			u32OncePressDealyCnt = s8KeyPressCnt = 0;
		}
	}
	
	return enRet;
}
