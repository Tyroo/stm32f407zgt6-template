#include "tftlcd_event.h"


/* Golbal cycle and once event queue */
CycleEventType* CycleEventQueue[CYCLE_EVENT_QUEUE_NUM];
OnceEventType*  OnceEventQueue[ONCE_EVENT_QUEUE_NUM];


void TFTLCD_EventProcess(void)
{
	static UINT16 EventCycleCnt = 0;
	UINT8  EventIndex;

	for(EventIndex = 0;
		EventIndex<ONCE_EVENT_QUEUE_NUM;
		EventIndex++)
	{
		if ((*(OnceEventQueue + EventIndex)) == NULL) continue;
		if (((*(OnceEventQueue + EventIndex))->EventStatus) == EVENT_STATE_ACTIVE)
		{
			(*(OnceEventQueue + EventIndex))->OnFunc((*(OnceEventQueue + EventIndex)));
			(*(OnceEventQueue + EventIndex))->EventStatus = EVENT_STATE_INACTIVE;
			(*(OnceEventQueue + EventIndex))->OnCbFunc();
		}
	}

	for(EventIndex = 0;
		EventIndex<CYCLE_EVENT_QUEUE_NUM;
		EventIndex++)
	{
		if ((*(CycleEventQueue + EventIndex)) == NULL) continue;
		if (((*(CycleEventQueue + EventIndex))->EventStatus) == EVENT_STATE_ACTIVE)
		{
			(*(CycleEventQueue + EventIndex))->OnFunc(EventCycleCnt, 
                (*(CycleEventQueue + EventIndex)));
		}
	}

	EventCycleCnt++;
	EventCycleCnt %= EVENT_CYCLE_CNT_VALUE;
}


bool TFTLCD_CycleEventRegister(void(*OnFunc)(UINT16 EventCnt, void* EventObj),
	void* pEventObj)
{
	UINT8 EventNumber;

	EventNumber = TFTLCD_GetCycleEventNumber();
	if (EventNumber == 0xFF) return False;

	((CycleEventType*)(pEventObj))->EventStatus = EVENT_STATE_INACTIVE;
	((CycleEventType*)(pEventObj))->OnFunc = OnFunc;
	((CycleEventType*)(pEventObj))->EventNumber = EventNumber;

	(*(CycleEventQueue + EventNumber)) = ((CycleEventType*)(pEventObj));

	return True;
}


bool TFTLCD_OnceEventRegister(void(*OnFunc)(void* EventObj), 
	void(*OnCbFunc)(void), void* pEventObj)
{
	UINT8 EventNumber;

	EventNumber = TFTLCD_GetOnceEventNumber();
	if (EventNumber == 0xFF) return False;

	((OnceEventType*)(pEventObj))->EventStatus = EVENT_STATE_INACTIVE;
	((OnceEventType*)(pEventObj))->OnFunc = OnFunc;
	((OnceEventType*)(pEventObj))->OnCbFunc = OnCbFunc;
	((OnceEventType*)(pEventObj))->EventNumber = EventNumber;

	(*(OnceEventQueue + EventNumber)) = ((OnceEventType*)(pEventObj));

	return True;
}


static UINT8 TFTLCD_GetCycleEventNumber() {

	UINT8 EventIndex;

	for (EventIndex = CYCLE_EVENT_QUEUE_NUM;
        EventIndex > 0;
		EventIndex--)
	{
		if (CycleEventQueue[EventIndex - 1] == NULL)
			return (EventIndex - 1);
		if (CycleEventQueue[EventIndex - 1]->EventNumber == EVENT_STATE_LOGOUT)
			return (EventIndex - 1);
	}

	return 0xFF;
}


static UINT8 TFTLCD_GetOnceEventNumber() {

	UINT8 EventIndex;

	for(EventIndex = ONCE_EVENT_QUEUE_NUM;
		EventIndex > 0;
		EventIndex--)
	{
		if (OnceEventQueue[EventIndex - 1] == NULL)
			return (EventIndex - 1);
		if (OnceEventQueue[EventIndex - 1]->EventNumber == EVENT_STATE_LOGOUT)
			return (EventIndex - 1);
	}
	return 0xFF;
}


/* User event function register */
void PrintCycleCnt(UINT16 Cnt, void* EventObj) 
{
	static UINT16 CurrentCnt;
	static UINT16 TriggerCnt;

	if (CurrentCnt == 0) CurrentCnt = Cnt;

	if (((EVENT_CYCLE_CNT_VALUE + Cnt - CurrentCnt) % EVENT_CYCLE_CNT_VALUE) > 5000)
	{
		TriggerCnt++;
		if (TriggerCnt>50000)
		{
			TriggerCnt = 0;
			((CycleEventType*)EventObj)->EventStatus = EVENT_STATE_INACTIVE;
			printf("Current cycle value:%d\n", CurrentCnt);
		}
	}	
}


void PrintCycleCnt1(UINT16 Cnt, void* EventObj)
{
	static UINT16 CurrentCnt;
	static UINT16 TriggerCnt;

	if (CurrentCnt == 0) CurrentCnt = Cnt;

	if (((EVENT_CYCLE_CNT_VALUE + Cnt - CurrentCnt) % EVENT_CYCLE_CNT_VALUE) > 50000)
	{
		TriggerCnt++;
		if (TriggerCnt>50000)
		{
			TriggerCnt = 0;
			((CycleEventType*)EventObj)->EventStatus = EVENT_STATE_INACTIVE;
			printf("Current cycle valueAAA:%d\n", CurrentCnt);
		}
	}
}

