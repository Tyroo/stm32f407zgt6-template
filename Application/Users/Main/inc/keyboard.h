#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#define KEYBOARD_BUTTON_NUX		(4)
#define KEYBOARD_DEBOUNCE_TIME	(10)	// ���̰�������ʱ�䣨ms��


typedef uint8_t (*pButtonState_t)(void);


typedef enum enButtonStatus
{
	enBtnStatus_LOOSEN,			// �����ɿ�
	enBtnStatus_LONGPRESS,		// ��������
	enBtnStatus_ONCEPRESS,		// ��������
	enBtnStatus_DOUBLEPRESS,	// ����˫��
} enButtonStatus_t;


typedef struct stcButtonObject
{
	enButtonStatus_t enBtnState;
	enButtonStatus_t enBtnStatePrev;
	
	uint8_t u8BtnNumber;
	int8_t s8BtnLoosenCnt;
	
	uint8_t u8BtnStateDebounceAfter;
	uint8_t u8BtnStateDebounceBefore;
	
	bool bBtnDebounceFlg;
	uint32_t u16BtnDebounceCnt;
	
	uint16_t u16LongPressDecideTime;
	uint16_t u16MultiPressIntervalTime;
	
	uint32_t u16LongPressDecideCnt;
	uint32_t u16MultiPressIntervalCnt;
	
	struct stcButtonObject *pstcButtonObjectNext;
	struct stcButtonObject *pstcButtonObjectPrev;
	
	pButtonState_t GetKeyState;
} stcButtonObject_t;


extern stcButtonObject_t stcPowerButton;// ��Դ��
extern stcButtonObject_t stcBackButton;	// ���ؼ�
extern stcButtonObject_t stcReadButton;	// ������
extern stcButtonObject_t stcZeroButton;	// �����
extern stcButtonObject_t stcModeButton;	// ģʽ��

extern stcButtonObject_t *pstcCurrButton;
extern stcEventSource_t stcBtnEventSource;


void Keyboard_Init(void);
void Keyboard_AddButton(stcButtonObject_t *pstcButton, pButtonState_t pButtonStateFunc);
void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, uint16_t u16LongPressTime, uint16_t u16MultiPressTime);
void Keyboard_Process(void);


#endif
