#include "malloc.h"


// �ڴ�أ�ָ��32�ֽ�λ����
__align(32) u8 Memory_Pond_Int[MEM1_MAX_SIZE];																	// �ڲ��ڴ��
__align(32) u8 Memory_Pond_Ext[MEM2_MAX_SIZE] __attribute__((at(0x68000000)));	// �ⲿSRAM�ڴ�
__align(32) u8 Memory_Pond_Ccm[MEM3_MAX_SIZE] __attribute__((at(0x10000000)));	// �ڲ�CCM�ڴ�

// �ڴ�����
u16 Memory_Table_Int[MEM1_TABLE_SIZE]; 																									// �ڲ�SRAM�ڴ�����
u16 Memory_Table_Ext[MEM2_TABLE_SIZE] __attribute__((at(0X68000000 + MEM2_MAX_SIZE)));	// �ⲿSRAM�ڴ�����
u16 Memory_Table_Ccm[MEM3_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	// �ڲ�CCM�ڴ�����

// �ڴ��С����
const u32 MallocTableSizeArr[MEM_TYPE_SIZE] = {MEM1_TABLE_SIZE,
MEM2_TABLE_SIZE, MEM3_TABLE_SIZE}; //�ڴ���С
const u32 MallocPondSizeArr[MEM_TYPE_SIZE] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE,
MEM3_MAX_SIZE}; //�ڴ�ֿ��С
const u32 MallocBankSizeArr[MEM_TYPE_SIZE] = {MEM1_BANK_SIZE,MEM2_BANK_SIZE,
MEM3_BANK_SIZE};

// �ڴ����ṹ�嶨��
Malloc_Manage MallocManage = MALLOC_MANAGE_DEFAULT;


// �ڴ����ģ���ʼ��
void Malloc_Init(uint8_t MemType) {
	Malloc_Set(MallocManage.MallocPondArr[MemType], 0, MallocPondSizeArr[MemType]);
	Malloc_Set(MallocManage.MallocTableArr[MemType], 0, MallocTableSizeArr[MemType]);
	MallocManage.MallocReadyArr[MemType] = 1;
}


// ��ָ���ڴ��ַ�����ݷ������һ��ַ
void Malloc_Copy(void* SrcAddr, void* DesAddr, uint16_t Len) {
	
	uint8_t* DesAddrTemp = DesAddr;
	uint8_t* SrcAddrTemp = SrcAddr; 
	
	while(Len--)
		*DesAddrTemp++ = *SrcAddrTemp++; 
}


// ����һ�ε�ַ�ڴ��ֵλ����ֵ
void Malloc_Set(void* DesAddr, uint16_t Value, uint16_t Len) {

	uint8_t* DesAddrTemp = DesAddr;
	
	while(Len--) {
		*DesAddrTemp++ = Value;
	}
}


// �ͷ�ָ���ڴ棨�ڲ����ã�
uint8_t _Malloc_Free(uint8_t MemType, uint32_t Offset) {
	
	uint32_t BankOffset;
	uint32_t BankIndex;
	uint32_t FreeBankNum;
	
	// δ��ʼ�����ȳ�ʼ��
	if (!MallocManage.MallocReadyArr[MemType]) {
		MallocManage.Init(MemType);
		return 2;		// δ��ʼ��
	}
		
	if (Offset<MallocPondSizeArr[MemType]) {
		// ��ȡҪ�ͷŵ���ʼ���
		BankOffset = Offset / MallocBankSizeArr[MemType];
		// ��ȡҪ�ͷŵĿ���Ŀ
		FreeBankNum = MallocManage.MallocPondArr[MemType][BankOffset];
		
		// ����Ӧ�Ŀ���������
		for(BankIndex=0;BankIndex<FreeBankNum;BankIndex++) {
			MallocManage.MallocTableArr[MemType][FreeBankNum+BankIndex] = 0;
		}
		return 1;
	}
	
	return 0;
}


// �ͷ�ָ����С�ڴ�
void Malloc_Free(uint8_t MemType, void* Ptr) {
	
	uint32_t AddrOffset;
	
	// ������ָ��Ϊ����ȡ���ͷŲ���
	if (Ptr == NULL) return;
	
	AddrOffset = (uint32_t)Ptr - (uint32_t)MallocManage.MallocPondArr[MemType];

	_Malloc_Free(MemType, AddrOffset);

}


// ����ָ����С�ڴ棬����ƫ�Ƶ�ַ���ڲ����ã�
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
	return (void*)(Offset*MallocBankSizeArr[MemType]);//����ƫ�Ƶ�ַ 
	
}


// ����ָ����С�ڴ棬�����׵�ַ
void* Malloc_Open(uint8_t MemType, uint16_t Size) {
	
	uint32_t Offset;
	Offset = (uint32_t)_Malloc_Open(MemType, Size);
	
	if (Offset == 0xFFFFFFFF) 
		return NULL;
	
	return (Offset + MallocManage.MallocPondArr[MemType]);
}


// ���¿���ָ����С�ڴ�
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


// ��ȡ�ڴ�ʹ����
uint8_t Malloc_Perused(uint8_t MemType) {
	uint32_t UsedNum;
	uint32_t TableIndex;
	
	for(TableIndex=0;TableIndex<MallocTableSizeArr[MemType];TableIndex++) {
		if (MallocManage.MallocTableArr[MemType][TableIndex])
			UsedNum++;
	}
	return (UsedNum / MallocTableSizeArr[MemType] * 100);
}
