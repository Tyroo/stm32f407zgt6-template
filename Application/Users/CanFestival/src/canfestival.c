#include "canfestival.h"
#include "can.h"


void setTimer(TIMEVAL value) {
	
}


TIMEVAL getElapsedTime(void) {
	return 1;
}


unsigned char canSend(CAN_PORT notused, Message *m) {
	//return CAN1_Send_Msg(m);
	return 0;
}


void timerFor(void) {

}


