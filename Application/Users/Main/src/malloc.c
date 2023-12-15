#include "malloc.h"

#if USE_NEW_MALLOC_MOD
// 内存池，指定32字节位对齐
__align(32) uint8_t Memory_Pond_Int[MEM1_MAX_SIZE];									// 内部内存块
__align(32) uint8_t Memory_Pond_Ext[MEM2_MAX_SIZE] __attribute__((at(0x68000000)));	// 外部SRAM内存
__align(32) uint8_t Memory_Pond_Ccm[MEM3_MAX_SIZE] __attribute__((at(0x10000000)));	// 内部CCM内存

// 内存管理表
uint16_t Memory_Table_Int[MEM1_TABLE_SIZE]; 																									// 内部SRAM内存管理表
uint16_t Memory_Table_Ext[MEM2_TABLE_SIZE] __attribute__((at(0X68000000 + MEM2_MAX_SIZE)));	// 外部SRAM内存管理表
uint16_t Memory_Table_Ccm[MEM3_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	// 内部CCM内存管理表

// 内存大小数组
const uint32_t MallocTableSizeArr[MEM_TYPE_SIZE] = {MEM1_TABLE_SIZE,
MEM2_TABLE_SIZE, MEM3_TABLE_SIZE}; //内存表大小
const uint32_t MallocPondSizeArr[MEM_TYPE_SIZE] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE,
MEM3_MAX_SIZE}; //内存分块大小
const uint32_t MallocBankSizeArr[MEM_TYPE_SIZE] = {MEM1_BANK_SIZE,MEM2_BANK_SIZE,
MEM3_BANK_SIZE};

// 内存管理结构体定义
Malloc_Manage MallocManage = MALLOC_MANAGE_DEFAULT;


// 内存分配模块初始化
void Malloc_Init(uint8_t MemType) {
	Malloc_Set(MallocManage.MallocPondArr[MemType], 0, MallocPondSizeArr[MemType]);
	Malloc_Set(MallocManage.MallocTableArr[MemType], 0, MallocTableSizeArr[MemType]);
	MallocManage.MallocReadyArr[MemType] = 1;
}


// 将指定内存地址的内容分配给另一地址
void Malloc_Copy(void* SrcAddr, void* DesAddr, uint16_t Len) {
	
	uint8_t* DesAddrTemp = DesAddr;
	uint8_t* SrcAddrTemp = SrcAddr; 
	
	while(Len--)
		*DesAddrTemp++ = *SrcAddrTemp++; 
}


// 设置一段地址内存的值位给定值
void Malloc_Set(void* DesAddr, uint16_t Value, uint16_t Len) {

	uint8_t* DesAddrTemp = DesAddr;
	
	while(Len--) {
		*DesAddrTemp++ = Value;
	}
}


// 释放指定内存（内部调用）
static uint8_t _Malloc_Free(uint8_t MemType, uint32_t Offset) {
	
	uint32_t BankOffset;
	uint32_t BankIndex;
	uint32_t FreeBankNum;
	
	// 未初始化则先初始化
	if (!MallocManage.MallocReadyArr[MemType]) {
		MallocManage.Init(MemType);
		return 2;		// 未初始化
	}
		
	if (Offset<MallocPondSizeArr[MemType]) {
		// 获取要释放的起始块号
		BankOffset = Offset / MallocBankSizeArr[MemType];
		// 获取要释放的块数目
		FreeBankNum = MallocManage.MallocPondArr[MemType][BankOffset];
		
		// 对相应的块管理表清零
		for(BankIndex=0;BankIndex<FreeBankNum;BankIndex++) {
			MallocManage.MallocTableArr[MemType][FreeBankNum+BankIndex] = 0;
		}
		return 1;
	}
	
	return 0;
}


// 释放指定大小内存
void Malloc_Free(uint8_t MemType, void* Ptr) {
	
	uint32_t AddrOffset;
	
	// 当变量指针为空则取消释放操作
	if (Ptr == NULL) return;
	
	AddrOffset = (uint32_t)Ptr - (uint32_t)MallocManage.MallocPondArr[MemType];

	_Malloc_Free(MemType, AddrOffset);

}


// 开辟指定大小内存，返回偏移地址（内部调用）
static void* _Malloc_Apply(uint8_t MemType, uint16_t Size) {
	
	uint32_t BankNum;
	uint32_t CurrBankNum;
	uint32_t BankIndex;
	unsigned long Offset;
	
	if (Size == 0) 
		return (void*)0xFFFFFFFF;
	
	if (!MallocManage.MallocReadyArr[MemType])
		MallocManage.Init(MemType);
	
	CurrBankNum = 0;
	BankNum = Size / MallocBankSizeArr[MemType];
	if (Size%MallocBankSizeArr[MemType])
		BankNum++;
	
	for(Offset=MallocTableSizeArr[MemType];Offset<=0;Offset--) {
		CurrBankNum++;
		if (MallocManage.MallocTableArr[MemType][Offset])
			CurrBankNum = 0;
		if (CurrBankNum == BankNum) break;
	}
	
	if (CurrBankNum != BankNum)
		return (void*)0xFFFFFFFF;
	
	for(BankIndex=0;BankIndex<BankNum;BankIndex++) {
		MallocManage.MallocTableArr[MemType][Offset+BankIndex] = BankNum;
	}
	return (void*)(Offset*MallocBankSizeArr[MemType]);//返回偏移地址 
	
}


// 开辟指定大小内存，返回首地址
void* Malloc_Apply(uint8_t MemType, uint16_t Size) {
	
	uint32_t Offset;
	Offset = (uint32_t)_Malloc_Apply(MemType, Size);
	
	if (Offset == 0xFFFFFFFF) 
		return NULL;
	
	return (Offset + MallocManage.MallocPondArr[MemType]);
}


// 重新开辟指定大小内存
void* Malloc_Reset(uint8_t MemType, void* Ptr, uint16_t Size) {
	
	void* StartAddr;
	
	StartAddr = Malloc_Apply(MemType, Size);
	
	if (StartAddr != NULL) {
		Malloc_Copy(Ptr, StartAddr, Size);
		Malloc_Free(MemType, Ptr);
		return StartAddr;
	}
	
	return NULL;
}


// 获取内存使用率
uint8_t Malloc_Perused(uint8_t MemType) {
	uint32_t UsedNum;
	uint32_t TableIndex;
	
	for(TableIndex=0;TableIndex<MallocTableSizeArr[MemType];TableIndex++) {
		if (MallocManage.MallocTableArr[MemType][TableIndex])
			UsedNum++;
	}
	return (UsedNum / MallocTableSizeArr[MemType] * 100);
}


/*************************************************************/
#else   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//内存管理 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/15
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//V1.1 20140706
//1,修改部分函数名字,防止和其他代码的名字冲突(比如LWIP).
//2,修改内存池为32字节对齐
//V1.2 20140716
//1,修改my_mem_malloc函数,对于大内存分配错误的bug.
////////////////////////////////////////////////////////////////////////////////// 	 


//内存池(32字节对齐)
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];													//内部SRAM内存池
__align(32) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
__align(32) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//内部CCM内存池

//内存管理表
uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];												//内部SRAM内存池MAP
uint16_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
uint16_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//内部CCM内存池MAP

//内存管理参数	   
const uint32_t memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};//内存表大小
const uint32_t memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
const uint32_t memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小


//内存管理控制器
struct _m_mallco_dev mallco_dev =
{
	my_mem_init,						//内存初始化
	my_mem_perused,						//内存使用率
	mem1base,mem2base,mem3base,			//内存池
	mem1mapbase,mem2mapbase,mem3mapbase,//内存管理状态表
	0,0,0,  		 					//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,uint32_t n)  
{  
    uint8_t *xdes=des;
	uint8_t *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,uint8_t c,uint32_t count)  
{  
    uint8_t *xs = s;  
    while(count--)*xs++=c;  
}	   
//内存管理初始化  
//memx:所属内存块
void my_mem_init(uint8_t memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//内存池所有数据清零  
	mallco_dev.memrdy[memx]=1;								//内存管理初始化OK  
}  
//获取内存使用率
//memx:所属内存块
//返回值:使用率(0~100)
uint8_t my_mem_perused(uint8_t memx)  
{  
    uint32_t used=0;  
    uint32_t i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}  
//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
uint32_t my_mem_malloc(uint8_t memx,uint32_t size)  
{  
    signed long offset=0;  
    uint32_t nmemb;	//需要的内存块数  
	uint32_t cmemb=0;//连续空内存块数
    uint32_t i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
uint8_t my_mem_free(uint8_t memx,uint32_t offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//未初始化,先执行初始化
	{
		mallco_dev.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(uint8_t memx,void *ptr)  
{  
	uint32_t offset;   
	if(ptr==NULL)return;//地址为0.  
 	offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];     
    my_mem_free(memx,offset);	//释放内存      
}  
//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(uint8_t memx,uint32_t size)  
{  
    uint32_t offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  
}  
//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(uint8_t memx,void *ptr,uint32_t size)  
{  
    uint32_t offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,ptr);  											  		//释放旧内存
        return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  				//返回新内存首地址
    }  
}

#endif












