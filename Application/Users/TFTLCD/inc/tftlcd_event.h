#ifndef __TFTLCD_EVENT_H__
#define __TFTLCD_EVENT_H__


#include <stdio.h>
#include "spi.h"


#ifndef CYCLE_EVENT_QUEUE_NUM
#define CYCLE_EVENT_QUEUE_NUM 4
#endif

#ifndef ONCE_EVENT_QUEUE_NUM
#define ONCE_EVENT_QUEUE_NUM 5
#endif


#ifndef CYCLE_EVENT_QUEUE_MAX_NUM
#define CYCLE_EVENT_QUEUE_MAX_NUM 10
#endif

#ifndef ONCE_EVENT_QUEUE_MAX_NUM
#define ONCE_EVENT_QUEUE_MAX_NUM 20
#endif

#if CYCLE_EVENT_QUEUE_NUM > CYCLE_EVENT_QUEUE_MAX_NUM
#error "The number of cycle events exceeded the upper limit. Procedure"
#endif

#if ONCE_EVENT_QUEUE_NUM > ONCE_EVENT_QUEUE_MAX_NUM
#error "The number of once events exceeded the upper limit. Procedure"
#endif


#define EVENT_CYCLE_CNT_VALUE 0xFFFF


#define EVENT_STATE_INACTIVE     0
#define EVENT_STATE_ACTIVE       1
#define EVENT_STATE_LOGOUT       2


typedef unsigned char          UINT8;
typedef unsigned int           UINT16;
typedef unsigned long          UINT32;


typedef struct {

	UINT8 EventNumber;
	UINT8 EventStatus;
    UINT16 _EventInitCnt;   // Don't change
	UINT16 _EventCompCnt;
    UINT16 EventPeriodCnt;  // Set range: [0,65534]
    
	void(*OnFunc)(void* pEvenObj);

} CycleEventType;


typedef struct {

	UINT8  EventNumber;
	UINT8  EventStatus;

	void(*OnFunc)(void* pEvenObj);
	void(*OnCbFunc)(void);

} OnceEventType;


void TFTLCD_EventProcess(void);

bool TFTLCD_CycleEventRegister(
    void(*OnFunc)(void* EventObj),
    UINT16 PeriodCnt,   // Set range:[0,65534]
    CycleEventType* pEventObj);

bool TFTLCD_OnceEventRegister(
	void(*OnFunc)(void* EventObj),
	void(*OnCbFunc)(void),
	OnceEventType* EventObj);
    
void TFTLCD_SetCycleEventPriority(UINT8 GradeNumber, 
    CycleEventType* pEventObj);

void TFTLCD_SetOnceEventPriority(UINT8 GradeNumber, 
    OnceEventType* pEventObj);

static UINT8 _TFTLCD_GetCycleEQNumber(void);

static UINT8 _TFTLCD_GetOnceEQNumber(void);
    
static void _TFTLCD_CycleBaseOnFunc(UINT16 EventCnt, 
    CycleEventType* _Obj);


/* User event register function */

void PrintCycleCnt(void* EventObj);

void PrintCycleCnt1(void* EventObj);


#endif
