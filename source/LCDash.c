/*! \file LCDash.c 
			\brief The "main" file of the LCDash project
*/
/*************************************************************************
 **************************************************************************
 **   LCDash  - 2 0 0 7
 **
 **   (C) 2007 - C. Ladden and M Gionfriddo
 **
 **   This header must appear on all derivatives of this code.
 **
 ***************************************************************************
 **************************************************************************/


#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>

#include "t6963.h"
#include "bitops.h"
#include "touchscreen.h"
#include "gui.h"
#include "LCDash.h"
#include "usb.h"
#include "FT232R.h"
#include "gauge.h"
#include "ms/megasquirt.h"
#include "messageBar.h"
#include "menu.h"
#include "speedo.h"
#include "obd.h"
#include "mut.h"
#include "datalogging.h"


//eeprom variables
EEMEM unsigned char ee_brightness;
EEMEM unsigned char ee_brightness_saved;
EEMEM unsigned char ee_isCalibrated;


//Global Variables
LCD_RECT main_menu_rect; /*!< The main menu area, pressable any time we're displaying gauges */
unsigned char dualDisplay_status = OFF;
unsigned char LCDash_currentProtocol=0; 

float f;



void dualDisplay_off()
{
USB_commandMode();

USB_puts("SC 0\r"); //select the main FT232R
USB_read(5,500);

USB_dataMode();

dualDisplay_status = OFF;
}


unsigned char dualDisplay_check()
{
	if (dualDisplay_status == ON)
		return TRUE;
	else
		return FALSE;
}

void dualDisplay_init()
{
USB_commandMode();
//char * buff = (char*)USB_getBuff();

USB_puts("SC 1\r"); //select the other FT232R
USB_read(5,400);

	USB_puts("fmc $00FF\r");
	USB_read(5,400);

	USB_puts("ffc $04\r");
	USB_read(5,400);

	FT232R_setBaud(B115200);
USB_commandMode();

USB_puts("SC 0\r"); //select the main FT232R
USB_read(5,500);

USB_dataMode();

dualDisplay_status = ON;
}


void USB_selectZero()
{
	USB_commandMode();
	while(USB_read(1,1)); //Flush
	USB_puts("SC 0\r"); //select the main FT232R
	USB_read(5,200);
	
	unsigned char * buff = USB_getBuff();
		while(1)
		{
		USB_puts("E\r"); USB_read(1,1);
		if (buff[0] == 'E')
			{
			while(USB_read(1,1)); //clear the buffer
			break;
			}
		}
	
	USB_dataMode();
}

void USB_selectOne()
{
	USB_commandMode();
	while(USB_read(1,1)); //Flush
	USB_puts("SC 1\r"); //select the other FT232R
	USB_read(5,200);
	
		unsigned char * buff = USB_getBuff();
		while(1)
		{
		USB_puts("E\r"); USB_read(1,1);
		if (buff[0] == 'E')
			{
			while(USB_read(1,1)); //clear the buffer
			break;
			}
		}
	
	USB_dataMode();
}


void dualDisplay_send(unsigned char * buff,unsigned char len)
{
unsigned char temp_usbBuff[200];
unsigned char x,counter=0;

	memcpy(&temp_usbBuff[0],(char*)buff,len); //save the previous buffer
	
	USB_selectOne();

	for(x=0;x<len;x++)
		{
		counter++;
		USB_putc(temp_usbBuff[x]);
		if (counter==2)
			{
			delay_ms(1);
			counter =0;
			}
		}

	USB_selectZero();


	memcpy((char*)buff,&temp_usbBuff[0],len); //restore the previous buffer
}





unsigned char LCDash_descrProtocol()
{

return LCDash_currentProtocol;

}

void LCDash_setProtocol(unsigned char protocol)
{
	LCDash_currentProtocol = protocol;
}



//Main Entry point here
int main()
{

	sei(); //enable global interrupts
	
	lcd_init();   
	touch_init(); 

	USB_init(); //automatically selecting USB device 1 whatever that may be (usually a USB-Serial dongle)
	
	//dualDisplay_init();////////////!!!!!!testing

	LCDash_init(); //doing lcdash init stuff here
	messageBar_init();

//Find and INIT the protocols
	ms_init();
	ms_getSensors();
	ms_getSensors();
	while(USB_read(1,10)){;}

	messageBar_addMsg("Searching MS");

	if (ms_isConnected())
		{
		messageBar_addMsg("Found MS ECU");
		LCDash_setProtocol(PROTOCOL_MEGASQUIRT); //set the protocol now to megasquirt
		lcd_clear_graph();
		}
	else
		{
		messageBar_addMsg("Searching OBD");
		if ( obd_init()	)
			{
			messageBar_addMsg("Found OBD ECU");
			LCDash_setProtocol(PROTOCOL_OBD);	//set the protocol now to OBD
			lcd_clear_graph();
			}
		else
			{
			messageBar_addMsg("Searching MUT");
			if ( mut_init() )
				{
		
				messageBar_addMsg("Found MUT ECU");
				LCDash_setProtocol(PROTOCOL_MUT);	//set the protocol now to MUT
				lcd_clear_graph();
				}
			else
				{
				LCDash_setProtocol(PROTOCOL_NONE);
				lcd_clear_graph();
				messageBar_addMsg("No ECU Found");
				}
			
			}
			

		}


LCDash_setProtocol(PROTOCOL_MEGASQUIRT);//!!test

//	speedo_init(); //init the speedometer input capture
	gauge_restoreLayout(); //restore the last layout
	gauge_drawGaugeLayout(); //draw the gauges


	POINT screen_loc; //point for our screen press
	GAUGE ** gauge_list;
	unsigned char gauge_count;
	
	gauge_list = gauge_getGaugePointers(&gauge_count); //get the active gauges
	//datalog_init(gauge_list,gauge_count);

//--The main while loop!
	while (1) 
	{

		gauge_list = gauge_getGaugePointers(&gauge_count); //get the active gauges



//--Populate our gauges depending on active protocol

		if ( LCDash_descrProtocol() == PROTOCOL_MEGASQUIRT )
			{
			ms_getSensors(); //fill the USB receive buffer with our MS response
			ms_convertSensors(&gauge_list[0], gauge_count); //use all the param_id's to find/convert thier values from the USB receive buffer
			}
		else


		if ( LCDash_descrProtocol() == PROTOCOL_OBD )
			{
			obd_convertSensors(&gauge_list[0], gauge_count);
			if (obd_checkStatus() == OBD_NOECU)
				{
				messageBar_addMsg("OBD ECU not present");
				}
			else
				{
				messageBar_addMsg("OBD ECU present");
				}
			}
		else


		if ( LCDash_descrProtocol() == PROTOCOL_MUT )
			{
			mut_convertSensors(&gauge_list[0], gauge_count);
			
			}
		else
			{
			//no protocol found
			} 



if ( LCDash_descrProtocol() != PROTOCOL_NONE)
	{
		gauge_refreshGaugeLayout(); //refresh all the current gauges
		gauge_checkAlarms();		//check for alarms going off
	}


//-------------------------------



		///Begin Main Menu Check//////////////////////////////////////////////////
		if (touch_get_cursor(&screen_loc))
		{
	
			if (pointInRect(screen_loc,main_menu_rect))
				{
			
			
				unsigned char users_menu_choice = MENU_MAIN; //setup for the first menu
				unsigned char result = 0;

				while(1)
					{
					result = handle_menu(users_menu_choice);

					users_menu_choice = result;

					if (users_menu_choice == EXIT_MENU)
						{
						//--clean up and repaint the home screen
						lcd_clear_graph(); 
						gauge_drawGaugeLayout();
						break;
						}
					}
				
				}
		}
		////End Main Menu Check////////////////////////////////////////////////////////

	} //end while



	return 0; //Never should get here.

} // end of main 




/////////////////////////////////////////
//
//
/////////////////////////////////////////
void LCDash_init()
{

	// Setup the press region for the main menu 
	main_menu_rect.left = 0;
	main_menu_rect.top = 0;
	main_menu_rect.right = 239;
	main_menu_rect.bottom = 127;
	
	/*	1. Draw main background
		2. Load previous settings
		3. Restore gauges
		4. Query USB ports
	*/
	
//////////////////////////////
//Restore brightness PWM value
backlightRestore();

}



float backlightChange(char amount)
{
float b;
unsigned char oc;

if (  (((unsigned char)OCR1A+amount) < 210) && (((unsigned char)OCR1A+amount)>20) )
	{
	OCR1A += amount;
	}

oc = 160 - ((unsigned char )OCR1A-31);
b = (float)oc / 160.0 *100;

return b;

}



void backlightStore()
{
	eeprom_write_byte(&ee_brightness,(unsigned char)OCR1A); //set the duty cycle in eeprom (8bit)
	eeprom_write_byte(&ee_brightness_saved,BRIGHTNESS_SAVED);//write to eeprom that brightness was saved
}



void backlightRestore()
{

if (eeprom_read_byte(&ee_brightness_saved) == BRIGHTNESS_SAVED)
	{
	unsigned char temp = eeprom_read_byte(&ee_brightness);
	if ((temp>20) && (temp < 210))
		{
		OCR1A = temp; //in bounds, so save it
		}
	else
		{
		OCR1A = 200;
		eeprom_write_byte(&ee_brightness,OCR1A); //set the duty cycle in eeprom (8bit)
		}
	}

}






/////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////
char pointInRect(POINT p, LCD_RECT r)
{
if (p.x >= r.left)
	if	(p.x <= r.right)
		if (p.y >= r.top)
			if (p.y <= r.bottom)
			return 1;
			
return 0;
}






