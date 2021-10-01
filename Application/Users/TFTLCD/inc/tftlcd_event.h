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
	void(*OnFunc)(UINT16 EventCnt, void* pEvenObj);

} CycleEventType;


typedef struct {

	UINT8 EventNumber;
	UINT8 EventStatus;
	void(*OnFunc)(void* pEvenObj);
	void(*OnCbFunc)(void);

} OnceEventType;


void TFTLCD_EventProcess(void);

bool TFTLCD_CycleEventRegister(
	void(*OnFunc)(UINT16 EventCnt, void* EventObj),
	void* EventObj);

bool TFTLCD_OnceEventRegister(
	void(*OnFunc)(void* EventObj),
	void(*OnCbFunc)(void),
	void* EventObj);

static UINT8 TFTLCD_GetCycleEventNumber(void);

static UINT8 TFTLCD_GetOnceEventNumber(void);


/* User event register function */

void PrintCycleCnt(UINT16 Cnt, void* EventObj);

void PrintCycleCnt1(UINT16 Cnt, void* EventObj);


#endif
