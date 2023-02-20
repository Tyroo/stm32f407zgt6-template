#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#include "main.h"


#define KEYBOARD_BUTTON_NUX		(4)
#define KEYBOARD_DEBOUNCE_TIME	(50)	// 键盘按键消抖时间（ms）


typedef unsigned char (*pButtonState_t)(void);

typedef enum enButtonStatus
{
	enBtnStatus_LOOSEN,			// 按键松开
	enBtnStatus_ONCEPRESS_DW,	// 按键单击按下
	enBtnStatus_DOUBLEPRESS_DW,	// 按键双击按下
	enBtnStatus_LONGPRESS,		// 按键长按
	enBtnStatus_ONCEPRESS_UP,	// 按键单击松开
	enBtnStatus_DOUBLEPRESS_UP	// 按钮双击松开
} enButtonStatus_t;


typedef struct stcButtonObject
{
	enButtonStatus_t enBtnState;
	enButtonStatus_t enBtnStatePrev;
	
	unsigned char u8BtnNumber;
	unsigned char s8BtnLoosenCnt;
	
	unsigned char u8BtnStateDebounceAfter;
	unsigned char u8BtnStateDebounceBefore;
	
	unsigned char bBtnDebounceFlg;
	unsigned int u16BtnDebounceCnt;
	
	unsigned short u16LongPressDecideTime;
	unsigned short u16MultiPressIntervalTime;
	
	unsigned int u16LongPressDecideCnt;
	unsigned int u16MultiPressIntervalCnt;
	
	struct stcButtonObject *pstcButtonObjectNext;
	struct stcButtonObject *pstcButtonObjectPrev;
	
	pButtonState_t GetKeyState;
} stcButtonObject_t;

extern stcButtonObject_t stcPowerButton;// 电源键
extern stcButtonObject_t stcBackButton;	// 返回键
extern stcButtonObject_t stcReadButton;	// 读数键
extern stcButtonObject_t stcZeroButton;	// 调零键
extern stcButtonObject_t stcModeButton;	// 模式键

extern stcEventSource_t stcBtnEventSource;

void Keyboard_Init(void);
void Keyboard_AddButton(stcButtonObject_t *pstcButton, pButtonState_t pButtonStateFunc);
void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, unsigned short u16LongPressTime, unsigned short u16MultiPressTime);
void Keyboard_Process(void);

#endif
