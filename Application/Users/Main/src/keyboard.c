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
	
	/* ��������ģ���ʼ�� */
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
		Keyboard_SetBtnDecideTime(pstcButton, 2000, 800);
	}
}


void Keyboard_SetBtnDecideTime(stcButtonObject_t *pstcButton, 
		uint16_t u16LongPressTime, uint16_t u16MultiPressTime)
{
	pstcButton->u16LongPressDecideTime = u16LongPressTime;
	pstcButton->u16MultiPressIntervalTime = u16MultiPressTime;
}


static void Keyboard_Debounce(void)
{
	uint8_t u8CurrBtnState = 0;
	
	u8CurrBtnState = pstcCurrButton->GetKeyState();
	
	// ����ť״̬�����仯ʱ
	if ((u8CurrBtnState != pstcCurrButton->u8BtnStateDebounceBefore)
		 && (pstcCurrButton->bBtnDebounceFlg == false))
	{
		// ��¼�����仯ʱ��ʱ��
		pstcCurrButton->u16BtnDebounceCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
		// ��ť������־����Ϊtrue
		pstcCurrButton->bBtnDebounceFlg = true;
	}
	// ִ�а�ť����
	else if (pstcCurrButton->bBtnDebounceFlg)
	{
		// ����һ���İ�ť����ʱ����ٻ�ȡ״̬��ʵ�ְ�ť����
		if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
			 pstcCurrButton->u16BtnDebounceCnt) > KEYBOARD_DEBOUNCE_TIME)
		{
			pstcCurrButton->bBtnDebounceFlg = false;
			pstcCurrButton->u8BtnStateDebounceAfter = u8CurrBtnState;
		}
	}
}


// ���̰���ɨ��ģ��
static void Keyboard_Scan(void)
{
	uint8_t u8BtnStateValue = 0;
	
	u8BtnStateValue = pstcCurrButton->u8BtnStateDebounceBefore + (pstcCurrButton->u8BtnStateDebounceAfter << 1);
	pstcCurrButton->u8BtnStateDebounceBefore = pstcCurrButton->u8BtnStateDebounceAfter;
	
	switch(u8BtnStateValue)
	{
		// �����ɿ���
		case 0:
			if (pstcCurrButton->enBtnState != enBtnStatus_LOOSEN)
			{
				if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
					 pstcCurrButton->u16MultiPressIntervalCnt) > pstcCurrButton->u16MultiPressIntervalTime)
				{
					pstcCurrButton->enBtnState = enBtnStatus_LOOSEN;
					pstcCurrButton->s8BtnLoosenCnt = 0;
				}
			}
			break;
		// �������ɿ�ʱ
		case 1:
			pstcCurrButton->enBtnState = (enButtonStatus_t)(enBtnStatus_ONCEPRESS_UP + pstcCurrButton->s8BtnLoosenCnt);
			pstcCurrButton->u16MultiPressIntervalCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			pstcCurrButton->s8BtnLoosenCnt = (pstcCurrButton->s8BtnLoosenCnt + 1) % 2;
			break;
		// ��ť������ʱ
		case 2:
			pstcCurrButton->enBtnState = (enButtonStatus_t)(enBtnStatus_ONCEPRESS_DW + pstcCurrButton->s8BtnLoosenCnt);
			pstcCurrButton->u16LongPressDecideCnt = stcTaskManageObject.u32TaskTimeBaseCountVal;
			break;
		// ��ť�����º�
		case 3:
			if (pstcCurrButton->enBtnState != enBtnStatus_LONGPRESS)
			{
				if ((stcTaskManageObject.u32TaskTimeBaseCountVal - 
					 pstcCurrButton->u16LongPressDecideCnt) > pstcCurrButton->u16LongPressDecideTime)
				{
					pstcCurrButton->enBtnState = enBtnStatus_LONGPRESS;
				}
			}
			break;
		default:
			break;
	}
}


// ��ť�źŷ���
static void Keyboard_SignalSend(void)
{
	static EventSignal_t u32CurrBtnSignal = 0;
	
	// �����ť״̬������Ͷ�Ӧ���¼��ź�
	if (pstcCurrButton->enBtnState != pstcCurrButton->enBtnStatePrev)
	{
		u32CurrBtnSignal &= ~(((EventSignal_t)7) << (pstcCurrButton->u8BtnNumber * 3));
		u32CurrBtnSignal |= ((EventSignal_t)pstcCurrButton->enBtnState << (pstcCurrButton->u8BtnNumber * 3));
		
		Event_SignalGenerate(
			&stcBtnEventSource, 
			u32CurrBtnSignal, 
			pstcCurrButton
		);
	}
	else
	{
		pstcCurrButton->pstcButtonObjectPrev->enBtnStatePrev = 
			pstcCurrButton->pstcButtonObjectPrev->enBtnState;
	}
}


// ����״̬����
void Keyboard_Process(void)
{
	// ������������
	Keyboard_Debounce();
	
	// ����ɨ�账��
	Keyboard_Scan();
	
	// ��ť�źŷ���
	Keyboard_SignalSend();
	
	// �л�����һ������
	pstcCurrButton = pstcCurrButton->pstcButtonObjectNext;
}
