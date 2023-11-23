#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#define KEYBOARD_BUTTON_NUM					(5)
#define KEYBOARD_HANDLER_BIND_NUM			(26)
#define KEYBOARD_DEBOUNCE_TIME				(10)	// 键盘按键消抖时间（ms）

#define KEYBOARD_LONGPRESS_KEEP				(0)		// 长按信号保持

#define KEYBOARD_GET_TIMEBASE()				(0)

#define BTN01_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 0)	// BTN01单击按下信号
#define BTN01_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 0)	// BTN01双击按下信号
#define BTN01_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 0)	// BTN01长按信号
#define BTN01_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 0)	// BTN01单击松开信号
#define BTN01_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 0)	// BTN01双击松开信号

// Button signal: [bit = 3]
#define BTN02_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 3)	
#define BTN02_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 3)	
#define BTN02_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 3)	
#define BTN02_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 3)	
#define BTN02_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 3)	

// Button signal: [bit = 6]
#define BTN03_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 6)	
#define BTN03_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 6)	
#define BTN03_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 6)	
#define BTN03_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 6)	
#define BTN03_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 6)	

// Button signal: [bit = 9]
#define BTN04_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 9)	
#define BTN04_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 9)	
#define BTN04_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 9)	
#define BTN04_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 9)	
#define BTN04_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 9)	

// Button signal: [bit = 12]
#define BTN05_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 12)
#define BTN05_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 12)
#define BTN05_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 12)
#define BTN05_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 12)
#define BTN05_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 12)

// Button signal: [bit = 15]
#define BTN06_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 15)
#define BTN06_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 15)
#define BTN06_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 15)
#define BTN06_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 15)
#define BTN06_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 15)

// Button signal: [bit = 18]
#define BTN07_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 18)
#define BTN07_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 18)
#define BTN07_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 18)
#define BTN07_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 18)
#define BTN07_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 18)

// Button signal: [bit = 21]
#define BTN08_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 21)
#define BTN08_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 21)
#define BTN08_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 21)
#define BTN08_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 21)
#define BTN08_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 21)

// Button signal: [bit = 24]
#define BTN09_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 24)
#define BTN09_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 24)
#define BTN09_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 24)
#define BTN09_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 24)
#define BTN09_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 24)

// Button signal: [bit = 27]
#define BTN10_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 27)
#define BTN10_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 27)
#define BTN10_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 27)
#define BTN10_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 27)
#define BTN10_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 27)

// Button signal: [bit = 30]
#define BTN11_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 30)
#define BTN11_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 30)
#define BTN11_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 30)
#define BTN11_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 30)
#define BTN11_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 30)

// Button signal: [bit = 33]
#define BTN12_ONCEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_DW   << 33)
#define BTN12_DOUBLEPRESS_DW_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_DW << 33)
#define BTN12_LONGPRESS_SIGNAL			((uint64_t)enBtnStatus_LONGPRESS	  << 33)
#define BTN12_ONCEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_ONCEPRESS_UP   << 33)
#define BTN12_DOUBLEPRESS_UP_SIGNAL		((uint64_t)enBtnStatus_DOUBLEPRESS_UP << 33)


typedef unsigned char (*pu8ButtonState_t)(void);


typedef enum enButtonStatus
{
	enBtnStatus_LOOSEN,			// 按键松开
	enBtnStatus_ONCEPRESS_UP,	// 按键单击松开
	enBtnStatus_DOUBLEPRESS_UP,	// 按钮双击松开
	enBtnStatus_ONCEPRESS_DW,	// 按键单击按下
	enBtnStatus_DOUBLEPRESS_DW,	// 按键双击按下
	enBtnStatus_LONGPRESS,		// 按键长按
} enButtonStatus_t;


typedef struct stcButtonObject
{
	enButtonStatus_t enBtnState;
	enButtonStatus_t enBtnStatePrev;
	
	unsigned char u8BtnNumber;
	
	unsigned char u8BtnLooseCnt;
	unsigned char u8BtnPressCnt;
	
	unsigned char u8BtnStateDebounceAfter;
	unsigned char u8BtnStateDebounceBefore;
	
	unsigned short u16LongPressDecideTime;
	unsigned short u16MultiPressIntervalTime;
	unsigned short u16MultiLooseIntervalTime;
	
	unsigned int u32BtnDebounceCnt;
	unsigned int u32MultiPressIntervalCnt;
	unsigned int u32MultiLooseIntervalCnt;
	
	struct stcButtonObject *pstcButtonObjectNext;
	struct stcButtonObject *pstcButtonObjectPrev;
	
	pu8ButtonState_t pu8GetKeyStateFunc;
} stcButtonObject_t;


extern stcButtonObject_t gstcKey01Button;	// KEY01
extern stcButtonObject_t gstcKey02Button;	// KEY02
extern stcButtonObject_t gstcKey03Button;	// KEY03
extern stcButtonObject_t gstcKey04Button;	// KEY04
extern stcButtonObject_t gstcKey05Button;	// KEY05

extern stcButtonObject_t *gpstcCurrButton;


void Keyboard_Init(void);
void Keyboard_Process(void);
void Keyboard_AddButton(stcButtonObject_t *pstcButton, pu8ButtonState_t pButtonStateFunc);
void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, 
							   unsigned short u16LongPressTime, 
							   unsigned short u16MultiActiveTime);

#endif
