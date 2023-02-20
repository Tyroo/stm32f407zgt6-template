#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#include "main.h"


#define KEYBOARD_BUTTON_NUX		(4)
#define KEYBOARD_DEBOUNCE_TIME	(50)	// ���̰�������ʱ�䣨ms��


typedef unsigned char (*pButtonState_t)(void);

typedef enum enButtonStatus
{
	enBtnStatus_LOOSEN,			// �����ɿ�
	enBtnStatus_ONCEPRESS_DW,	// ������������
	enBtnStatus_DOUBLEPRESS_DW,	// ����˫������
	enBtnStatus_LONGPRESS,		// ��������
	enBtnStatus_ONCEPRESS_UP,	// ���������ɿ�
	enBtnStatus_DOUBLEPRESS_UP	// ��ť˫���ɿ�
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

extern stcButtonObject_t stcPowerButton;// ��Դ��
extern stcButtonObject_t stcBackButton;	// ���ؼ�
extern stcButtonObject_t stcReadButton;	// ������
extern stcButtonObject_t stcZeroButton;	// �����
extern stcButtonObject_t stcModeButton;	// ģʽ��

extern stcEventSource_t stcBtnEventSource;

void Keyboard_Init(void);
void Keyboard_AddButton(stcButtonObject_t *pstcButton, pButtonState_t pButtonStateFunc);
void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, unsigned short u16LongPressTime, unsigned short u16MultiPressTime);
void Keyboard_Process(void);

#endif
