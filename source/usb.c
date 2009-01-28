#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usb.h"
#include "bitops.h"
#include "t6963.h"
#include "gui.h"

//Global variables
unsigned char USB_buff[USB_BUFF_SIZE]; /*!< The actual USB buffer */
unsigned int USB_buff_cnt=0;	/*!< The current size of the USB buffer */

volatile char USB_alarmStatus; /*!< Our status of the timeout alarm */
volatile unsigned int USB_alarmCounter; /*!<  A counter for our timeout alarm */

volatile char USB_currentMode; /*!< The current mode for the style of comms with the VNC1L, either COMMANDMODE, or DATAMODE */


///////////////////////////////////
//Timer 0 Overflow Interrupt, running at 1mS overflow rate for the timeout timer.
//////////////////////////////////////
SIGNAL (SIG_OVERFLOW0)
{

if (USB_alarmCounter != 1 )
	{
	USB_alarmCounter--;   //decrement the ms counter
	SETBIT(TIFR0,TOV0); //restart the timer
	}
else
	USB_alarmStatus = ALARM_SOUND;


}



///////////
//
////////////////////////
unsigned char* USB_getBuff()
{
	return (unsigned char*)USB_buff;
}



///////////
//
////////////////////////
unsigned char USB_getBuffSize()
{
	return USB_buff_cnt;
}



///////////
//
////////////////////////
char USB_getMode()
{
	return USB_currentMode;
}



///////////
//
////////////////////////
unsigned char USB_read(unsigned int amount,unsigned int timeout_cnt)
{

unsigned int  x;
unsigned char data;


//flush the bufffer
for(x=0;x<USB_BUFF_SIZE;x++)
	{
	USB_buff[x] = 0x00;
	}
USB_buff_cnt = 0;

//USB_flushChip(); /*! Clear any messages waiting */

//set and start the alarm
USB_setAlarm(timeout_cnt);

while ( (USB_buff_cnt < amount) && USB_checkAlarm() ) 
	{


	//-------Read Stage
	if ( CHECKBIT(PIND,USB_RXF) == 0 )
		{
		USB_DATA_DDR = 0x00; //set as input

		CLRBIT(PORTD,PD4);
		asm("nop");
		asm("nop");
		asm("nop");
		data = USB_DATA_PINS;
		SETBIT(PORTD,PD4);

		if (USB_buff_cnt < USB_BUFF_SIZE)
			{
			USB_buff[USB_buff_cnt] = data;
			USB_buff_cnt++;
			}
		else
			USB_buff_cnt = 0;	
		}
	//--------End Read Stage

	} //end while

	return USB_buff_cnt;
} //end USB_read



///////////
//
////////////////////////
void USB_send(unsigned char * s, unsigned int len)
{

unsigned int x;

for (x=0;x<len;x++)
	USB_putc(s[x]);

}



///////////
//
////////////////////////
void USB_puts(char* s)
{

while(*s)
	{
	USB_putc(*s);
	s++;
	}

}//-end USB_puts



///////////
//
////////////////////////
void USB_putc(unsigned char c)
{

USB_DATA_DDR = 0xFF; //set as output



	//-------Write Stage
	while(CHECKBIT(PIND,USB_TXE)==1)
		{
		;
		}

		SETBIT(USB_CTRL_PORT,USB_WR);
		USB_DATA_PORT = c;
		CLRBIT(USB_CTRL_PORT,USB_WR);
}



///////////
//
////////////////////////
char USB_commandMode()
{

	SETBIT(USB_CTRL_PORT,USB_DATAREQ); //hold this high to stay in command mode
	
	USB_setAlarm(200);
	//make sure we're in command mode
	while(!CHECKBIT(USB_CTRL_PIN,USB_DATAACK) )
		{
		if ( !USB_checkAlarm() )
			return 0; //we timed out
		}

	USB_currentMode = COMMANDMODE;
	return 1;
}



///////////
//
////////////////////////
char USB_dataMode()
{

	CLRBIT(USB_CTRL_PORT, USB_DATAREQ); //clear to stay in data mode
	USB_setAlarm(200);	

	//make sure we're in datamode
	while( CHECKBIT(USB_CTRL_PIN,USB_DATAACK) )
		{
		if ( !USB_checkAlarm() )
			return 0; //we timed out
		}

	USB_currentMode = DATAMODE;
	return 1;
}



///////////
//
////////////////////////
void USB_setAlarm(unsigned int timeout_amt)
{
	USB_alarmCounter = timeout_amt;		//reset the counter
	USB_alarmStatus = ALARM_SILENT; //clears the status

	SETBIT(TIFR0,TOV0); //restart the timeout (timer 0)
}



///////////
//
////////////////////////
char USB_checkAlarm()
{	
	return USB_alarmStatus;
}



///////////
//
////////////////////////
void USB_flushChip()
{

USB_setAlarm(250); //??!! Too long/little of a timeout?

	while ( CHECKBIT(PIND,USB_RXF) == 0  )
		{
		if ( !USB_checkAlarm() )
			return; //we timed out

		USB_DATA_DDR = 0x00; //set as input

		CLRBIT(PORTD,PD4); //clock in and ignore the data
		asm("nop");
		asm("nop");
		asm("nop");
		SETBIT(PORTD,PD4);
		}
}



///////////
//
////////////////////////
char USB_queryPort(char the_port,char* response)
{

	if (USB_getMode() != COMMANDMODE)
		if ( !USB_setMode(COMMANDMODE) ) //switch to command mode
			return 0;  //commandMode failed

	USB_flushChip(); /*! Clear any messages waiting */

	if (the_port==PORT1)
		{
		USB_puts("QP1\r");
		}
	else
	if (the_port==PORT2)
		{
		USB_puts("QP2\r");
		}

	USB_read(13,200); //read the data, overshoot the amount of characters

	strncpy(response, (char*)USB_buff,13);


	return 1; //success
}
 


///////////
//
////////////////////////
char USB_selectDevice(char dev_number)
{
char out[7] = "SC "; //first part of the string

	if (dev_number > 7)
		return 0;	//can't choose a device > 7 right now
	
	if (USB_getMode() != COMMANDMODE)
		if ( !USB_setMode(COMMANDMODE) ) //switch to command mode
			return 0;  //commandMode failed

	//USB_flushChip(); /*! don't care about previous data */

	out[4] = dev_number + 48; //convert to ascii
	out[5] = 0x0D;
	out[6] = 0x00;

	//SC command
	USB_puts(out);
	USB_read(5,300); //!!!Need make faster this time out??!!!

	
	return 1; //Success
}



///////////
//
////////////////////////
char USB_setMode(char the_mode)
{
	if (the_mode==COMMANDMODE)
		{
		if ( USB_commandMode() )
			return 1; //success
		}
	else
	if (the_mode==DATAMODE)
		{
		if ( USB_dataMode() )
			return 1; //success
		}

return 0; //failed
}



///////////
//
////////////////////////
char USB_explore(char dev_number, char * response)
{
char out[6];

	if (dev_number > 7)
		return 0; //cannot query more than 8 devices

	if (USB_currentMode != COMMANDMODE)
		if (USB_commandMode() == 0)  //need to be in command mode 
			return 0;	//return if failed

	USB_flushChip(); /*! don't care about previous data */

	//Build the QD command
	strncpy(out, "QD ", 3);
	out[3] = dev_number+48; //convert to an ascii number
	out[4] = 0x0D;
	out[5] = 0x00;

	USB_puts(out); //send the message out

	USB_read(128+5,200);  //read 32 bytes of info, 200mS timeout

	strncpy(response,(char*)USB_buff,128); //copy response into the destination string

	return 1;	//we finally saved all valid info, return 1
}





void USB_init()
{
	//-------------------------
	// USB should be initialized from the bootloader
	// Shouldn't have to re-initialize the chip. (verify this!)
	//
	//
	// The USB uses timer 0 as it's timeout timer
	//
	//-----------------------------


	//----USB timeout timer (Timer-0)
	//Using timer 0, prescale f-osc by 64 = 1ms overflow
	TCCR0B = (0<<CS02) | (1<<CS01) | (1<<CS00);

	SETBIT(TIMSK0,TOIE0); //enable timer overflow interrupt 

	sei(); //enable global interrupts
	
	USB_currentMode = COMMANDMODE; //the current mode on startup


	//Extended Command Set (ECS)
	USB_puts("ECS\r");
	USB_read(5,1000);

	//Input Number in Hex mode (IPA)
	USB_puts("IPA\r");
	USB_read(5,1000);


	//Select Device 0
	//USB_selectDevice(0); 
	USB_puts("SC 0\r");
	USB_read(5,400);

	USB_puts("fbd $1A0000\r");	//set baud to 115200
	USB_read(5,400);


	USB_puts("fmc $00FF\r");
	USB_read(5,400);


	USB_puts("ffc $04\r");
	USB_read(5,400);
	
	USB_dataMode();

} //--end USB_init
