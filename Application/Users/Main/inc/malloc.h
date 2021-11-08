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

#define MEM1_MAX_SIZE 100*1024	// 内部内存池大小100KB
#define MEM2_MAX_SIZE 960*1024	// 外部内存池大小960KB
#define MEM3_MAX_SIZE 64*1024	// CCM内存池大小64KB

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
#else

#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN	 0		//内部内存池
#define SRAMEX   1		//外部内存池
#define SRAMCCM  2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)


#define SRAMBANK 	3	//定义支持的SRAM块数.	


//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM1_MAX_SIZE			100*1024  						//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面
#define MEM2_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM2_MAX_SIZE			960 *1024  						//最大管理内存960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM3_MAX_SIZE			60 *1024  						//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小
		 


//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);					//初始化
	u8 (*perused)(u8);		  	    	//内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	 //在mallco.c里面定义

void mymemset(void *s,u8 c,u32 count);	//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存     
void my_mem_init(u8 memx);				//内存管理初始化函数(外/内部调用)
u32 my_mem_malloc(u8 memx,u32 size);	//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);		//内存释放(内部调用)
u8 my_mem_perused(u8 memx);				//获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存(外部调用)

#endif


#endif
