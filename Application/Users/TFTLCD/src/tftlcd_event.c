#include "tftlcd_event.h"


/* Static cycle and once event queue */
static CycleEventType* CycleEventQueue[CYCLE_EVENT_QUEUE_NUM];
static OnceEventType*  OnceEventQueue[ONCE_EVENT_QUEUE_NUM];


void TFTLCD_EventProcess(void)
{
	static UINT16 EventCycleCnt = 1;
	static UINT8  EventIndex;
    static CycleEventType* pCycleEventObj;
    static OnceEventType*  pOnceEventObj;

	for(EventIndex = 0;
		EventIndex<ONCE_EVENT_QUEUE_NUM;
		EventIndex++)
	{
        pOnceEventObj = (*(OnceEventQueue + EventIndex));
        
		if (pOnceEventObj == NULL) continue;
		if ((pOnceEventObj->EventStatus) == EVENT_STATE_ACTIVE)
		{
			pOnceEventObj->OnFunc(pOnceEventObj);
			pOnceEventObj->EventStatus = EVENT_STATE_INACTIVE;
			pOnceEventObj->OnCbFunc();
		}
	}

	for(EventIndex = 0;
		EventIndex<CYCLE_EVENT_QUEUE_NUM;
		EventIndex++)
	{
        pCycleEventObj = (*(CycleEventQueue + EventIndex));
        
		if (pCycleEventObj == NULL) continue;
		if ((pCycleEventObj->EventStatus) == EVENT_STATE_ACTIVE)
		{
            _TFTLCD_CycleBaseOnFunc(EventCycleCnt, pCycleEventObj);
		}
	}

	EventCycleCnt %= EVENT_CYCLE_CNT_VALUE;
    EventCycleCnt++;
}


static void _TFTLCD_CycleBaseOnFunc(UINT16 EventCnt, 
    CycleEventType* _Obj)
{
    if (!_Obj->_EventInitCtn)
        _Obj->_EventInitCtn = EventCnt;
    
	if ((((EVENT_CYCLE_CNT_VALUE + EventCnt - _Obj->_EventInitCtn)% 
        EVENT_CYCLE_CNT_VALUE)% 
        _Obj->EventPeriodCnt) == 0)
    {
        _Obj->OnFunc(_Obj);
    }
}


bool TFTLCD_CycleEventRegister(void(*OnFunc)(void* EventObj),
    UINT16 PeriodCnt, CycleEventType* pEventObj)
{
	UINT8 EventNumber;

	EventNumber = _TFTLCD_GetCycleEQNumber();
	if (!EventNumber) return False;

	pEventObj->EventStatus = EVENT_STATE_INACTIVE;
	pEventObj->OnFunc = OnFunc;
	pEventObj->EventNumber = EventNumber;
    pEventObj->_EventInitCtn = 0;
    pEventObj->EventPeriodCnt = PeriodCnt;

	(*(CycleEventQueue + EventNumber - 1)) = pEventObj;

	return True;
}


bool TFTLCD_OnceEventRegister(void(*OnFunc)(void* EventObj), 
	void(*OnCbFunc)(void), OnceEventType* pEventObj)
{
	UINT8 EventNumber;

	EventNumber = _TFTLCD_GetOnceEQNumber();
	if (!EventNumber) return False;

	((OnceEventType*)(pEventObj))->EventStatus = EVENT_STATE_INACTIVE;
	((OnceEventType*)(pEventObj))->OnFunc = OnFunc;
	((OnceEventType*)(pEventObj))->OnCbFunc = OnCbFunc;
	((OnceEventType*)(pEventObj))->EventNumber = EventNumber;

	(*(OnceEventQueue + EventNumber - 1)) = ((OnceEventType*)(pEventObj));

	return True;
}


void TFTLCD_SetOnceEventPriority(UINT8 GradeNumber, 
    OnceEventType* pEventObj)
{
    OnceEventType* pOnceEvent;
    
    if ((GradeNumber<1) || (GradeNumber>ONCE_EVENT_QUEUE_NUM+1))
        return;
    
    pOnceEvent = (*(OnceEventQueue+GradeNumber));
    
    (*(OnceEventQueue+GradeNumber)) = pEventObj;
    (*(OnceEventQueue+pEventObj->EventNumber)) = pOnceEvent;
    
    pEventObj->EventNumber = GradeNumber;
}


void TFTLCD_SetCycleEventPriority(UINT8 GradeNumber, 
    CycleEventType* pEventObj)
{
    CycleEventType* pCycleEvent;
    
    if ((GradeNumber<1) || (GradeNumber>CYCLE_EVENT_QUEUE_NUM+1))
        return;
    
    pCycleEvent = (*(CycleEventQueue+GradeNumber));
    
    (*(CycleEventQueue+GradeNumber)) = pEventObj;
    (*(CycleEventQueue+pEventObj->EventNumber)) = pCycleEvent;
    
    pEventObj->EventNumber = GradeNumber;
}


static UINT8 _TFTLCD_GetCycleEQNumber(void) {

	UINT8 EventIndex;

	for (EventIndex = CYCLE_EVENT_QUEUE_NUM;
        EventIndex > 0;
		EventIndex--)
	{
		if (CycleEventQueue[EventIndex - 1] == NULL)
			return EventIndex;
		if (CycleEventQueue[EventIndex - 1]->EventNumber == EVENT_STATE_LOGOUT)
			return EventIndex;
	}

	return 0;
}


static UINT8 _TFTLCD_GetOnceEQNumber(void) {

	UINT8 EventIndex;

	for(EventIndex = ONCE_EVENT_QUEUE_NUM;
		EventIndex > 0;
		EventIndex--)
	{
		if (OnceEventQueue[EventIndex - 1] == NULL)
			return EventIndex;
		if (OnceEventQueue[EventIndex - 1]->EventNumber == EVENT_STATE_LOGOUT)
			return EventIndex;
	}
	return 0;
}


/* User event function register */
void PrintCycleCnt(void* EventObj) 
{
    CycleEventType* CycleEventObj;
    CycleEventObj = (CycleEventType*)EventObj;
    
    CycleEventObj->EventStatus = EVENT_STATE_INACTIVE;
    printf("Current cycle value:%d\n", CycleEventObj->EventNumber);
}


void PrintCycleCnt1(void* EventObj)
{
    CycleEventType* CycleEventObj;
    CycleEventObj = (CycleEventType*)EventObj;
    
    CycleEventObj->EventStatus = EVENT_STATE_INACTIVE;
    printf("Current cycle value1:%d\n", CycleEventObj->EventNumber);
}

