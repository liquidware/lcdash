#ifndef _OBD_H_
#define _OBD_H_

#include "gauge.h"

#define OBD_EXISTS		0
#define OBD_RECVOK		1
#define OBD_NOECU		2
#define OBD_RECVERROR	3

#define OBD_DELAYMIN	0 	//0*10 = 0ms
#define OBD_DELAYMAX	200 	//200*10 = 2000ms

char obd_init(); 
void obd_convertSensors(GAUGE * g_list[],unsigned char gauge_count);
long obd_requestPID(unsigned char PID,unsigned char size);


GAUGE* obd_getGaugeList();
unsigned char obd_getGaugeCount();
void obd_adjustDelay(char amount);
unsigned char obd_checkStatus();
#endif
