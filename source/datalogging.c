#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>


#include "gauge.h"
#include "usb.h"
#include "gui.h"
#include "datalogging.h"

unsigned char datalogBuff[512];
unsigned int datalogCnt=0;
unsigned char datalogLineCnt=0;



void datalog_init(GAUGE ** gList, unsigned char gaugeCnt)
{
unsigned char x;
char valueOut[50];
unsigned char valueOutCnt=0;

for (x=0;x<gaugeCnt;x++)
	{
	strncpy(&valueOut[valueOutCnt], gList[x]->label, GAUGE_LABEL_LENGTH);
	valueOutCnt += strnlen(gList[x]->label,GAUGE_LABEL_LENGTH);
	valueOut[valueOutCnt]=9; //add a tab
	valueOutCnt++;
	}

strncpy(&valueOut[valueOutCnt],"\r\n",2);
valueOutCnt+=2;

//Open file for writing----------------------------------------
datalog_openFile();



//--Write the header----------------------------------
			unsigned char strCount = strnlen(valueOut,50);
			char tempChars[3];
			
			dtostrf(strCount,2,0,tempChars); //the character count
			gui_puts(tempChars,0,0,0);
			USB_puts("WRF "); USB_puts(tempChars); USB_putc('\r');
			
			USB_puts(valueOut);

			USB_putc('\r');
			USB_read(5,500);

			datalog_sync();
//-----------------------------------------------------

datalog_closeFile();
datalog_switchToSerial();

}



void datalog_log(GAUGE ** gList, unsigned char gaugeCnt)
{

unsigned char x;
char valueOut[6];


for (x=0;x<gaugeCnt;x++)
	{
	dtostrf(gList[x]->curr_val,5,0,valueOut); //translate the value to chars
	valueOut[5]=9; //add a tab
	strncpy((char*)&datalogBuff[datalogCnt],valueOut,6);
	datalogCnt+=6; //increment index
	}

strncpy((char*)&datalogBuff[datalogCnt],"\r\n",2);
datalogCnt+=2;



	
//	datalog_closeFile();
//	while(1);
if (datalogCnt > 500)
	{
datalog_openFile();
datalog_writeBuff();
datalog_closeFile();
while(1);

	for (datalogCnt = 100;datalogCnt > 0; datalogCnt--)
		{
		datalog_writeBuff();		
		}


	
	datalogCnt = 0;
	datalog_switchToSerial();
//	while(1);
	}

}




void datalog_writeBuff()
{

//--Write the header----------------------------------
//datalog_switchToFlash();

		//	unsigned int strCount = strnlen((char*)datalogBuff,512);
		//	char tempChars[4];

		//	dtostrf(strCount,3,0,tempChars); //the character count

		//	gui_puts(tempChars,0,0,0);			

			USB_puts("WRF "); USB_puts("24"); USB_putc('\r');
			
			//USB_puts((char*)datalogBuff);
			USB_puts("hello12345678987654321\r\n");

			//USB_puts("\r\n"); 
			USB_putc('\r');
			USB_read(5,500);
	//		datalog_sync();
//-----------------------------------------------------

}






void datalog_openFile()
{
	datalog_switchToFlash();

	USB_puts("OPW log.xls\r");
	USB_read(5,500);	
	datalog_sync();
}






void datalog_closeFile()
{

//--Close file-----------------------------------
	datalog_switchToFlash();

	USB_puts("CLF log.xls\r");
	USB_read(5,500);
	datalog_sync();
//-------------------------------------------------

}


void datalog_switchToFlash()
{

char * buff = (char*)USB_getBuff();

	while(!USB_commandMode()) {;}

	while(1)
		{
		USB_puts("\r");
		USB_read(5,1000);
		gui_puts(buff,0,0,0);
		if (!strncmp(buff,"D:",2))
			break;
		}
gui_puts("Done",60,0,0);

/*
	USB_puts("SC 1\r"); //select the USBflash
	USB_read(5,500);
	datalog_sync();

	USB_puts("SC 1\r"); //select the USBflash
	USB_read(5,500);
	datalog_sync();
*/

}



void datalog_switchToSerial()
{

//	while(!USB_commandMode()) {;}

///	USB_puts("SC 0\r"); //select the USBserial
///	USB_read(5,500);

	while(!USB_dataMode()) {;}

}


void datalog_sync()
{
unsigned char * buff=USB_getBuff();

	while(1)
		{
		USB_puts("E\r"); USB_read(1,5);
		if (buff[0] == 'E')
			{
			while(USB_read(1,5)); //clear the buffer
			break;
			}
		}
}
