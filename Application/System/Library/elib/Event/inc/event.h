#ifndef __EVENT_H__
#define __EVENT_H__

typedef unsigned int EventSignal_t;

#define EVENT_SOURCE_QUEUE_MAX_NUM			(10)
#define EVENT_ASSERT(v)						do {if ((v) == 0) return;} while(0)

/* >>> Event signal define start */

/* Button signal: bit = 0 */
#define KEY0_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS   << 0)	// key0键长按信号
#define KEY0_BTN_ONCEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_ONCEPRESS   << 0)	// key0键单击信号
#define KEY0_BTN_DOUBLEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_DOUBLEPRESS << 0)	// key0键双击信号

/* Button signal: bit = 2 */
#define KEY1_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS   << 2)	// key1键长按信号
#define KEY1_BTN_ONCEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_ONCEPRESS   << 2)	// key1键单击信号
#define KEY1_BTN_DOUBLEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_DOUBLEPRESS << 2)	// key1键双击信号

/* Button signal: bit = 4 */
#define KEY2_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS   << 4)	// key2键长按信号
#define KEY2_BTN_ONCEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_ONCEPRESS   << 4)	// key2键单击信号
#define KEY2_BTN_DOUBLEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_DOUBLEPRESS << 4)	// key2键双击信号

/* Button signal: bit = 6 */
#define WEUP_BTN_LONGPRESS_SIGNAL			((EventSignal_t)enBtnStatus_LONGPRESS   << 6)	// weup键长按信号
#define WEUP_BTN_ONCEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_ONCEPRESS   << 6)	// weup键单击信号
#define WEUP_BTN_DOUBLEPRESS_SIGNAL			((EventSignal_t)enBtnStatus_DOUBLEPRESS << 6)	// weup键双击信号

/* Custom signal: bit = 8 */
/*
* #define CUSTOM_1_SIGNAL					((EventSignal_t)1 << (bit + 1))
* #define CUSTOM_2_SIGNAL					((EventSignal_t)1 << (bit + 2))
* #define CUSTOM_3_SIGNAL					((EventSignal_t)1 << (bit + 3))
# #define 		...											...
* #define CUSTOM_N_SIGNAL					((EventSignal_t)1 << (bit + N))
*
* TODO: (N >= 0 && N <= 31)
*/

#define CUSTOM_TEST_SIGNAL					((EventSignal_t)1 << 8)						// 自定义测试事件

/* >>> Event signal define end */



typedef enum enEventStatus
{
	enEventStatus_NULL,			// 销毁事件
	enEventStatus_ENABLE,		// 在事件被遍历之前使能
	enEventStatus_AT_ENABLE,	// 在事件被遍历之后使能
	enEventStatus_DISABLE,		// 失能该事件
} enEventStatus_t;

typedef struct stcEventHnadler
{
	EventSignal_t u32EventRegisterSignal;
	enEventStatus_t enEventState;
	
	struct stcEventSource *pstcEventSource;
	
	struct stcEventHnadler *_pstcEventHandlerNext;
	struct stcEventHnadler *_pstcEventHandlerPrev;
	
	void (*pEventFunc)(struct stcEventHnadler *pstcEventHandler);
	
} stcEventHandler_t;


typedef struct stcEventSource
{
	void *pObject;
	
	stcEventHandler_t *pstcEventHandlerList;
	
	uint8_t u8EventTotalNumber; 
	uint8_t u8EventWaitNumber;
	
	EventSignal_t u32EventTriggerSignal;
} stcEventSource_t;


typedef struct stcEventManage
{
	EventSignal_t u32CurrEventSignal;
	EventSignal_t u32PrevEventSignal;
	
	uint8_t u8EventSourceFirstIndex;
	uint8_t u8EventSourceLastIndex;
	stcEventSource_t *pstcEventSourceQueue[EVENT_SOURCE_QUEUE_MAX_NUM];
	
	stcEventHandler_t *pstcEventCurrHandler;
} stcEventManage_t;

typedef void (*EventFunc_t)(stcEventHandler_t *pstcEventHandler);

extern stcEventManage_t stcEventManageObject;

void Event_Process(void);
void Event_SetStatus(stcEventHandler_t *pstcEvent, enEventStatus_t enState);
void Event_Register(stcEventHandler_t *pstcEvent, void *pEventSource, 
					EventSignal_t u32Signal, EventFunc_t pEventFunc);



/*
* @[Event_SignalGenerate] function used example.
* 
* 1. #define CUSTOM_SIGNAL ((EventSignal_t)1 << bit)
*
* 2. stcEventSource_t stcCustomEventSource;
*
* 3. stcEventHandler_t stcCustomEventHandler;
*
* 4. void Custom_EventFunc() { printf("Custom_EventFunc running !"); }
*
* 5. Event_Register(
*		&stcCustomEventHandler,
*		&stcCustomEventSource,
*		CUSTOM_SIGNAL,
*		Custom_EventFunc
*	 );
*
* 6. Event_SignalGenerate(
*		&stcCustomEventSource,
*		CUSTOM_SIGNAL,
*		NULL
*	 );
*
* Run Result:
*
* >> Custom_EventFunc running !
*/
static inline void Event_SignalGenerate(stcEventSource_t *pstcEventSource, 
										EventSignal_t u32TriggerSignal,
										void *pSourceObject)
{
	static uint8_t u8LastIndex;
	
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

#endif
