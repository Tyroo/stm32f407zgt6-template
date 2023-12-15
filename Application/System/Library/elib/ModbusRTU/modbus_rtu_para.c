#include "modbus_rtu_para.h"


// MODBUS参数表（注意：寄存器地址必须从低到高填写）
const stcModbusPara_t gastcModbusPara_Table[MODBUS_RTU_PARA_TABLE_SIZE] = 
{
/*	  			寄存器地址	  					参数指针				  	参数位数			 参数读/写		  	参数访问回调函数		*/
	{			0x0000,								NULL,				{ enValueType_Bit8, 	enReadWrite },		NULL				},
};


// 根据寄存器地址，查询参数表中的参数索引
const stcModbusPara_t * ModbusPara_Find(uint16_t u16RegAddr, uint16_t * pu16ParaIndex, 
										const stcModbusPara_t * astcParaTable, uint16_t u16ParaTableSize)
{
	const stcModbusPara_t * pstcPara = NULL;
	
	int16_t s16FindIndex;
	int16_t s16LowIndex = 0;//初始状态 low 指针指向第一个关键字
	int16_t s16HighIndex = u16ParaTableSize - 1;//high 指向最后一个关键字
	
	// 使用二分查找，查找参数索引
	while (s16LowIndex <= s16HighIndex)
	{
		s16FindIndex = (s16LowIndex + s16HighIndex) >> 1;
		
        if (astcParaTable[s16FindIndex].u16RegAddr == u16RegAddr)
		{
			pstcPara = &astcParaTable[s16FindIndex];
			break;
		}
        else if (astcParaTable[s16FindIndex].u16RegAddr > u16RegAddr)
		{
			s16HighIndex = s16FindIndex - 1;
		}
		else
		{
			s16LowIndex = s16FindIndex + 1;
		}
	}

	if (pu16ParaIndex != NULL)
	{
		*pu16ParaIndex = s16FindIndex;
	}
	
    return pstcPara;
}
