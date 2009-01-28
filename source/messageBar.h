#ifndef _MESSAGEBAR_H_
#define _MESSAGEBAR_H_

#define MAX_MSGS	5		//!< The Maximum mssages we can have popped into the message queue 
#define MAX_MSG_SIZE	40

#define OFF		0
#define ON		1

void messageBar_init();

void messageBar_addMsg(char * the_msg);

void messageBar_refresh();
void messageBar_clear();

#endif 
