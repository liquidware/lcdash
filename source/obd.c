#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>

#include "usb.h"
#include "obd.h"
#include "obd_gauges.h"
#include "FT232R.h"
#include "gui.h"
#include "t6963.h"

unsigned char obd_delay=0;
unsigned char obd_status=OBD_NOECU;


char obd_init()
{
char* buff = (char*)USB_getBuff();


	if (!FT232R_setBaud(B9600))
		return 0; //failed

	/*Lets get it started */
	USB_puts("ATE0\r"); 
	USB_read(10,200);

	USB_puts("ATE0\r"); 
	USB_read(10,200);

	USB_puts("ATL0\r"); //line feeds off
	USB_read(5,200);

	/* --------Begin ELM 327 Setup ---*/
	USB_puts("ATZ\r"); //send a reset
	USB_read(20,1000); //read the response
	if (strncmp(&buff[2],"ELM327",6))
		{
		//gui_puts("No OBD",0,0,0); 
		return 0; //there is no ELM attached
		}
	else
		{
		gui_puts("Found OBD",0,0,0); //got OBD
		}

	/* --- Echo Off ---*/
	USB_puts("ATE0\r"); 
	USB_read(10,200);
	gui_puts("Echo:",0,8,0);
	if (!strncmp(&buff[5],"OK",2))
		gui_puts("OK",36,8,0);
	else
		gui_puts("FAILED!",36,8,0);		

	/* --- Line Feeds Off ---*/
	USB_puts("ATL0\r"); //line feeds off
	USB_read(5,200);
	gui_puts("Line FD:",0,16,0);
	if (!strncmp(&buff[0],"OK",2))
		gui_puts("OK",54,16,0);
	else
		gui_puts("FAILED!",54,16,0);

	/* --- Auto Search protocol ---*/
	USB_puts("ATTP0\r"); //Set protocol to automatic
	USB_read(5,200);
	gui_puts("AUTO OBD:",0,24,0);
	if (!strncmp(&buff[0],"OK",2))
		gui_puts("OK",60,24,0);
	else
		gui_puts("FAILED!",60,24,0);


	/* --------Begin OBD ---*/
	USB_puts("0100\r"); //Request supported PIDs
	USB_read(34,7000);
	if (!strncmp(&buff[0],"SEARCHING",2))
		{
		gui_puts("OBD Init...",0,32,0);
		}
	if (!strncmp(&buff[13],"41 00",2))
		{
		gui_puts("OK",72,32,0); /* Response from 41 00 */
		}
	else
		{
		gui_puts("FAILED!",72,32,0);
		}

obd_status=OBD_EXISTS; //a global, saying OBD is alive
return 1;
}



unsigned char obd_checkStatus()
{
	return obd_status;
}



/*
#define PID_ENGINELOAD		0x04
#define PID_ENGINECLT		0x05
#define PID_STFUELTRIMB1	0x06
#define PID_LTFUELTRIMB1	0x07
#define PID_STFUELTRIMB2	0x08
#define PID_LTFUELTRIMB2	0x09
#define PID_FUELPRESS		0x0A
#define PID_MAP				0x0B
#define PID_RPM				0x0C
#define PID_VSS				0x0D
#define PID_ADVANCE			0x0E
#define PID_MAT				0x0F
#define PID_MAF				0x10
#define PID_TPS				0x11
*/

void obd_convertSensors(GAUGE * g_list[],unsigned char gauge_count)
{

GAUGE * g;


while(gauge_count--)
	{

	
	g = g_list[gauge_count];

	if (g->param_id == PID_ENGINELOAD)
		{
		g->curr_val = (float)(obd_requestPID(PID_ENGINELOAD,1)*100/255);
		}
	else
	if (g->param_id == PID_ENGINECLT)
		{
		g->curr_val = (float)(obd_requestPID(PID_ENGINECLT,1)-40);	
		}
	else
	if (g->param_id == PID_STFUELTRIMB1)
		{
		g->curr_val = (float)(0.7812*(obd_requestPID(PID_STFUELTRIMB1,1)-128));	
		}
	else
	if (g->param_id == PID_LTFUELTRIMB1)
		{
		g->curr_val = (float)(0.7812*(obd_requestPID(PID_LTFUELTRIMB1,1)-128));
		}
	else
	if (g->param_id == PID_STFUELTRIMB2)
		{
		g->curr_val = (float)(0.7812*(obd_requestPID(PID_STFUELTRIMB2,1)-128));
		}
	else
	if (g->param_id == PID_LTFUELTRIMB2)
		{
		g->curr_val = (float)(0.7812*(obd_requestPID(PID_LTFUELTRIMB2,1)-128));
		}
	else
	if (g->param_id == PID_FUELPRESS)
		{
		g->curr_val = (float)(obd_requestPID(PID_FUELPRESS,1)*3);
		}
	else
	if (g->param_id == PID_RPM)
		{
		long v = obd_requestPID(PID_RPM,2);
		unsigned char a = (unsigned char) (v >> 8);
		unsigned char b = (unsigned char) v;

		g->curr_val = (float)( (a*256+b)/4 );
		}
	else
	if (g->param_id == PID_VSS)
		{
		g->curr_val = (float)obd_requestPID(PID_VSS,1);
		}
	else
	if (g->param_id == PID_ADVANCE)
		{
		g->curr_val = (float)(obd_requestPID(PID_ADVANCE,1)/2-64);
		}
	else			
	if (g->param_id == PID_MAT)
		{
		g->curr_val = (float)(obd_requestPID(PID_MAT,1)-40);
		}
	else
	if (g->param_id == PID_MAF)
		{
		long v = obd_requestPID(PID_MAF,2);
		unsigned char a = (unsigned char)(v >> 8);
		unsigned char b = (unsigned char) v;

		g->curr_val = (float)( ((256*a)+b)/100 );
		}
	else
	if (g->param_id == PID_TPS)
		{
		g->curr_val = (float)(obd_requestPID(PID_TPS,1)*100/255);
		}
	
	//Check for a receive error
	//If an error is found, zero out the gauge
	unsigned char commStatus = obd_checkStatus();
	if (commStatus == OBD_RECVERROR)
		{
		g->curr_val = 0;
		}
	else
	if (commStatus == OBD_NOECU)
		{
		g->curr_val = 0;
		}

	} //end while

} //end function



long obd_requestPID(unsigned char PID, unsigned char size)
{
unsigned char x;
char * buff=(char*)USB_getBuff();
char out[3];
long val=0;


while(USB_read(1,30)); //purge
USB_puts("01"); //mode 01 PID's
itoa(PID,out,16);

if (PID<16)
	{
	out[1] = out[0]; //for single digits
	out[0] = '0';
	}

if (PID==0)
	{
	out[0] = '0';
	out[1] = '0';
	}

USB_putc(out[0]);
USB_putc(out[1]);
USB_putc('\r');


unsigned char recv_cnt= (size*3)+6;
char * t_buff = buff;

unsigned char count = USB_read(recv_cnt,1000);

if ((buff[0] == '4') && (buff[1] == '1'))
	{
	char* endptr;

	t_buff+=6;
	val = strtol(t_buff,&endptr,16);
	for (x=0;x<size-1;x++)
		{	
		val = val << 8;
		val += strtol(endptr,&endptr,16);
		}

	obd_status = OBD_RECVOK; //good response
	}
else
	{
//---Error checking here----//
	if ( !strncmp(buff,"NO DATA",7) || (count == 0) )
		{
		obd_status = OBD_NOECU; //engine is turned off
		obd_delay=0;			//reset the delay
		}
	else
		{
		obd_adjustDelay(5); //add some delay because we're querying too fast 
		obd_status = OBD_RECVERROR;
		}

	}

for(x=0;x<obd_delay;x++)
		delay_ms(10);

return val;
}




void obd_adjustDelay(char amount)
{

	if (amount>0)
		{
		if ((obd_delay+amount) < OBD_DELAYMAX)
			obd_delay+=amount; // add some delay
		}
	else
		{
		if (obd_delay > OBD_DELAYMIN)
			obd_delay-=amount; // add some delay
		}
}


GAUGE* obd_getGaugeList()
{

	return &obd_GaugeList[0];

}

unsigned char obd_getGaugeCount()
{

	return OBD_GAUGE_CNT;
}

