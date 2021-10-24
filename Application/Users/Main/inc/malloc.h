#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdio.h>
#include "stm32f4xx.h"

#define MEM_TYPE_SIZE 3

#define MEM_TYPE1 1
#define MEM_TYPE2 2
#define MEM_TYPE3 3

#define MEM1_MAX_SIZE 100*1024	// �ڲ��ڴ�ش�С100KB
#define MEM2_MAX_SIZE 960*1024	// �ⲿ�ڴ�ش�С960KB
#define MEM3_MAX_SIZE 64*1024		// CCM�ڴ�ش�С64KB

#define MEM1_BANK_SIZE 32	// �ڲ��ڴ������С32Byte
#define MEM2_BANK_SIZE 32	// �ⲿ�ڴ������С32Byte
#define MEM3_BANK_SIZE 32	// CCM�ڴ������С32Byte

#define MEM1_TABLE_SIZE MEM1_MAX_SIZE/MEM1_BANK_SIZE	// �ڲ��ڴ������С32Byte
#define MEM2_TABLE_SIZE MEM2_MAX_SIZE/MEM2_BANK_SIZE	// �ⲿ�ڴ������С32Byte
#define MEM3_TABLE_SIZE MEM3_MAX_SIZE/MEM3_BANK_SIZE	// CCM�ڴ������С32Byte


#define MALLOC_MANAGE_DEFAULT { Malloc_Init,\
								Malloc_Perused,\
								{ Memory_Pond_Int, Memory_Pond_Ext, Memory_Pond_Ccm },\
								{ Memory_Table_Int, Memory_Table_Ext, Memory_Table_Ccm },\
								{ 0, 0, 0 }}


// �ڴ����ṹ��
typedef struct {
	
	void (*Init)(uint8_t); 						//��ʼ������
	uint8_t (*Perused)(uint8_t); 				//�ڴ�ʹ����
	uint8_t* MallocPondArr[MEM_TYPE_SIZE];  	//�ڴ�ع���SRAMBANK��������ڴ�
	uint16_t* MallocTableArr[MEM_TYPE_SIZE];	//�ڴ����״̬��
	uint8_t MallocReadyArr[MEM_TYPE_SIZE]; 	    //�ڴ�����Ƿ����
	
} Malloc_Manage;


extern Malloc_Manage MallocManage;



void Malloc_Init(uint8_t MemType);

void Malloc_Free(uint8_t MemType, void* Ptr);

void Malloc_Copy(void* SrcAddr, void* DesAddr, uint16_t Len);

void Malloc_Set(void* DesAddr, uint16_t Value, uint16_t Len);

void* Malloc_Apply(uint8_t MemType, uint16_t Size);

void* Malloc_Reset(uint8_t MemType, void* Ptr, uint16_t Size);

static uint8_t _Malloc_Free(uint8_t MemType, uint32_t Offset);

static void* _Malloc_Apply(uint8_t MemType, uint16_t Size);

uint8_t Malloc_Perused(uint8_t MemType);


#endif
