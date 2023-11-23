#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#define TASK_HANDLER_MAX_NUM	(20)	// 最大任务数量
#define TASK_ASSERT(v)			do {if ((v) == 0) return;} while(0)
	

typedef enum enTaskStatus
{
	enTaskStatus_DELAY,
	enTaskStatus_RUN,
	enTaskStatus_PAUSE,
	enTaskStatus_STOP,
} enTaskStatus_t;


typedef enum enSchedulerStatus
{
	enSchedulerStatus_RUN,
	enSchedulerStatus_WAITSTOP,
	enSchedulerStatus_STOP,
} enSchedulerStatus_t;


typedef struct stcTaskHandler
{
	enTaskStatus_t u8TaskStatus;
	
	uint16_t u16TaskNumber;
	uint32_t u32DelayCountVal;
	uint32_t u32DelayCountCacheVal;
	uint32_t u32TaskExecuteTimePointCountVal;
	uint32_t u32SchedulerPauseTaskDelayCountVal;
	
	struct stcTaskHandler *_pstcTaskHandlerPrev;
	struct stcTaskHandler *_pstcTaskHandlerNext;
	
	void (*pTaskFunction)();
} stcTaskHandler_t;


typedef struct stcTaskManage
{
	uint8_t u8TotalTaskNumber;
	uint8_t u8StopTaskNumber;
	
	enSchedulerStatus_t enSchedulerState;
	
	uint32_t u32TaskTimeBaseCountVal;
	uint32_t u32SchedulerPauseTimePointCountVal;
	
	stcTaskHandler_t *pstcCurrTaskHandler;
	stcTaskHandler_t *pstcSchedulerPausePointTaskHandler;
	stcTaskHandler_t *pstcSchedulerPauseClogsTaskHandler;
	
	stcTaskHandler_t *pstcTaskHandlerTable;
	stcTaskHandler_t *pstcStopTaskHandlerTable[TASK_HANDLER_MAX_NUM];
	
} stcTaskManage_t;


extern stcTaskManage_t stcTaskManageObject;


void Task_Init(void);
void Task_Scheduler(void);
void Task_SchedulerPause(void);
bool Task_SchedulerWaitRun(void);
void Task_Create(stcTaskHandler_t *pstcTask, void (*pTaskFunc)(void));
void Task_SetStatus(stcTaskHandler_t *pstcTask, enTaskStatus_t enState, uint32_t u32DelayCountVal);

#endif
