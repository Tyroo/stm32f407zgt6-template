#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdio.h>
#include "stm32f4xx.h"

#define MEM_TYPE_SIZE 3

#define MEM_TYPE1 1
#define MEM_TYPE2 2
#define MEM_TYPE3 3

#define MEM1_MAX_SIZE 100*1024	// 内部内存池大小100KB
#define MEM2_MAX_SIZE 960*1024	// 外部内存池大小960KB
#define MEM3_MAX_SIZE 64*1024		// CCM内存池大小64KB

#define MEM1_BANK_SIZE 32	// 内部内存管理块大小32Byte
#define MEM2_BANK_SIZE 32	// 外部内存管理块大小32Byte
#define MEM3_BANK_SIZE 32	// CCM内存管理块大小32Byte

#define MEM1_TABLE_SIZE MEM1_MAX_SIZE/MEM1_BANK_SIZE	// 内部内存管理块大小32Byte
#define MEM2_TABLE_SIZE MEM2_MAX_SIZE/MEM2_BANK_SIZE	// 外部内存管理块大小32Byte
#define MEM3_TABLE_SIZE MEM3_MAX_SIZE/MEM3_BANK_SIZE	// CCM内存管理块大小32Byte


#define MALLOC_MANAGE_DEFAULT { Malloc_Init,\
								Malloc_Perused,\
								{ Memory_Pond_Int, Memory_Pond_Ext, Memory_Pond_Ccm },\
								{ Memory_Table_Int, Memory_Table_Ext, Memory_Table_Ccm },\
								{ 0, 0, 0 }}


// 内存管理结构体
typedef struct {
	
	void (*Init)(uint8_t); 						//初始化方法
	uint8_t (*Perused)(uint8_t); 				//内存使用率
	uint8_t* MallocPondArr[MEM_TYPE_SIZE];  	//内存池管理SRAMBANK个区域的内存
	uint16_t* MallocTableArr[MEM_TYPE_SIZE];	//内存管理状态表
	uint8_t MallocReadyArr[MEM_TYPE_SIZE]; 	    //内存管理是否就绪
	
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
