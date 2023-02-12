#ifndef __PID_H__
#define __PID_H__

typedef struct 
{
	float pid_Kp;
	float pid_Ki;
	float pid_Kd;
	
	float pid_refValue;
	float pid_flbValue;
	
	float pid_currError;
	float pid_nextError;
	float pid_lastError;
	
	float pid_pValue;
	float pid_iValue;
	float pid_dValue;
	
	float pid_constValue;
	
	float pid_outValue;

	void (*PID_Pos_Run)();
	void (*PID_Inc_Run)();
	
} PID_ObjectType;


#endif
