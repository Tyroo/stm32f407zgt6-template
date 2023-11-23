#ifndef __EVENT_H__
#define __EVENT_H__


/** 
  * @defgroup Event modules parameter define
  * \{
  */

#define EVENT_SOURCE_QUEUE_MAX_NUM			(10)
#define EVENT_ASSERT(v)						do {if ((v) == 0) return;} while(0)

/**
  * \}
  */


/** 
  * @defgroup Signal Define
  * @brief    
  *			\Example\
  *			#define CUSTOM_0_SIGNAL					((EventSignal_t)0b1    << ( 0 ))
  *			#define CUSTOM_1_SIGNAL					((EventSignal_t)0b11   << ( 1 ))
  *			#define CUSTOM_2_SIGNAL					((EventSignal_t)0b111  << ( 3 ))
  *			#define CUSTOM_3_SIGNAL					((EventSignal_t)0b1111 << ( 6 ))
  *			#define 		...											...
  *			#define CUSTOM_N_SIGNAL					((EventSignal_t)1 << (bit + N))
  *
  *			\TODO\ 
  *			Range: N >= 0 && N <= 31
  * \{
  */

// Button signal: [bit = 0]
#define KEY0_BTN_ONCEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_DW   << 0)	// KEY0单击按下信号
#define KEY0_BTN_DOUBLEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_DW << 0)	// KEY0双击按下信号
#define KEY0_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS	   << 0)	// KEY0长按信号
#define KEY0_BTN_ONCEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_UP   << 0)	// KEY0单击松开信号
#define KEY0_BTN_DOUBLEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_UP << 0)	// KEY0双击松开信号

// Button signal: [bit = 3]
#define KEY1_BTN_ONCEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_DW   << 3)	// KEY1单击按下信号
#define KEY1_BTN_DOUBLEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_DW << 3)	// KEY1双击按下信号
#define KEY1_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS	   << 3)	// KEY1长按信号
#define KEY1_BTN_ONCEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_UP   << 3)	// KEY1单击松开信号
#define KEY1_BTN_DOUBLEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_UP << 3)	// KEY1双击松开信号

// Button signal: [bit = 6]
#define KEY2_BTN_ONCEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_DW   << 6)	// KEY2单击按下信号
#define KEY2_BTN_DOUBLEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_DW << 6)	// KEY2双击按下信号
#define KEY2_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS	   << 6)	// KEY2长按信号
#define KEY2_BTN_ONCEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_UP   << 6)	// KEY2单击松开信号
#define KEY2_BTN_DOUBLEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_UP << 6)	// KEY2双击松开信号

// Button signal: [bit = 9]
#define WKUP_BTN_ONCEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_DW   << 9)	// WKUP单击按下信号
#define WKUP_BTN_DOUBLEPRESS_DW_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_DW << 9)	// WKUP双击按下信号
#define WKUP_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS	   << 9)	// WKUP长按信号
#define WKUP_BTN_ONCEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_ONCEPRESS_UP   << 9)	// WKUP单击松开信号
#define WKUP_BTN_DOUBLEPRESS_UP_SIGNAL		((EventSignal_t)enBtnStatus_DOUBLEPRESS_UP << 9)	// WKUP双击松开信号

// Custom signal: [bit = 12]
#define CUSTOM_TEST_SIGNAL					((EventSignal_t)1 << 12)							// 自定义测试事件

/**
  * \}
  */

/** @defgroup
  * @{
  */

typedef enum enEventStatus
{
	enEventStatus_NULL,			// 销毁事件
	enEventStatus_ENABLE,		// 在事件被遍历之前使能
	enEventStatus_AT_ENABLE,	// 在事件被遍历之后使能
	enEventStatus_DISABLE,		// 失能该事件
} enEventStatus_t;

typedef struct stcEventHandler
{
	unsigned int u32EventRegisterSignal;
	enEventStatus_t enEventState;
	
	struct stcEventSource *pstcEventSource;
	
	struct stcEventHandler *_pstcEventHandlerNext;
	struct stcEventHandler *_pstcEventHandlerPrev;
	
	void (*pEventFunc)(struct stcEventHandler *pstcEventHandler);
	
} stcEventHandler_t;

typedef struct stcEventSource
{
	void *pObject;
	stcEventHandler_t *pstcEventHandlerList;
	unsigned char u8EventTotalNumber;
	unsigned char u8EventWaitNumber;
	unsigned int u32EventTriggerSignal;
} stcEventSource_t;

typedef struct stcEventManage
{
	unsigned int u32CurrEventSignal;
	unsigned int u32PrevEventSignal;
	
	unsigned char u8EventSourceFirstIndex;
	unsigned char u8EventSourceLastIndex;
	stcEventSource_t *pstcEventSourceQueue[EVENT_SOURCE_QUEUE_MAX_NUM];
	
	stcEventHandler_t *pstcEventCurrHandler;
} stcEventManage_t;

typedef unsigned int EventSignal_t;

typedef void (*EventFunc_t)(stcEventHandler_t *pstcEventHandler);

/**
  * \}
  */

/** @funcgroup
  * @{
  */

/**
  * @brief  Handle the event that occurred and call it in the main loop
  * @param  None
  * @retval None
  */
void Event_Process(void);


/**
  * @brief  Set the status of the event
  * @param  [pstcEvent] Event handle pointer @ref stcEventHandler_t
  * @param  [enState] Event status @ref enEventStatus_t
  * @retval None
  */
void Event_SetStatus(stcEventHandler_t *pstcEvent, enEventStatus_t enState);


/**
  * @brief  Register an event
  * @param  [pstcEvent] Event handle pointer @ref stcEventHandler_t
  * @param  [pEventSource] Event source @ref stcEventSource_t
  * @param  [u32Signal] Register signal @ref Signal Define
  * @param  [pEventFunc] The callback function executed after the event
  * @retval None
  */
void Event_Register(stcEventHandler_t *pstcEvent, void *pEventSource, 
					EventSignal_t u32Signal, EventFunc_t pEventFunc);


/**
  * @brief  Send a signal to trigger an event
  * @param  [pEventSource] Event source @ref stcEventSource_t
  * @param  [u32TriggerSignal] Signal to depart
  * @param  [pSourceObject] The sending body object of the signal, which is used in the callback function
  * @retval None
  */
static inline void Event_SignalGenerate(stcEventSource_t *pstcEventSource, 
										EventSignal_t u32TriggerSignal,
										void *pSourceObject)
{
	extern stcEventManage_t stcEventManageObject;
	static unsigned char u8LastIndex;
	
	u8LastIndex = stcEventManageObject.u8EventSourceLastIndex;
		
	if (u8LastIndex < EVENT_SOURCE_QUEUE_MAX_NUM)
	{
		pstcEventSource->pObject = pSourceObject;
		pstcEventSource->u32EventTriggerSignal = u32TriggerSignal;
		
		stcEventManageObject.pstcEventSourceQueue[
			u8LastIndex++
		] = pstcEventSource;
		stcEventManageObject.u8EventSourceLastIndex = u8LastIndex;
	}
}

/**
  * \}
  */

#endif
