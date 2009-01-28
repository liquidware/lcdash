#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <math.h>

#include "usb.h"
#include "mut.h"
#include "mut_gauges.h"
#include "FT232R.h"
#include "gui.h"
#include "t6963.h"
#include "mut_lookups.h"
#include "bitops.h"

unsigned char mut_delay=0;
volatile unsigned char mut_status=MUT_NOECU;


char mut_init()
{

gui_puts("MUT INIT...",0,0,0);

CLRBIT(TIMSK1,OCIE1A); //disable timer1 interrupts

//ADCSRA |= (0<<ADIE); //disable ADC interrupts

////////////////////////////////////////////
//Begin MUT test
	unsigned char* buff = USB_getBuff();

	USB_commandMode();
	while(USB_read(1,20));




/////////////////////////////////////////////////////////////////
	USB_puts("fbd $C00000\r");	//set baud to 15625
	USB_read(5,400);
/////////////////////////////////////////////////////////////////

unsigned char x=3;

while(x--)
{

/////////////////////////////////////////////////////////////////
	USB_puts("fmc $0202\r");	//RTS# low
	USB_read(5,400);
//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
	USB_puts("fmc $0101\r");	//DTR# High
	USB_read(5,400);
//////////////////////////////////////////////////////////////////
cli();

delay_ms(192); //MUT 200ms 0

delay_ms(192); //MUT 200ms 0
delay_ms(192); //MUT 200ms 1
delay_ms(192); //MUT 200ms 2
delay_ms(192); //MUT 200ms 3
delay_ms(192); //MUT 200ms 4
delay_ms(192); //MUT 200ms 5
delay_ms(192); //MUT 200ms 6
delay_ms(192); //MUT 200ms 7

sei();

/////////////////////////////////////////////////////////////////
	USB_puts("fmc $0001\r");	//DTR# low
	USB_read(5,400);
//////////////////////////////////////////////////////////////////

cli();
	delay_ms(192); //MUT 200ms stop bit
	delay_ms(192); //MUT 200ms again
	delay_ms(95); //MUT 100ms delay
sei();	
/////////////////////////////////////////////////////////////////
	USB_puts("fmc $0002\r");	//RTS# high
	USB_read(5,400);
//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
	USB_puts("fmc $0101\r");	//DTR# High
	USB_read(5,400);
//////////////////////////////////////////////////////////////////

}

	USB_dataMode();

//gui_puts("DONE",70,0,0);

unsigned char cnt=0;
char out[10];

x=2;
while(x--)
{


	
	USB_putc(0x17);
	cnt = USB_read(2,200);
	itoa(cnt,out,16);
	gui_puts(out,50,32,0);

	itoa(buff[0],out,16);
	gui_puts("  ",0,40,0);
	gui_puts(out,0,40,0);

	itoa(buff[1],out,16);
	gui_puts("  ",15,40,0);
	gui_puts(out,15,40,0);
	while(USB_read(1,20) ); //purge
}

if (cnt==2)
	{
	gui_puts("MUT ECU Found",0,8,0);
	}
else
	{
	gui_puts("MUT NOT Found!",0,8,0);
	SETBIT(TIMSK1,OCIE1A);//enable interrupts on timer 1 overflow
	return 0;
	}

SETBIT(TIMSK1,OCIE1A);//enable interrupts on timer 1 overflow
//TIMSK1 = (1 << OCIE1A); //enable interrupts on timer 1 overflow
//TIMSK1 = timer1MaskSave; //enable timer1 interrupts


return 1;
}





/*
#define MUT_TPS				0x17
#define MUT_RPM				0x21
#define MUT_MAP				0x38
#define MUT_ACCELENRICH		0x1D
#define MUT_TIMINGADV		0x06
#define MUT_FUELTRIM_LOW	0x0E
#define MUT_FUELTRIM_MED	0x0D
#define MUT_FUELTRIM_HIGH	0x0C
#define MUT_O2FEEDBACKTRIM	0x0F
#define MUT_O2SENSOR		0x13
#define MUT_O2SENSOR2		0x1F
#define MUT_VSS				0x2F
#define MUT_CLT				0x07
#define MUT_AIRFLOW			0x1A
#define MUT_MAT				0x3A
#define MUT_INJPW			0x29
#define MUT_BAT				0x14
#define MUT_AFRMAP			0x32
#define MUT_LOADCALC		0x00
#define MUT_AIRVOL			0x2C
#define MUT_BARO			0x15
#define MUT_KNOCKSUM		0x26
#define MUT_ISCSTEPS		0x16
#define MUT_OCTANELEVEL		0x27
#define MUT_TARGETIDLERPM	0x24
#define MUT_KNOCKVOLTAGE	0x30
#define MUT_EGRTEMP			0x12
*/



void mut_convertSensors(GAUGE * g_list[],unsigned char gauge_count)
{

GAUGE * g;


while(gauge_count--)
	{

	
	g = g_list[gauge_count];

	if (g->param_id == MUT_TPS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_TPS)*100/255);
		}
	else

	if (g->param_id == MUT_RPM)
		{
		g->curr_val = (float)(mut_requestPID(MUT_RPM)*31.25);
		}
	else

	if (g->param_id == MUT_MAP)
		{
		g->curr_val = (float)(mut_requestPID(MUT_MAP)*0.19348);
		}
	else

	if (g->param_id == MUT_ACCELENRICH)
		{
		g->curr_val = (float)(mut_requestPID(MUT_ACCELENRICH)*200.0/255.0);
		}
	else

	if (g->param_id == MUT_TIMINGADV)
		{
		g->curr_val = (float)(mut_requestPID(MUT_TIMINGADV)-20);
		}
	else

	if (g->param_id == MUT_FUELTRIM_LOW)
		{
		g->curr_val = (float)( (mut_requestPID(MUT_FUELTRIM_LOW)*0.1961) - 25);
		}
	else

	if (g->param_id == MUT_FUELTRIM_MED)
		{
		g->curr_val = (float)( (mut_requestPID(MUT_FUELTRIM_MED)*0.1961) - 25);
		}
	else

	if (g->param_id == MUT_FUELTRIM_HIGH)
		{
		g->curr_val = (float)( (mut_requestPID(MUT_FUELTRIM_HIGH)*0.1961) - 25);
		}
	else

	if (g->param_id == MUT_O2FEEDBACKTRIM)
		{
		g->curr_val = (float)(mut_requestPID(MUT_O2FEEDBACKTRIM)*0.78125);
		}
	else

	if (g->param_id == MUT_O2SENSOR)
		{
		g->curr_val = (float)(mut_requestPID(MUT_O2SENSOR)*0.0195);
		}
	else

	if (g->param_id == MUT_O2SENSOR2)
		{
		g->curr_val = (float)(mut_requestPID(MUT_O2SENSOR2)*0.0195);
		}
	else

	if (g->param_id == MUT_VSS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_VSS)*1.2427);
		}
	else

	if (g->param_id == MUT_CLT)
		{
		unsigned char i = mut_requestPID(MUT_CLT);
		g->curr_val = (float)(i);// pgm_read_word(&mut_Coolant[i]));
		}
	else

	if (g->param_id == MUT_AIRFLOW)
		{
		g->curr_val = (float)(mut_requestPID(MUT_AIRFLOW)*6.29);
		}
	else

	if (g->param_id == MUT_MAT)
		{
		unsigned char i = mut_requestPID(MUT_MAT);


		g->curr_val = (float)(i);//pgm_read_word(&mut_MAT[i]));
		}
	else

	if (g->param_id == MUT_INJPW)
		{
		g->curr_val = (float)(mut_requestPID(MUT_INJPW)*0.256);
		}
	else

	if (g->param_id == MUT_BAT)
		{
		g->curr_val = (float)(mut_requestPID(MUT_BAT)*0.0733);
		}
	else

	if (g->param_id == MUT_AFRMAP)
		{
		g->curr_val = (float)(mut_requestPID(MUT_AFRMAP));
		}
	else

	if (g->param_id == MUT_ECULOAD)
		{
		g->curr_val = (float)(mut_requestPID(MUT_ECULOAD)*0.625);
		}
	else

	if (g->param_id == MUT_ECULOAD2B)
		{
		unsigned int val;
		val = mut_requestPID(MUT_ECULOAD) << 8;
		val+= mut_requestPID(MUT_ECULOAD2B);
		g->curr_val = (float)(val*0.3125);
		}
	else

	if (g->param_id == MUT_AIRVOL)
		{
		g->curr_val = (float)(mut_requestPID(MUT_AIRVOL));
		}
	else

	if (g->param_id == MUT_KNOCKSUM)
		{
		g->curr_val = (float)(mut_requestPID(MUT_KNOCKSUM));
		}
	else

	if (g->param_id == MUT_ISCSTEPS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_ISCSTEPS));
		}
	else

	if (g->param_id == MUT_VSS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_VSS)*1.2427);
		}
	else

	if (g->param_id == MUT_OCTANELEVEL)
		{
		g->curr_val = (float)(mut_requestPID(MUT_OCTANELEVEL)*100/255);
		}
	else

	if (g->param_id == MUT_VSS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_VSS)*1.2427);
		}
	else

	if (g->param_id == MUT_TARGETIDLERPM)
		{
		g->curr_val = (float)(mut_requestPID(MUT_TARGETIDLERPM)*7.8);
		}
	else

	if (g->param_id == MUT_KNOCKVOLTAGE)
		{
		g->curr_val = (float)(mut_requestPID(MUT_KNOCKVOLTAGE)*1.2427);
		}
	else

	if (g->param_id == MUT_VSS)
		{
		g->curr_val = (float)(mut_requestPID(MUT_VSS)*0.0195);
		}
	else

	if (g->param_id == MUT_EGRTEMP)
		{
		g->curr_val = (float)(-2.7*(float)mut_requestPID(MUT_EGRTEMP)+597.7);
		}
	else

	if (g->param_id == MUT_MPG)
		{
		//560cc is for the INJECTOR size, make user inputable
		float speed = 		  (float)(mut_requestPID(MUT_VSS)*1.2427);
		float injPulseWidth = (float)(mut_requestPID(MUT_INJPW)*0.256);
		float injDutyCycle = (float)(injPulseWidth*31.25*mut_requestPID(MUT_RPM)/1200);
		float denom = (560*4*injDutyCycle/100*0.015873);
		if (denom != 0)
			g->curr_val = (speed/denom);
		else
			g->curr_val = 0;
		}
	else

	if (g->param_id == MUT_INJDUTYCYCLE)
		{
		float injPulseWidth = (float)(mut_requestPID(MUT_INJPW)*0.256);
		g->curr_val = (float)(injPulseWidth*31.25*mut_requestPID(MUT_RPM)/1200);
		}
	else

	if (g->param_id == MUT_ESTHP)
		{
		/*
				unsigned int val;
		val = mut_requestPID(MUT_ECULOAD) << 8;
		val+= mut_requestPID(MUT_ECULOAD2B);
		g->curr_val = (float)(val*0.3125);
		*/
		unsigned int ecuload = (mut_requestPID(MUT_ECULOAD) << 8) +  mut_requestPID(MUT_ECULOAD2B);
		float ecuload2b = (float) ( ecuload * 0.3125);
		float rpm = (float)(31.25*mut_requestPID(MUT_RPM));
		float grev = ecuload2b/95;
		float Lbsmin = (grev*rpm) /454;
		
		g->curr_val = Lbsmin*9.15;

/*
Estimated g/rev : Eval => [LoadMUT2Byte]/95
Estimated Lbs/min: Eval => ([gRev]*[rpm])/454
Estimated Horsepower: Eval => [LbsMin]*9.15
Estimated Torque: Eval => ([WHP]*5252/[rpm])
*/

		}
//	else
//	if 
//	else



	} //end while




} //end function




unsigned char mut_requestPID(unsigned char PID)
{

unsigned char cnt;
//char out[10];
unsigned char * buff=(unsigned char*)USB_getBuff();

mut_status = MUT_RECVING;
while(USB_read(1,10)); //purge


	USB_putc(PID);
	cnt = USB_read(2,200);

//error checking
if (buff[0] != PID)
	{
	//not connected
	mut_status = MUT_NOECU;
	}
else
if (cnt == 1)
	{
	//not connected
	mut_status = MUT_NOECU;
	}
else
	{
	mut_status= MUT_RECVOK;
	return buff[1]; //valid response
	}


return 0; //should never get here
}






unsigned char mut_getPrecision(unsigned char param_id)
{

	if ( 	param_id == MUT_INJPW || param_id == MUT_O2SENSOR || 
			param_id == MUT_O2SENSOR2 || param_id == MUT_MPG || 
			param_id == MUT_BAT || param_id == MUT_ESTHP)
		{
		return 1;
		}

return 0; //normal precision
}




GAUGE* mut_getGaugeList()
{

	return &mut_GaugeList[0];

}




unsigned char mut_getGaugeCount()
{

	return MUT_GAUGE_CNT;
}


