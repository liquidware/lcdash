#ifndef _DATALOGGING_H_
#define _DATALOGGING_H_

void datalog_init(GAUGE ** gList, unsigned char gaugeCnt);
void datalog_writeBuff();
void datalog_log(GAUGE ** gList, unsigned char gaugeCnt);
void datalog_openFile();
void datalog_closeFile();
void datalog_switchToFlash();
void datalog_switchToSerial();
void datalog_sync();
#endif
