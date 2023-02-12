#include "main.h"

/*******************************************************************************
** ���ƣ�keyboard.c
** ���ܣ�����״̬����
** �汾��V1.0
** ���ߣ��ż��� 
** ���ڣ�2023��1��4��
*******************************************************************************/


stcButtonObject_t *pstcCurrButton;
stcEventSource_t stcBtnEventSource;

stcButtonObject_t stcKEY0Button;	// KEY0��
stcButtonObject_t stcKEY1Button;	// KEY1��
stcButtonObject_t stcKEY2Button;	// KEY2��
stcButtonObject_t stcWKUPButton;	// WAKE_UP��

static uint8_t GetKEY0BtnState(void);
static uint8_t GetKEY1BtnState(void);
static uint8_t GetKEY2BtnState(void);
static uint8_t GetWKUPBtnState(void);

stcButtonObject_t *pstcButtonTable[KEYBOARD_BUTTON_NUX] = 
{
	&stcKEY0Button, &stcKEY1Button, &stcKEY2Button,
	&stcWKUPButton
};

pButtonState_t pstcButtonStateFuncTable[KEYBOARD_BUTTON_NUX] = 
{
	&GetKEY0BtnState, &GetKEY1BtnState, &GetKEY2BtnState,
	&GetWKUPBtnState
};


static uint8_t GetKEY0BtnState(void) { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)); }
static uint8_t GetKEY1BtnState(void)  { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)); }
static uint8_t GetKEY2BtnState(void)  { return (uint8_t)(1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)); }
static uint8_t GetWKUPBtnState(void)  { return (uint8_t)GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); }


// ����ģ���ʼ��
void Keyboard_Init(void)
{
	/* GPIO��ʼ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);// ʹ�� GPIOA,GPIOE ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; // KEY0 KEY1 KEY2 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ�� GPIOE2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP ��Ӧ���� PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� GPIOA0
	
	/* ���̶����ʼ�� */
	if (pstcButtonTable[0] == NULL)
		for( ; ; );
	
	pstcCurrButton = pstcButtonTable[0];
	pstcCurrButton->pstcButtonObjectNext = pstcCurrButton;
	pstcCurrButton->pstcButtonObjectPrev = pstcCurrButton;

	for(uint8_t i=0; i<KEYBOARD_BUTTON_NUX; i++)
	{
		Keyboard_AddButton(
			pstcButtonTable[i], 
			pstcButtonStateFuncTable[i]
		);
		pstcButtonTable[i]->u8BtnNumber = i;
	}
}


void Keyboard_AddButton(stcButtonObject_t *pstcButton, pButtonState_t pButtonStateFunc)
{
	stcButtonObject_t *pstcFirstButton = pstcButtonTable[0];
	
	if (pstcButton != NULL)
	{
		if (pstcButton != pstcFirstButton)
		{
			pstcButton->pstcButtonObjectPrev = pstcFirstButton->pstcButtonObjectPrev;
			pstcButton->pstcButtonObjectNext = pstcFirstButton;
			pstcFirstButton->pstcButtonObjectPrev->pstcButtonObjectNext = pstcButton;
			pstcFirstButton->pstcButtonObjectPrev = pstcButton;
		}
		pstcButton->GetKeyState = pButtonStateFunc;
		Keyboard_SetBtnDecideTime(pstcButton, 2000, 50);
	}
}


void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, 
		uint16_t u16LongPressTime, uint16_t u16MultiPressTime)
{
	pstcButton->u16LongPressDecideTime = u16LongPressTime;
	pstcButton->u16MultiPressIntervalTime = u16MultiPressTime;
}


static void Keyboard_Debounce(stcButtonObject_t *pstcBtnObject)
{
	uint8_t u8CurrBtnState = 0;
	
	u8CurrBtnState = pstcBtnObject->GetKeyState();
	
	// ����ť״̬�����仯ʱ
	if ((u8CurrBtnState != pstcBtnObject->u8BtnStateDebounceBefore)
		 && (pstcBtnObject->bBtnDebounceFlg == false))
	{
		// ��¼�����仯ʱ��ʱ��
		pstcBtnObject->u16BtnDebounceCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
		// ��ť������־����Ϊtrue
		pstcBtnObject->bBtnDebounceFlg = true;
	}
	// ִ�а�ť����
	else if (pstcBtnObject->bBtnDebounceFlg)
	{
		// ����һ���İ�ť����ʱ����ٻ�ȡ״̬��ʵ�ְ�ť����
		if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
			 pstcBtnObject->u16BtnDebounceCnt) > KEYBOARD_DEBOUNCE_TIME)
		{
			pstcBtnObject->bBtnDebounceFlg = false;
			pstcBtnObject->u8BtnStateDebounceAfter = u8CurrBtnState;
		}
	}
}


// ���̰���ɨ��ģ��
static void Keyboard_Scan(stcButtonObject_t *pstcBtnObject)
{
	uint8_t u8BtnStateValue = 0;
	
	u8BtnStateValue = pstcBtnObject->u8BtnStateDebounceBefore + (pstcBtnObject->u8BtnStateDebounceAfter << 1);
	pstcBtnObject->u8BtnStateDebounceBefore = pstcBtnObject->u8BtnStateDebounceAfter;
	
	switch(u8BtnStateValue)
	{
		// �����ɿ���
		case 0:
			if (pstcBtnObject->s8BtnLoosenCnt < 1)
			{
				pstcBtnObject->enBtnState = enBtnStatus_LOOSEN;
			}
			else
			{
				if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
					 pstcBtnObject->u16MultiPressIntervalCnt) > pstcBtnObject->u16MultiPressIntervalTime)
				{
					pstcBtnObject->enBtnState = (enButtonStatus_t)(pstcBtnObject->s8BtnLoosenCnt + 1);
					pstcBtnObject->s8BtnLoosenCnt = 0;
				}
			}
			break;
		// �������ɿ�ʱ
		case 1:
			pstcBtnObject->s8BtnLoosenCnt++;
			if (pstcBtnObject->s8BtnLoosenCnt == 1)
				pstcBtnObject->u16MultiPressIntervalCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			break;
		// ��ť������ʱ
		case 2:
			pstcBtnObject->u16LongPressDecideCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			break;
		// ��ť�����º�
		case 3:
			if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
				 pstcBtnObject->u16LongPressDecideCnt) > pstcBtnObject->u16LongPressDecideTime)
			{
				pstcBtnObject->s8BtnLoosenCnt = - 1;
				pstcBtnObject->enBtnState = enBtnStatus_LONGPRESS;
			}
			break;
		default:
			break;
	}
}


// ��ť�źŷ���
static void Keyboard_SignalSend(stcEventSource_t *pstcEventSource)
{
	static EventSignal_t u32CurrBtnSignal = 0;
	
	// �����ť״̬������Ͷ�Ӧ���¼��ź�
	if (pstcCurrButton->enBtnState != pstcCurrButton->enBtnStatePrev)
	{
		pstcCurrButton->enBtnStatePrev = pstcCurrButton->enBtnState;
		
		u32CurrBtnSignal &= ~(((EventSignal_t)3) << (pstcCurrButton->u8BtnNumber * 2));
		u32CurrBtnSignal |= ((EventSignal_t)pstcCurrButton->enBtnState << (pstcCurrButton->u8BtnNumber * 2));
		
		Event_SignalGenerate(
			pstcEventSource, 
			u32CurrBtnSignal, 
			pstcCurrButton
		);
	}
}


// ����״̬����
void Keyboard_Process(void)
{
	// ������������
	Keyboard_Debounce(pstcCurrButton);
	
	// ����ɨ�账��
	Keyboard_Scan(pstcCurrButton);
	
	// ��ť�źŷ���
	Keyboard_SignalSend(&stcBtnEventSource);
	
	pstcCurrButton = pstcCurrButton->pstcButtonObjectNext;
}
