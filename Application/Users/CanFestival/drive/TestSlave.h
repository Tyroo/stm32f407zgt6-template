
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef TESTSLAVE_H
#define TESTSLAVE_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 TestSlave_valueRangeTest (UNS8 typeValue, void * value);
const indextable * TestSlave_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data TestSlave_Data;
extern UNS16 Slave_FuncCode_F0[24];		/* Mapped at index 0x20F0, subindex 0x01 - 0x18 */

#endif // TESTSLAVE_H
