/*******************************************************************************
** 名称：task.c
** 功能：任务调度
** 版本：V1.0
** 作者：张嘉良 
** 日期：2023年1月4日
*******************************************************************************/


#include "task.h"


stcTaskManage_t stcTaskManageObject;
stcTaskHandler_t stcIdleTaskHandler;

static void Task_StopTaskDelete(stcTaskHandler_t *pstcTask);
static void Task_IdleTaskFunc(void);


// 任务调度模块初始化
void Task_Init(void)
{
	stcIdleTaskHandler.pTaskFunction = &Task_IdleTaskFunc;
	stcIdleTaskHandler.u16TaskNumber = 0;
	stcIdleTaskHandler.u32DelayCountCacheVal = 999;
	stcIdleTaskHandler.u32DelayCountVal = 999;
	stcIdleTaskHandler.u8TaskStatus = enTaskStatus_DELAY;
	stcIdleTaskHandler._pstcTaskHandlerNext = &stcIdleTaskHandler;
	stcIdleTaskHandler._pstcTaskHandlerPrev = &stcIdleTaskHandler;
	
	stcTaskManageObject.u32TaskTimeBaseCountVal = 0;
	stcTaskManageObject.pstcTaskHandlerTable = &stcIdleTaskHandler;
	stcTaskManageObject.pstcCurrTaskHandler = &stcIdleTaskHandler;
	stcTaskManageObject.u8TotalTaskNumber += 1;
}


// 任务停止处理
static void Task_Stop(stcTaskHandler_t *pstcTask)
{
	if ((pstcTask->u16TaskNumber != stcIdleTaskHandler.u16TaskNumber) && 
		(stcTaskManageObject.enSchedulerState == enSchedulerStatus_RUN))
	{
		pstcTask->u8TaskStatus = enTaskStatus_STOP;
		stcTaskManageObject.pstcStopTaskHandlerTable[stcTaskManageObject.u8StopTaskNumber] = pstcTask;
		stcTaskManageObject.u8StopTaskNumber++;
		stcTaskManageObject.u8TotalTaskNumber--;
	}
}


// 任务设置延时
static void Task_Delay(stcTaskHandler_t *pstcTask, uint32_t u32DelayCountVal)
{
	switch(pstcTask->u8TaskStatus)
	{
		case enTaskStatus_PAUSE:
		case enTaskStatus_RUN:
			pstcTask->u32DelayCountCacheVal = u32DelayCountVal;
			pstcTask->u32DelayCountVal = u32DelayCountVal;
			pstcTask->u32TaskExecuteTimePointCountVal = stcTaskManageObject.u32TaskTimeBaseCountVal;
			pstcTask->u8TaskStatus = enTaskStatus_DELAY;
			break;
		case enTaskStatus_DELAY:
			pstcTask->u32DelayCountCacheVal = u32DelayCountVal;
			break;
		default:
			break;
	}
}


// 任务暂停
static void Task_Pause(stcTaskHandler_t *pstcTask)
{
	if (pstcTask->u16TaskNumber != stcIdleTaskHandler.u16TaskNumber)
		pstcTask->u8TaskStatus = enTaskStatus_PAUSE;
}


// 任务全速运行
static void Task_Run(stcTaskHandler_t *pstcTask)
{
	pstcTask->u8TaskStatus = enTaskStatus_RUN;
	pstcTask->u32DelayCountCacheVal = 0;
	pstcTask->u32DelayCountVal = 0;
}


// 任务状态设置
void Task_SetStatus(stcTaskHandler_t *pstcTask, enTaskStatus_t enState, uint32_t u32DelayCountVal)
{
	TASK_ASSERT(pstcTask != NULL);
	TASK_ASSERT((pstcTask->_pstcTaskHandlerNext != NULL) && 
		(pstcTask->_pstcTaskHandlerPrev != NULL)
	);
	
	switch(enState)
	{
		case enTaskStatus_RUN:
			Task_Run(pstcTask);
			break;
		case enTaskStatus_DELAY:
			Task_Delay(pstcTask, u32DelayCountVal);
			break;
		case enTaskStatus_STOP:
			Task_Stop(pstcTask);
			break;
		case enTaskStatus_PAUSE:
			Task_Pause(pstcTask);
			break;
		default:
			break;
	}
}


// 任务创建
void Task_Create(stcTaskHandler_t *pstcTask, void (*pTaskFunc)(void))
{
	TASK_ASSERT(pstcTask != NULL);
	TASK_ASSERT((pstcTask->_pstcTaskHandlerNext == NULL) && 
		(pstcTask->_pstcTaskHandlerPrev == NULL)
	);
	
	if (stcTaskManageObject.u8TotalTaskNumber < (TASK_HANDLER_MAX_NUM + 1))
	{
		pstcTask->u8TaskStatus = enTaskStatus_PAUSE;
		pstcTask->pTaskFunction = pTaskFunc;
		pstcTask->u16TaskNumber = stcTaskManageObject.pstcTaskHandlerTable->_pstcTaskHandlerPrev->u16TaskNumber + 1;
		pstcTask->_pstcTaskHandlerPrev = stcTaskManageObject.pstcTaskHandlerTable->_pstcTaskHandlerPrev;
		pstcTask->_pstcTaskHandlerNext = stcTaskManageObject.pstcTaskHandlerTable;
		stcTaskManageObject.pstcTaskHandlerTable->_pstcTaskHandlerPrev->_pstcTaskHandlerNext = pstcTask;
		stcTaskManageObject.pstcTaskHandlerTable->_pstcTaskHandlerPrev = pstcTask;
		stcTaskManageObject.u8TotalTaskNumber++;
	}
}


// 暂停调度器
void Task_SchedulerPause(void)
{
	if (stcTaskManageObject.enSchedulerState == enSchedulerStatus_RUN)
	{
		stcTaskManageObject.u32SchedulerPauseTimePointCountVal = stcTaskManageObject.u32TaskTimeBaseCountVal;
		stcTaskManageObject.pstcSchedulerPausePointTaskHandler = stcTaskManageObject.pstcCurrTaskHandler;
		stcTaskManageObject.pstcCurrTaskHandler = &stcIdleTaskHandler;
		stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler = &stcIdleTaskHandler;
		stcTaskManageObject.enSchedulerState = enSchedulerStatus_WAITSTOP;
		Task_SetStatus(&stcIdleTaskHandler, enTaskStatus_RUN, 0);
	}
}


// 恢复调度器的运行（调度器未完全暂停的情况下恢复调度器则会恢复失败）
bool Task_SchedulerWaitRun(void)
{
	bool bRet = false;
	uint8_t u8RecoverTaskNum;
	
	if (stcTaskManageObject.enSchedulerState == enSchedulerStatus_STOP)
	{
		u8RecoverTaskNum = stcTaskManageObject.u8TotalTaskNumber + stcTaskManageObject.u8StopTaskNumber;
		
		for(uint8_t i=0; i<u8RecoverTaskNum; i++)
		{
			stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler = 
				stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->_pstcTaskHandlerNext;
			
			if (stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u8TaskStatus != enTaskStatus_STOP)
			{
				stcTaskManageObject.u8TotalTaskNumber++;
				stcTaskManageObject.u8StopTaskNumber--;
				
				if (stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u8TaskStatus == enTaskStatus_DELAY)
				{
					stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u32TaskExecuteTimePointCountVal = 
						stcTaskManageObject.u32TaskTimeBaseCountVal - 
						stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u32SchedulerPauseTaskDelayCountVal;
				}
			}
		}
		stcTaskManageObject.pstcCurrTaskHandler = stcTaskManageObject.pstcSchedulerPausePointTaskHandler;
		stcTaskManageObject.enSchedulerState = enSchedulerStatus_RUN;
		
		bRet = true;
	}
	
	return bRet;
}


// 任务调度
void Task_Scheduler(void)
{
	static uint32_t u32TaskTimeCountCurrent = 0;
	static uint32_t u32TaskTimeCountDelta = 0;
	
	if (stcTaskManageObject.enSchedulerState != enSchedulerStatus_STOP)
	{
		u32TaskTimeCountCurrent = stcTaskManageObject.u32TaskTimeBaseCountVal;
		
		u32TaskTimeCountDelta = u32TaskTimeCountCurrent - 
			stcTaskManageObject.pstcCurrTaskHandler->u32TaskExecuteTimePointCountVal;
		
		switch(stcTaskManageObject.pstcCurrTaskHandler->u8TaskStatus)
		{
			case enTaskStatus_RUN:
				stcTaskManageObject.pstcCurrTaskHandler->pTaskFunction();
				break;
			case enTaskStatus_DELAY:
				if (u32TaskTimeCountDelta >= stcTaskManageObject.pstcCurrTaskHandler->u32DelayCountVal)
				{
					stcTaskManageObject.pstcCurrTaskHandler->u32TaskExecuteTimePointCountVal = 
						u32TaskTimeCountCurrent;
					stcTaskManageObject.pstcCurrTaskHandler->pTaskFunction();
					stcTaskManageObject.pstcCurrTaskHandler->u32DelayCountVal = 
						stcTaskManageObject.pstcCurrTaskHandler->u32DelayCountCacheVal;
				}
				break;
			default:
				break;
		}
		stcTaskManageObject.pstcCurrTaskHandler = 
			stcTaskManageObject.pstcCurrTaskHandler->_pstcTaskHandlerNext;
	}
}


// 删除一个指定的停止任务
static void Task_StopTaskDelete(stcTaskHandler_t *pstcTask)
{
	TASK_ASSERT(pstcTask != NULL);
	TASK_ASSERT((pstcTask->_pstcTaskHandlerNext != NULL) && 
		(pstcTask->_pstcTaskHandlerPrev != NULL));
	
	pstcTask->u8TaskStatus = enTaskStatus_STOP;
	
	pstcTask->_pstcTaskHandlerPrev->_pstcTaskHandlerNext = pstcTask->_pstcTaskHandlerNext;
	pstcTask->_pstcTaskHandlerNext->_pstcTaskHandlerPrev = pstcTask->_pstcTaskHandlerPrev;
	pstcTask->_pstcTaskHandlerNext = NULL;
	pstcTask->_pstcTaskHandlerPrev = NULL;
}


// 空闲任务
static void Task_IdleTaskFunc(void)
{
	// 调度器正常运行处理
	if (stcTaskManageObject.enSchedulerState != enSchedulerStatus_WAITSTOP)
	{
		// 清理被停止的任务
		if (stcTaskManageObject.u8StopTaskNumber > 0)
		{
			stcTaskManageObject.u8StopTaskNumber--;
			
			Task_StopTaskDelete(stcTaskManageObject.pstcStopTaskHandlerTable[
				stcTaskManageObject.u8StopTaskNumber]
			);
			stcTaskManageObject.pstcStopTaskHandlerTable[
				stcTaskManageObject.u8StopTaskNumber] = NULL;
		}
		
		return;
	}
	
	// 调度器预停止处理
	if (stcTaskManageObject.u8TotalTaskNumber > 1)
	{
		stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler = 
			stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->_pstcTaskHandlerNext;
		
		if (stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u8TaskStatus != enTaskStatus_STOP)
		{
			stcTaskManageObject.u8TotalTaskNumber--;
			stcTaskManageObject.u8StopTaskNumber++;
			
			if (stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u8TaskStatus == enTaskStatus_DELAY)
			{
				stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u32SchedulerPauseTaskDelayCountVal = 
					stcTaskManageObject.u32SchedulerPauseTimePointCountVal - 
					stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler->u32TaskExecuteTimePointCountVal;
			}
		}
	}
	else
	{
		stcTaskManageObject.u8TotalTaskNumber--;
		stcTaskManageObject.u8StopTaskNumber++;
		
		stcTaskManageObject.pstcSchedulerPauseClogsTaskHandler = 
			stcTaskManageObject.pstcSchedulerPausePointTaskHandler;
		
		stcTaskManageObject.enSchedulerState = enSchedulerStatus_STOP;
		
		stcIdleTaskHandler.u32SchedulerPauseTaskDelayCountVal = 0;
		Task_SetStatus(&stcIdleTaskHandler, enTaskStatus_DELAY, 1000);
	}
	
	stcTaskManageObject.pstcCurrTaskHandler = stcTaskManageObject.pstcCurrTaskHandler->_pstcTaskHandlerPrev;
}
