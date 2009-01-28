#ifndef _MUT_H_
#define _MUT_H_

#include "gauge.h"

#define MUT_EXISTS		0
#define MUT_RECVING		1
#define MUT_RECVOK		2
#define MUT_NOECU		3
#define MUT_RECVERROR	4

#define MUT_DELAYMIN	0 	//0*10 = 0ms
#define MUT_DELAYMAX	200 	//200*10 = 2000ms

#define MUT_KEEPALIVEMAX 2000	//128us * 5000 = 500ms

char mut_init(); 
void mut_convertSensors(GAUGE * g_list[],unsigned char gauge_count);
unsigned char mut_requestPID(unsigned char PID);


GAUGE* mut_getGaugeList();
unsigned char mut_getGaugeCount();
unsigned char mut_getPrecision(unsigned char param_id);

//void mut_adjustDelay(char amount);
//unsigned char mut_checkStatus();
#endif
