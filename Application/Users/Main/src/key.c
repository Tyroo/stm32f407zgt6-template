#include "key.h"
#include "delay.h"


//������ʼ������
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, 
	ENABLE);//ʹ�� GPIOA,GPIOE ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	//KEY0 KEY1 KEY2 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ�� GPIOE2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP ��Ӧ���� PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� GPIOA0
} 



// ����ɨ�躯����mode��0��֧����������1֧��������
u8 Key_Scan(u8 mode)
{
	static u8 key_up=1;//�������ɿ���־
	
	if(mode) key_up=1; //֧������ 
	
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)) {
		Delay_Ms(10);//ȥ����
		key_up=0;
		if(KEY0==0) return 1;
		else if(KEY1==0) return 2;
		else if(KEY2==0) return 3;
		else if(WK_UP==1) return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0) {
		key_up=1; 
	}else {}
	return 0;// �ް�������
}


// ������ťKEY0
u8 Key0_Scan() {
	
	static u8 KeyUp = 1;
	static u8 KeyCode = 0;
	
	if (KeyUp && (KEY0 == 0)) {
		Delay_Ms(10);	//ȥ����
		if (KEY0 == 0) {
			KeyUp = 0;
			KeyCode += 1;
			KeyCode %= 2;
		}
		
	} else if (KEY0 == 1) {
		KeyUp = 1;
	}
	return KeyCode;
}
