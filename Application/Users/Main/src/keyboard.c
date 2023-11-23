/*******************************************************************************
** 名称：keyboard.c
** 功能：按键状态处理
** 版本：V1.0
** 作者：张嘉良 
** 日期：2023年1月4日
*******************************************************************************/


#include <stdio.h>
#include "keyboard.h"
#include "stm32f4xx_gpio.h"


stcButtonObject_t * gpstcCurrButton;

stcButtonObject_t gstcKey01Button;	// KEY01
stcButtonObject_t gstcKey02Button;	// KEY02
stcButtonObject_t gstcKey03Button;	// KEY03
stcButtonObject_t gstcKey04Button;	// KEY04
stcButtonObject_t gstcKey05Button;	// KEY05


static unsigned char GPIO_GetKey01BtnState(void)
{
	return ((GPIOA->IDR & GPIO_Pin_2) != Bit_RESET);
}
static unsigned char GPIO_GetKey02BtnState(void)
{
	return ((GPIOB->IDR & GPIO_Pin_11) == Bit_RESET);
}
static unsigned char GPIO_GetKey03BtnState(void)
{
	return ((GPIOA->IDR & GPIO_Pin_7) == Bit_RESET);
}
static unsigned char GPIO_GetKey04BtnState(void)
{
	return ((GPIOB->IDR & GPIO_Pin_1) == Bit_RESET);
}
static unsigned char GPIO_GetKey05BtnState(void)
{
	return ((GPIOB->IDR & GPIO_Pin_10) == Bit_RESET);
}



stcButtonObject_t *pstcButtonTable[KEYBOARD_BUTTON_NUM] = 
{
	&gstcKey01Button, &gstcKey02Button, &gstcKey03Button,
	&gstcKey04Button, &gstcKey05Button
};


pu8ButtonState_t pstcButtonStateFuncTable[KEYBOARD_BUTTON_NUM] = 
{
	GPIO_GetKey01BtnState, GPIO_GetKey02BtnState, GPIO_GetKey03BtnState,
	GPIO_GetKey04BtnState, GPIO_GetKey05BtnState
};


// 键盘模块初始化
void Keyboard_Init(void)
{
	for(uint8_t i = 0; i < KEYBOARD_BUTTON_NUM; i++)
	{
		// 加入按钮
		Keyboard_AddButton(pstcButtonTable[i], pstcButtonStateFuncTable[i]);
	}
	
	// 设置按钮长按判定时间和双击判定时间
	Keyboard_SetBtnDecideTime(pstcButtonTable[0], 2000, 100);
	Keyboard_SetBtnDecideTime(pstcButtonTable[1], 1500, 100);
	Keyboard_SetBtnDecideTime(pstcButtonTable[2], 3000, 100);
	Keyboard_SetBtnDecideTime(pstcButtonTable[3], 1500, 100);
	Keyboard_SetBtnDecideTime(pstcButtonTable[4], 1500, 100);
}


void Keyboard_AddButton(stcButtonObject_t *pstcButton, pu8ButtonState_t pu8ButtonStateFunc)
{
	static stcButtonObject_t * gpstcHeadButton = NULL;
	
	if (pstcButton == NULL) return;
	
	if (gpstcHeadButton != NULL)
	{
		pstcButton->pstcButtonObjectPrev = gpstcHeadButton->pstcButtonObjectPrev;
		pstcButton->pstcButtonObjectNext = gpstcHeadButton;
		gpstcHeadButton->pstcButtonObjectPrev->pstcButtonObjectNext = pstcButton;
		gpstcHeadButton->pstcButtonObjectPrev = pstcButton;
		pstcButton->u8BtnNumber = pstcButton->pstcButtonObjectPrev->u8BtnNumber + 1;
	}
	else
	{
		pstcButton->pstcButtonObjectNext = pstcButton;
		pstcButton->pstcButtonObjectPrev = pstcButton;
		pstcButton->u8BtnNumber = 0;
		gpstcHeadButton = pstcButton;
		gpstcCurrButton = gpstcHeadButton;
	}
	
	pstcButton->pu8GetKeyStateFunc = pu8ButtonStateFunc;
}


void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, 
							   uint16_t u16LongPressTime, uint16_t u16MultiActiveTime)
{
	pstcButton->u16LongPressDecideTime = u16LongPressTime;
	pstcButton->u16MultiPressIntervalTime = u16MultiActiveTime;
	pstcButton->u16MultiLooseIntervalTime = u16MultiActiveTime;
}


static void Keyboard_Debounce(void)
{
	uint8_t u8CurrBtnState = gpstcCurrButton->pu8GetKeyStateFunc();
	
	// 当按钮状态发生变化时
	if (u8CurrBtnState != gpstcCurrButton->u8BtnStateDebounceBefore)
	{
		// 记录发生变化时的时刻
		if (gpstcCurrButton->u32BtnDebounceCnt == 0)
		{
			gpstcCurrButton->u32BtnDebounceCnt = KEYBOARD_GET_TIMEBASE();
		}
		// 经过一定的按钮消抖时间后再获取状态，实现按钮消抖
		else if (((uint32_t)(KEYBOARD_GET_TIMEBASE() - gpstcCurrButton->u32BtnDebounceCnt)) > KEYBOARD_DEBOUNCE_TIME)
		{
			gpstcCurrButton->u8BtnStateDebounceAfter = u8CurrBtnState;
			gpstcCurrButton->u32BtnDebounceCnt = 0;
		}
	}
	else
	{
		gpstcCurrButton->u32BtnDebounceCnt = 0;
	}
}


// 键盘按键扫描模块
static void Keyboard_Scan(void)
{
	switch(gpstcCurrButton->u8BtnStateDebounceBefore | (gpstcCurrButton->u8BtnStateDebounceAfter << 1))
	{
		// 按键松开后
		case 0:
			gpstcCurrButton->enBtnState = enBtnStatus_LOOSEN;
		
			if (gpstcCurrButton->u8BtnPressCnt > 0)
			{
				if (((uint32_t)(KEYBOARD_GET_TIMEBASE() - gpstcCurrButton->u32MultiPressIntervalCnt)) >= 
					gpstcCurrButton->u16MultiPressIntervalTime)
				{
					gpstcCurrButton->enBtnState = (enButtonStatus_t)(gpstcCurrButton->u8BtnPressCnt + enBtnStatus_DOUBLEPRESS_UP);
					gpstcCurrButton->u8BtnPressCnt = 0;
				}
			}
			
			if (gpstcCurrButton->u8BtnLooseCnt > 0)
			{
				if (((uint32_t)(KEYBOARD_GET_TIMEBASE() - gpstcCurrButton->u32MultiLooseIntervalCnt)) >= 
					gpstcCurrButton->u16MultiLooseIntervalTime)
				{
					gpstcCurrButton->enBtnState = (enButtonStatus_t)(gpstcCurrButton->u8BtnLooseCnt);
					gpstcCurrButton->u8BtnLooseCnt = 0;
				}
			}
			break;
		// 按键被松开时
		case 1:
			if (gpstcCurrButton->u8BtnLooseCnt == 0)
				gpstcCurrButton->u32MultiLooseIntervalCnt = KEYBOARD_GET_TIMEBASE();
			gpstcCurrButton->u8BtnLooseCnt = (gpstcCurrButton->u8BtnLooseCnt + 1) % 3;
			break;
		// 按钮被按下时
		case 2:
			if (gpstcCurrButton->u8BtnPressCnt == 0)
				gpstcCurrButton->u32MultiPressIntervalCnt = KEYBOARD_GET_TIMEBASE();
			gpstcCurrButton->u8BtnPressCnt = (gpstcCurrButton->u8BtnPressCnt + 1) % 3;
			break;
		// 按钮被按下后
		case 3:
			if (gpstcCurrButton->u8BtnPressCnt > 0)
			{
				if (((uint32_t)(KEYBOARD_GET_TIMEBASE() - gpstcCurrButton->u32MultiPressIntervalCnt)) >= 
					gpstcCurrButton->u16LongPressDecideTime)
				{
					gpstcCurrButton->enBtnState = enBtnStatus_LONGPRESS;
					gpstcCurrButton->u8BtnPressCnt = 0;
					gpstcCurrButton->u8BtnLooseCnt = 2;
				}
			}
			break;
		default:
			break;
	}
	
	// 按钮状态锁存
	gpstcCurrButton->u8BtnStateDebounceBefore = gpstcCurrButton->u8BtnStateDebounceAfter;
	// 切换至下一个按键
	gpstcCurrButton = gpstcCurrButton->pstcButtonObjectNext;
}


// 按键状态处理
void Keyboard_Process(void)
{
	// 按键消抖处理
	Keyboard_Debounce();
	
	// 按键扫描处理
	Keyboard_Scan();
}
