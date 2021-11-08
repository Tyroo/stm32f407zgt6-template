#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdio.h>
#include "stm32f4xx.h"

#define USE_NEW_MALLOC_MOD   0

#if USE_NEW_MALLOC_MOD
#define MEM_TYPE_SIZE 3

#define MEM_TYPE1 1
#define MEM_TYPE2 2
#define MEM_TYPE3 3

#define MEM1_MAX_SIZE 100*1024	// �ڲ��ڴ�ش�С100KB
#define MEM2_MAX_SIZE 960*1024	// �ⲿ�ڴ�ش�С960KB
#define MEM3_MAX_SIZE 64*1024	// CCM�ڴ�ش�С64KB

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
#else

#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��
#define SRAMCCM  2		//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)


#define SRAMBANK 	3	//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			100*1024  						//�������ڴ� 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
#define MEM2_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE			960 *1024  						//�������ڴ�960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
		 
//mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM3_MAX_SIZE			60 *1024  						//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//�ڴ���С
		 


//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);					//��ʼ��
	u8 (*perused)(u8);		  	    	//�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void my_mem_init(u8 memx);				//�ڴ�����ʼ������(��/�ڲ�����)
u32 my_mem_malloc(u8 memx,u32 size);	//�ڴ����(�ڲ�����)
u8 my_mem_free(u8 memx,u32 offset);		//�ڴ��ͷ�(�ڲ�����)
u8 my_mem_perused(u8 memx);				//����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)

#endif


#endif
