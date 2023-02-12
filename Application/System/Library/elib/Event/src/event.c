/*******************************************************************************
** 名称：event.c
** 功能：事件处理
** 版本：V1.0
** 作者：张嘉良 
** 日期：2023年1月4日
*******************************************************************************/

#include "main.h"


stcEventManage_t stcEventManageObject;


// 注册一个事件
void Event_Register(stcEventHandler_t *pstcEvent,
					void *pstcEventSource, 
					EventSignal_t u32RegisterSignal, 
					EventFunc_t pEventFunc)
{
	EVENT_ASSERT(pEventFunc != NULL);
	EVENT_ASSERT(pstcEventSource != NULL);
	
	stcEventSource_t *pstcTmpEventSource = pstcEventSource;
	stcEventHandler_t *pstcTmpEvent;
	
	pstcEvent->pEventFunc = pEventFunc;
	pstcEvent->pstcEventSource = pstcEventSource;
	pstcEvent->enEventState = enEventStatus_ENABLE;
	pstcEvent->u32EventRegisterSignal = u32RegisterSignal;
	
	pstcTmpEvent = pstcTmpEventSource->pstcEventHandlerList;
	
	for(uint8_t i = pstcTmpEventSource->u8EventTotalNumber; i > 0; i--)
	{
		pstcTmpEvent = pstcTmpEvent->_pstcEventHandlerPrev;
		
		if (u32RegisterSignal >= pstcTmpEvent->u32EventRegisterSignal)
		{
			// 将表尾插入的元素插入到链表指定位置上
			pstcTmpEvent->_pstcEventHandlerNext->_pstcEventHandlerPrev = pstcEvent;
			pstcEvent->_pstcEventHandlerNext = pstcTmpEvent->_pstcEventHandlerNext;
			pstcTmpEvent->_pstcEventHandlerNext = pstcEvent;
			pstcEvent->_pstcEventHandlerPrev = pstcTmpEvent;
			pstcTmpEventSource->u8EventTotalNumber++;
			return;
		}
	}
	
	// 在事件链表头节点处插入一个事件
	if (pstcTmpEvent != NULL)
	{
		pstcEvent->_pstcEventHandlerNext = pstcTmpEvent;
		pstcEvent->_pstcEventHandlerPrev = pstcTmpEvent->_pstcEventHandlerPrev;
		pstcTmpEvent->_pstcEventHandlerPrev->_pstcEventHandlerNext = pstcEvent;
		pstcTmpEvent->_pstcEventHandlerPrev = pstcEvent;
	}
	else
	{
		pstcEvent->_pstcEventHandlerNext = pstcEvent;
		pstcEvent->_pstcEventHandlerPrev = pstcEvent;
	}
	
	pstcTmpEventSource->pstcEventHandlerList = pstcEvent;
	pstcTmpEventSource->u8EventTotalNumber++;
}


// 设置事件的状态
void Event_SetStatus(stcEventHandler_t *pstcEvent, enEventStatus_t enState)
{
	EVENT_ASSERT(pstcEvent != NULL);
	EVENT_ASSERT(pstcEvent->enEventState != enEventStatus_NULL);
	
	static stcEventSource_t *pstcTmpEventSource;
	
	if (enState != enEventStatus_NULL)
	{
		pstcEvent->enEventState = enState;
		return;
	}
	
	pstcTmpEventSource = pstcEvent->pstcEventSource;
	
	EVENT_ASSERT(pstcTmpEventSource != NULL);
	EVENT_ASSERT(pstcTmpEventSource->pstcEventHandlerList != NULL);
	
	// 如果将一个事件销毁
	// 如果事件销毁发生在事件遍历时
	if (pstcTmpEventSource->u8EventWaitNumber > 0)
	{
		// 要销毁的事件所在的源与当前遍历的事件源相同时
		if (pstcEvent->pstcEventSource == stcEventManageObject.pstcEventCurrHandler->pstcEventSource)
		{
			// 事件源相同，但是要销毁的事件与当前正在执行的事件不是同一个事件
			// 则根据当前事件的信号数值与要销毁的事件的信号数值作比较，
			// 如果小于等于则将待遍历的事件计数器减1
			if (pstcEvent != stcEventManageObject.pstcEventCurrHandler)
				pstcTmpEventSource->u8EventWaitNumber -= (
					pstcEvent->u32EventRegisterSignal >= 
					stcEventManageObject.pstcEventCurrHandler->u32EventRegisterSignal
				);
			// 事件源相同，但是要销毁的事件与当前正在执行的事件是同一个事件
			// 则进行一次事件切换，将当前事件指针指向上一个事件
			else
				stcEventManageObject.pstcEventCurrHandler = pstcEvent->_pstcEventHandlerPrev;
		}
	}
	
	// 将当前事件从该源的事件链表中移除
	pstcEvent->_pstcEventHandlerPrev->_pstcEventHandlerNext = pstcEvent->_pstcEventHandlerNext;
	pstcEvent->_pstcEventHandlerNext->_pstcEventHandlerPrev = pstcEvent->_pstcEventHandlerPrev;
	
	// 如果要销毁的事件是该事件所处的源中的第一个事件
	// 则需要将该事件源中的事件链表头节点指针pstcEventHandlerList更改为指向下一个事件
	if (pstcEvent == pstcTmpEventSource->pstcEventHandlerList)
	{
		pstcTmpEventSource->pstcEventHandlerList = 
			(pstcEvent != pstcEvent->_pstcEventHandlerNext) ?
			pstcEvent->_pstcEventHandlerNext : NULL;
	}
	
	// 断开该节点
	pstcEvent->_pstcEventHandlerNext = NULL;
	pstcEvent->_pstcEventHandlerPrev = NULL;
	// 将此事件的事件源中的事件数减1
	pstcTmpEventSource->u8EventTotalNumber--;
}


// 事件处理
void Event_Process(void)
{
	static stcEventHandler_t *pstcCurrEventHandler;
	static stcEventSource_t *pstcCurrEventSource;
	
	/* 如果事件队列中没有事件 */
	if (stcEventManageObject.u8EventSourceFirstIndex == 
		stcEventManageObject.u8EventSourceLastIndex)
	{
		stcEventManageObject.u8EventSourceFirstIndex = 
			stcEventManageObject.u8EventSourceLastIndex = 0;
		return;
	}
	
	/* 如果事件队列中有事件 */
	// 依次取出事件队列中的事件源
	pstcCurrEventSource = stcEventManageObject.pstcEventSourceQueue[
		stcEventManageObject.u8EventSourceFirstIndex
	];
	
	stcEventManageObject.pstcEventCurrHandler = pstcCurrEventSource->pstcEventHandlerList;
	
	// 获取事件源中注册的事件信号种类个数
	pstcCurrEventSource->u8EventWaitNumber = pstcCurrEventSource->u8EventTotalNumber;
	
	// 遍历事件源中的事件，找到与当前发生的事件信号对应的所有事件，并处理
	while(pstcCurrEventSource->u8EventWaitNumber > 0)
	{
		pstcCurrEventSource->u8EventWaitNumber--;
		
		pstcCurrEventHandler = stcEventManageObject.pstcEventCurrHandler;
		
		// 如果当前发生的事件信号与此事件的信号一致，则执行此事件的事件回调函数
		if( pstcCurrEventHandler->u32EventRegisterSignal == 
			pstcCurrEventSource->u32EventTriggerSignal )
		{
			// 如果下一个事件信号的数值大于当前事件信号的数值，则退出遍历
			if( pstcCurrEventHandler->_pstcEventHandlerNext->u32EventRegisterSignal > 
				pstcCurrEventSource->u32EventTriggerSignal )
			{
				pstcCurrEventSource->u8EventWaitNumber = 0;
			}
			
			switch(pstcCurrEventHandler->enEventState)
			{
				// 如果该事件状态为使能，则调用事件回调函数
				case enEventStatus_ENABLE:
					pstcCurrEventHandler->pEventFunc(pstcCurrEventHandler);
					break;
				case enEventStatus_DISABLE:
					break;
				// 如果该事件状态为AT_ENABLE，则在被遍历后使能
				case enEventStatus_AT_ENABLE:
					pstcCurrEventHandler->enEventState = enEventStatus_ENABLE;
					break;
				default:
					break;
			}
		}
		// 从事件链表中取出下一个事件
		stcEventManageObject.pstcEventCurrHandler = 
			stcEventManageObject.pstcEventCurrHandler->_pstcEventHandlerNext;
	}
	// 事件队列头索引自增
	stcEventManageObject.u8EventSourceFirstIndex++;
}
