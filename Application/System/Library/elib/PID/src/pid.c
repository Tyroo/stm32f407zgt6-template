#include "pid.h"


// 位置式PID
void PID_Positional_Loop(PID_ObjectType *pidobj)
{
	pidobj->pid_currError = pidobj->pid_refValue - pidobj->pid_flbValue;
	
	pidobj->pid_pValue = pidobj->pid_Kp * pidobj->pid_currError;
	
	pidobj->pid_iValue += (pidobj->pid_Ki * pidobj->pid_currError);
	
	pidobj->pid_dValue = pidobj->pid_Kd * (pidobj->pid_currError - pidobj->pid_nextError);
	
	pidobj->pid_outValue += (pidobj->pid_pValue + pidobj->pid_iValue + pidobj->pid_dValue - pidobj->pid_constValue);
	
	pidobj->pid_lastError = pidobj->pid_nextError;
	
	pidobj->pid_nextError = pidobj->pid_currError;
}


// 增量式PID
void PID_Incremental_Loop(PID_ObjectType *pidobj)
{
	pidobj->pid_currError = pidobj->pid_refValue - pidobj->pid_flbValue;
	
	pidobj->pid_pValue = pidobj->pid_Kp * (pidobj->pid_currError -  pidobj->pid_nextError);
	
	pidobj->pid_iValue = pidobj->pid_Ki * pidobj->pid_currError;
	
	pidobj->pid_dValue = pidobj->pid_Kd * (pidobj->pid_currError + pidobj->pid_lastError - 2 * pidobj->pid_nextError);
	
	pidobj->pid_outValue += (pidobj->pid_pValue + pidobj->pid_iValue + pidobj->pid_dValue - pidobj->pid_constValue);
	
	pidobj->pid_lastError = pidobj->pid_nextError;
	
	pidobj->pid_nextError = pidobj->pid_currError;
}
