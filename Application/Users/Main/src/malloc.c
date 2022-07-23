#include "malloc.h"

#if USE_NEW_MALLOC_MOD
// �ڴ�أ�ָ��32�ֽ�λ����
__align(32) uint8_t Memory_Pond_Int[MEM1_MAX_SIZE];																	// �ڲ��ڴ��
__align(32) uint8_t Memory_Pond_Ext[MEM2_MAX_SIZE] __attribute__((at(0x68000000)));	// �ⲿSRAM�ڴ�
__align(32) uint8_t Memory_Pond_Ccm[MEM3_MAX_SIZE] __attribute__((at(0x10000000)));	// �ڲ�CCM�ڴ�

// �ڴ�����
uint16_t Memory_Table_Int[MEM1_TABLE_SIZE]; 																									// �ڲ�SRAM�ڴ�����
uint16_t Memory_Table_Ext[MEM2_TABLE_SIZE] __attribute__((at(0X68000000 + MEM2_MAX_SIZE)));	// �ⲿSRAM�ڴ�����
uint16_t Memory_Table_Ccm[MEM3_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	// �ڲ�CCM�ڴ�����

// �ڴ��С����
const uint32_t MallocTableSizeArr[MEM_TYPE_SIZE] = {MEM1_TABLE_SIZE,
MEM2_TABLE_SIZE, MEM3_TABLE_SIZE}; //�ڴ���С
const uint32_t MallocPondSizeArr[MEM_TYPE_SIZE] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE,
MEM3_MAX_SIZE}; //�ڴ�ֿ��С
const uint32_t MallocBankSizeArr[MEM_TYPE_SIZE] = {MEM1_BANK_SIZE,MEM2_BANK_SIZE,
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
static uint8_t _Malloc_Free(uint8_t MemType, uint32_t Offset) {
	
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
	return (void*)(Offset*MallocBankSizeArr[MemType]);//����ƫ�Ƶ�ַ 
	
}


// ����ָ����С�ڴ棬�����׵�ַ
void* Malloc_Apply(uint8_t MemType, uint16_t Size) {
	
	uint32_t Offset;
	Offset = (uint32_t)_Malloc_Apply(MemType, Size);
	
	if (Offset == 0xFFFFFFFF) 
		return NULL;
	
	return (Offset + MallocManage.MallocPondArr[MemType]);
}


// ���¿���ָ����С�ڴ�
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


/*************************************************************/
#else   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/15
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//V1.1 20140706
//1,�޸Ĳ��ֺ�������,��ֹ��������������ֳ�ͻ(����LWIP).
//2,�޸��ڴ��Ϊ32�ֽڶ���
//V1.2 20140716
//1,�޸�my_mem_malloc����,���ڴ��ڴ��������bug.
////////////////////////////////////////////////////////////////////////////////// 	 


//�ڴ��(32�ֽڶ���)
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(32) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//�ⲿSRAM�ڴ��
__align(32) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//�ڲ�CCM�ڴ��
//�ڴ�����
uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];												//�ڲ�SRAM�ڴ��MAP
uint16_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP
uint16_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//�ڲ�CCM�ڴ��MAP
//�ڴ�������	   
const uint32_t memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};//�ڴ���С
const uint32_t memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//�ڴ�ֿ��С
const uint32_t memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//�ڴ��ܴ�С


//�ڴ���������
struct _m_mallco_dev mallco_dev =
{
	my_mem_init,						//�ڴ��ʼ��
	my_mem_perused,						//�ڴ�ʹ����
	mem1base,mem2base,mem3base,			//�ڴ��
	mem1mapbase,mem2mapbase,mem3mapbase,//�ڴ����״̬��
	0,0,0,  		 					//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des,void *src,uint32_t n)  
{  
    uint8_t *xdes=des;
	uint8_t *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,uint8_t c,uint32_t count)  
{  
    uint8_t *xs = s;  
    while(count--)*xs++=c;  
}	   
//�ڴ�����ʼ��  
//memx:�����ڴ��
void my_mem_init(uint8_t memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//�ڴ�״̬����������  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
	mallco_dev.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
}  
//��ȡ�ڴ�ʹ����
//memx:�����ڴ��
//����ֵ:ʹ����(0~100)
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
//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
uint32_t my_mem_malloc(uint8_t memx,uint32_t size)  
{  
    signed long offset=0;  
    uint32_t nmemb;	//��Ҫ���ڴ����  
	uint32_t cmemb=0;//�������ڴ����
    uint32_t i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ������
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  
//�ͷ��ڴ�(�ڲ�����) 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
uint8_t my_mem_free(uint8_t memx,uint32_t offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  
//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(uint8_t memx,void *ptr)  
{  
	uint32_t offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];     
    my_mem_free(memx,offset);	//�ͷ��ڴ�      
}  
//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(uint8_t memx,uint32_t size)  
{  
    uint32_t offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  
}  
//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(uint8_t memx,void *ptr,uint32_t size)  
{  
    uint32_t offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx,ptr);  											  		//�ͷž��ڴ�
        return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }  
}

#endif












