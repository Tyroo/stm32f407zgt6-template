/** �жϷ���-���ȼ�����ģ�飨��֤ͨ���� **/
#include "nvic.h"


// ����NVIC�����жϷ���ȵ�
void Nvic_Init(int Group) {
	/* ��NVIC���� */
	uint32_t GroupDefaultValue;
	
//	/* �����ж�������ƫ�� */
//	SCB->VTOR = (FLASH_BASE | 0x2000);
//	
//	__set_PRIMASK(0);    //�������ж�
	
	switch(Group) {
		
		case 0: GroupDefaultValue = NVIC_PriorityGroup_0;break;
		case 1: GroupDefaultValue = NVIC_PriorityGroup_1;break;
		case 2: GroupDefaultValue = NVIC_PriorityGroup_2;break;
		case 3: GroupDefaultValue = NVIC_PriorityGroup_3;break;
		case 4: GroupDefaultValue = NVIC_PriorityGroup_4;break;
		default: GroupDefaultValue = NVIC_PriorityGroup_0;break;
		
	}
	
	NVIC_PriorityGroupConfig(GroupDefaultValue);
	
	/*  Other  */
	
}


void Nvic_Config(int NvicName, int PrePriority, int SubPriority, int Cmd) {
	
	NVIC_InitTypeDef NVIC_InitStructre;																	// ����һ��NVIC���ýṹ��
	
	NVIC_InitStructre.NVIC_IRQChannel = NvicName;												// ָ���ж�Դ����
	NVIC_InitStructre.NVIC_IRQChannelPreemptionPriority = PrePriority;	// �����жϵ���ռ���ȼ�
	NVIC_InitStructre.NVIC_IRQChannelSubPriority = SubPriority; 				// �����жϵ���Ӧ���ȼ�
	NVIC_InitStructre.NVIC_IRQChannelCmd = Cmd? ENABLE : DISABLE;				// �Ƿ�ʹ���жϣ�1ʹ�ܣ�0ʧ��
	
	NVIC_Init(&NVIC_InitStructre);																			// Ӧ��NVIC���ø���
	
}
