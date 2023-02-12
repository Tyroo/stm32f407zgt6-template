#include "main.h"

/*******************************************************************************
** 名称：keyboard.c
** 功能：按键状态处理
** 版本：V1.0
** 作者：张嘉良 
** 日期：2023年1月4日
*******************************************************************************/


stcButtonObject_t *pstcCurrButton;
stcEventSource_t stcBtnEventSource;

stcButtonObject_t stcKEY0Button;	// KEY0键
stcButtonObject_t stcKEY1Button;	// KEY1键
stcButtonObject_t stcKEY2Button;	// KEY2键
stcButtonObject_t stcWKUPButton;	// WAKE_UP键

static uint8_t GetKEY0BtnState(void);
static uint8_t GetKEY1BtnState(void);
static uint8_t GetKEY2BtnState(void);
static uint8_t GetWKUPBtnState(void);

stcButtonObject_t *pstcButtonTable[KEYBOARD_BUTTON_NUX] = 
{
	&stcKEY0Button, &stcKEY1Button, &stcKEY2Button,
	&stcWKUPButton
};

pButtonState_t pstcButtonStateFuncTable[KEYBOARD_BUTTON_NUX] = 
{
	&GetKEY0BtnState, &GetKEY1BtnState, &GetKEY2BtnState,
	&GetWKUPBtnState
};


static uint8_t GetKEY0BtnState(void) { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)); }
static uint8_t GetKEY1BtnState(void)  { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)); }
static uint8_t GetKEY2BtnState(void)  { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)); }
static uint8_t GetWKUPBtnState(void)  { return (uint8_t)GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); }


// 键盘模块初始化
void Keyboard_Init(void)
{
	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);// 使能 GPIOA,GPIOE 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; // KEY0 KEY1 KEY2 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化 GPIOE2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP 对应引脚 PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 GPIOA0
	
	/* 键盘对象初始化 */
	if (pstcButtonTable[0] == NULL)
		for( ; ; );
	
	pstcCurrButton = pstcButtonTable[0];
	pstcCurrButton->pstcButtonObjectNext = pstcCurrButton;
	pstcCurrButton->pstcButtonObjectPrev = pstcCurrButton;

	for(uint8_t i=0; i<KEYBOARD_BUTTON_NUX; i++)
	{
		Keyboard_AddButton(
			pstcButtonTable[i], 
			pstcButtonStateFuncTable[i]
		);
		pstcButtonTable[i]->u8BtnNumber = i;
	}
}


void Keyboard_AddButton(stcButtonObject_t *pstcButton, pButtonState_t pButtonStateFunc)
{
	stcButtonObject_t *pstcFirstButton = pstcButtonTable[0];
	
	if (pstcButton != NULL)
	{
		if (pstcButton != pstcFirstButton)
		{
			pstcButton->pstcButtonObjectPrev = pstcFirstButton->pstcButtonObjectPrev;
			pstcButton->pstcButtonObjectNext = pstcFirstButton;
			pstcFirstButton->pstcButtonObjectPrev->pstcButtonObjectNext = pstcButton;
			pstcFirstButton->pstcButtonObjectPrev = pstcButton;
		}
		pstcButton->GetKeyState = pButtonStateFunc;
		Keyboard_SetBtnDecideTime(pstcButton, 2000, 50);
	}
}


void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, 
		uint16_t u16LongPressTime, uint16_t u16MultiPressTime)
{
	pstcButton->u16LongPressDecideTime = u16LongPressTime;
	pstcButton->u16MultiPressIntervalTime = u16MultiPressTime;
}


static void Keyboard_Debounce(stcButtonObject_t *pstcBtnObject)
{
	uint8_t u8CurrBtnState = 0;
	
	u8CurrBtnState = pstcBtnObject->GetKeyState();
	
	// 当按钮状态发生变化时
	if ((u8CurrBtnState != pstcBtnObject->u8BtnStateDebounceBefore)
		 && (pstcBtnObject->bBtnDebounceFlg == false))
	{
		// 记录发生变化时的时刻
		pstcBtnObject->u16BtnDebounceCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
		// 按钮消抖标志设置为true
		pstcBtnObject->bBtnDebounceFlg = true;
	}
	// 执行按钮消抖
	else if (pstcBtnObject->bBtnDebounceFlg)
	{
		// 经过一定的按钮消抖时间后再获取状态，实现按钮消抖
		if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
			 pstcBtnObject->u16BtnDebounceCnt) > KEYBOARD_DEBOUNCE_TIME)
		{
			pstcBtnObject->bBtnDebounceFlg = false;
			pstcBtnObject->u8BtnStateDebounceAfter = u8CurrBtnState;
		}
	}
}


// 键盘按键扫描模块
static void Keyboard_Scan(stcButtonObject_t *pstcBtnObject)
{
	uint8_t u8BtnStateValue = 0;
	
	u8BtnStateValue = pstcBtnObject->u8BtnStateDebounceBefore + (pstcBtnObject->u8BtnStateDebounceAfter << 1);
	pstcBtnObject->u8BtnStateDebounceBefore = pstcBtnObject->u8BtnStateDebounceAfter;
	
	switch(u8BtnStateValue)
	{
		// 按键松开后
		case 0:
			if (pstcBtnObject->s8BtnLoosenCnt < 1)
			{
				pstcBtnObject->enBtnState = enBtnStatus_LOOSEN;
			}
			else
			{
				if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
					 pstcBtnObject->u16MultiPressIntervalCnt) > pstcBtnObject->u16MultiPressIntervalTime)
				{
					pstcBtnObject->enBtnState = (enButtonStatus_t)(pstcBtnObject->s8BtnLoosenCnt + 1);
					pstcBtnObject->s8BtnLoosenCnt = 0;
				}
			}
			break;
		// 按键被松开时
		case 1:
			pstcBtnObject->s8BtnLoosenCnt++;
			if (pstcBtnObject->s8BtnLoosenCnt == 1)
				pstcBtnObject->u16MultiPressIntervalCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			break;
		// 按钮被按下时
		case 2:
			pstcBtnObject->u16LongPressDecideCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			break;
		// 按钮被按下后
		case 3:
			if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
				 pstcBtnObject->u16LongPressDecideCnt) > pstcBtnObject->u16LongPressDecideTime)
			{
				pstcBtnObject->s8BtnLoosenCnt = - 1;
				pstcBtnObject->enBtnState = enBtnStatus_LONGPRESS;
			}
			break;
		default:
			break;
	}
}


// 按钮信号发送
static void Keyboard_SignalSend(stcEventSource_t *pstcEventSource)
{
	static EventSignal_t u32CurrBtnSignal = 0;
	
	// 如果按钮状态变更则发送对应的事件信号
	if (pstcCurrButton->enBtnState != pstcCurrButton->enBtnStatePrev)
	{
		pstcCurrButton->enBtnStatePrev = pstcCurrButton->enBtnState;
		
		u32CurrBtnSignal &= ~(((EventSignal_t)3) << (pstcCurrButton->u8BtnNumber * 2));
		u32CurrBtnSignal |= ((EventSignal_t)pstcCurrButton->enBtnState << (pstcCurrButton->u8BtnNumber * 2));
		
		Event_SignalGenerate(
			pstcEventSource, 
			u32CurrBtnSignal, 
			pstcCurrButton
		);
	}
}


// 按键状态处理
void Keyboard_Process(void)
{
	// 按键消抖处理
	Keyboard_Debounce(pstcCurrButton);
	
	// 按键扫描处理
	Keyboard_Scan(pstcCurrButton);
	
	// 按钮信号发送
	Keyboard_SignalSend(&stcBtnEventSource);
	
	pstcCurrButton = pstcCurrButton->pstcButtonObjectNext;
}
