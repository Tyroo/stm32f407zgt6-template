#include "malloc.h"


// 内存池，指定32字节位对齐
__align(32) u8 Memory_Pond_Int[MEM1_MAX_SIZE];																	// 内部内存块
__align(32) u8 Memory_Pond_Ext[MEM2_MAX_SIZE] __attribute__((at(0x68000000)));	// 外部SRAM内存
__align(32) u8 Memory_Pond_Ccm[MEM3_MAX_SIZE] __attribute__((at(0x10000000)));	// 内部CCM内存

// 内存管理表
u16 Memory_Table_Int[MEM1_TABLE_SIZE]; 																									// 内部SRAM内存管理表
u16 Memory_Table_Ext[MEM2_TABLE_SIZE] __attribute__((at(0X68000000 + MEM2_MAX_SIZE)));	// 外部SRAM内存管理表
u16 Memory_Table_Ccm[MEM3_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	// 内部CCM内存管理表

// 内存大小数组
const u32 MallocTableSizeArr[MEM_TYPE_SIZE] = {MEM1_TABLE_SIZE,
MEM2_TABLE_SIZE, MEM3_TABLE_SIZE}; //内存表大小
const u32 MallocPondSizeArr[MEM_TYPE_SIZE] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE,
MEM3_MAX_SIZE}; //内存分块大小
const u32 MallocBankSizeArr[MEM_TYPE_SIZE] = {MEM1_BANK_SIZE,MEM2_BANK_SIZE,
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
uint8_t _Malloc_Free(uint8_t MemType, uint32_t Offset) {
	
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
void* _Malloc_Open(uint8_t MemType, uint16_t Size) {
	
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
void* Malloc_Open(uint8_t MemType, uint16_t Size) {
	
	uint32_t Offset;
	Offset = (uint32_t)_Malloc_Open(MemType, Size);
	
	if (Offset == 0xFFFFFFFF) 
		return NULL;
	
	return (Offset + MallocManage.MallocPondArr[MemType]);
}


// 重新开辟指定大小内存
void* Malloc_RstOpen(uint8_t MemType, void* Ptr, uint16_t Size) {
	
	void* StartAddr;
	
	StartAddr = Malloc_Open(MemType, Size);
	
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
