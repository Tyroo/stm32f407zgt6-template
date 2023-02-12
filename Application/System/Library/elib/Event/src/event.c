/*******************************************************************************
** ���ƣ�event.c
** ���ܣ��¼�����
** �汾��V1.0
** ���ߣ��ż��� 
** ���ڣ�2023��1��4��
*******************************************************************************/

#include "main.h"


stcEventManage_t stcEventManageObject;


// ע��һ���¼�
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
			// ����β�����Ԫ�ز��뵽����ָ��λ����
			pstcTmpEvent->_pstcEventHandlerNext->_pstcEventHandlerPrev = pstcEvent;
			pstcEvent->_pstcEventHandlerNext = pstcTmpEvent->_pstcEventHandlerNext;
			pstcTmpEvent->_pstcEventHandlerNext = pstcEvent;
			pstcEvent->_pstcEventHandlerPrev = pstcTmpEvent;
			pstcTmpEventSource->u8EventTotalNumber++;
			return;
		}
	}
	
	// ���¼�����ͷ�ڵ㴦����һ���¼�
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


// �����¼���״̬
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
	
	// �����һ���¼�����
	// ����¼����ٷ������¼�����ʱ
	if (pstcTmpEventSource->u8EventWaitNumber > 0)
	{
		// Ҫ���ٵ��¼����ڵ�Դ�뵱ǰ�������¼�Դ��ͬʱ
		if (pstcEvent->pstcEventSource == stcEventManageObject.pstcEventCurrHandler->pstcEventSource)
		{
			// �¼�Դ��ͬ������Ҫ���ٵ��¼��뵱ǰ����ִ�е��¼�����ͬһ���¼�
			// ����ݵ�ǰ�¼����ź���ֵ��Ҫ���ٵ��¼����ź���ֵ���Ƚϣ�
			// ���С�ڵ����򽫴��������¼���������1
			if (pstcEvent != stcEventManageObject.pstcEventCurrHandler)
				pstcTmpEventSource->u8EventWaitNumber -= (
					pstcEvent->u32EventRegisterSignal >= 
					stcEventManageObject.pstcEventCurrHandler->u32EventRegisterSignal
				);
			// �¼�Դ��ͬ������Ҫ���ٵ��¼��뵱ǰ����ִ�е��¼���ͬһ���¼�
			// �����һ���¼��л�������ǰ�¼�ָ��ָ����һ���¼�
			else
				stcEventManageObject.pstcEventCurrHandler = pstcEvent->_pstcEventHandlerPrev;
		}
	}
	
	// ����ǰ�¼��Ӹ�Դ���¼��������Ƴ�
	pstcEvent->_pstcEventHandlerPrev->_pstcEventHandlerNext = pstcEvent->_pstcEventHandlerNext;
	pstcEvent->_pstcEventHandlerNext->_pstcEventHandlerPrev = pstcEvent->_pstcEventHandlerPrev;
	
	// ���Ҫ���ٵ��¼��Ǹ��¼�������Դ�еĵ�һ���¼�
	// ����Ҫ�����¼�Դ�е��¼�����ͷ�ڵ�ָ��pstcEventHandlerList����Ϊָ����һ���¼�
	if (pstcEvent == pstcTmpEventSource->pstcEventHandlerList)
	{
		pstcTmpEventSource->pstcEventHandlerList = 
			(pstcEvent != pstcEvent->_pstcEventHandlerNext) ?
			pstcEvent->_pstcEventHandlerNext : NULL;
	}
	
	// �Ͽ��ýڵ�
	pstcEvent->_pstcEventHandlerNext = NULL;
	pstcEvent->_pstcEventHandlerPrev = NULL;
	// �����¼����¼�Դ�е��¼�����1
	pstcTmpEventSource->u8EventTotalNumber--;
}


// �¼�����
void Event_Process(void)
{
	static stcEventHandler_t *pstcCurrEventHandler;
	static stcEventSource_t *pstcCurrEventSource;
	
	/* ����¼�������û���¼� */
	if (stcEventManageObject.u8EventSourceFirstIndex == 
		stcEventManageObject.u8EventSourceLastIndex)
	{
		stcEventManageObject.u8EventSourceFirstIndex = 
			stcEventManageObject.u8EventSourceLastIndex = 0;
		return;
	}
	
	/* ����¼����������¼� */
	// ����ȡ���¼������е��¼�Դ
	pstcCurrEventSource = stcEventManageObject.pstcEventSourceQueue[
		stcEventManageObject.u8EventSourceFirstIndex
	];
	
	stcEventManageObject.pstcEventCurrHandler = pstcCurrEventSource->pstcEventHandlerList;
	
	// ��ȡ�¼�Դ��ע����¼��ź��������
	pstcCurrEventSource->u8EventWaitNumber = pstcCurrEventSource->u8EventTotalNumber;
	
	// �����¼�Դ�е��¼����ҵ��뵱ǰ�������¼��źŶ�Ӧ�������¼���������
	while(pstcCurrEventSource->u8EventWaitNumber > 0)
	{
		pstcCurrEventSource->u8EventWaitNumber--;
		
		pstcCurrEventHandler = stcEventManageObject.pstcEventCurrHandler;
		
		// �����ǰ�������¼��ź�����¼����ź�һ�£���ִ�д��¼����¼��ص�����
		if( pstcCurrEventHandler->u32EventRegisterSignal == 
			pstcCurrEventSource->u32EventTriggerSignal )
		{
			// �����һ���¼��źŵ���ֵ���ڵ�ǰ�¼��źŵ���ֵ�����˳�����
			if( pstcCurrEventHandler->_pstcEventHandlerNext->u32EventRegisterSignal > 
				pstcCurrEventSource->u32EventTriggerSignal )
			{
				pstcCurrEventSource->u8EventWaitNumber = 0;
			}
			
			switch(pstcCurrEventHandler->enEventState)
			{
				// ������¼�״̬Ϊʹ�ܣ�������¼��ص�����
				case enEventStatus_ENABLE:
					pstcCurrEventHandler->pEventFunc(pstcCurrEventHandler);
					break;
				case enEventStatus_DISABLE:
					break;
				// ������¼�״̬ΪAT_ENABLE�����ڱ�������ʹ��
				case enEventStatus_AT_ENABLE:
					pstcCurrEventHandler->enEventState = enEventStatus_ENABLE;
					break;
				default:
					break;
			}
		}
		// ���¼�������ȡ����һ���¼�
		stcEventManageObject.pstcEventCurrHandler = 
			stcEventManageObject.pstcEventCurrHandler->_pstcEventHandlerNext;
	}
	// �¼�����ͷ��������
	stcEventManageObject.u8EventSourceFirstIndex++;
}
