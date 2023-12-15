#ifndef __AT_COMMAND_INSTRUCT_H__
#define __AT_COMMAND_INSTRUCT_H__


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct
{
	const char * ps8Command;
	bool (*pvInstructCallbackFunc)(const char * ps8Data, uint16_t u16DataSize);
} stcAtCommand_Instruct_t;


typedef enum
{
	enAtCommand_Instruct_Test,			// 测试
	enAtCommand_Instruct_Bottom
} enAtCommand_Instruct_t;


extern const stcAtCommand_Instruct_t gastcAtCommand_Instruct_Table[enAtCommand_Instruct_Bottom];

#endif
