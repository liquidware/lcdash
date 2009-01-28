#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>


#include "megasquirt.h"
#include "ms1_defs.h"
#include "ms1.h"
#include "menu.h"
#include "gui.h"
#include "t6963.h"
#include "LCDash.h"
#include "usb.h"

extern MS_INFO_STRUCT ms_info; 				//!< An externally created structure from megasquirt.c

unsigned char tpsMin=0;
unsigned char tpsMax=0;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
//  			MS1 Extra Wizard
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
///		Wizard 1		coolant sensor
//////////////////////////////////////////////////////////////////
unsigned char menu_1wizard(void)
{
char title[40];

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background

strncpy_P(title,PSTR("Select a Coolant Sensor:"),40);
gui_puts(title,45,10,0);



//!< Making some new buttons here
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



LCD_RECT gm_standard;	//outline of GM Standard
	gm_standard.left = 24;
	gm_standard.top = 34;
	gm_standard.right = 150;
	gm_standard.bottom = gm_standard.top+20;
gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,0); 
lcd_rect(gm_standard,1);


/*
LCD_RECT manual;	//outline of manual
	manual.left = 24;
	manual.top = 94;
	manual.right = 150;
	manual.bottom = manual.top + 20;
gui_puts("Manual",manual.left+8,manual.top+7,0);
lcd_rect(manual,1); 
*/


POINT screen_loc;

/*! Wait for ever in the wizard 1 menu */
while(1)
	{
	
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_SENSORSETUP; 
				}

		else if (pointInRect(screen_loc,gm_standard))
				{
				lcd_fill_rect(gm_standard,0xFF);
				gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,1);
				delay_ms(200);
				lcd_fill_rect(gm_standard,0x00);
				gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,0);
				lcd_rect(gm_standard,1);
				return MENU_2WIZARD;
				}

/*
		else if (pointInRect(screen_loc,manual))
				{
				lcd_fill_rect(manual,0xFF);
				gui_puts("Manual",manual.left+8,manual.top+7,1);
				delay_ms(200);
				lcd_fill_rect(manual,0x00);
				gui_puts("Manual",manual.left+8,manual.top+7,0);
				lcd_rect(manual,1);
				return MENU_2WIZARD;
				}
*/
		
		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 1 wizard


//////////////////////////////////////////////////////////////////
///		Wizard 2  Intake Air Temperature
//////////////////////////////////////////////////////////////////
unsigned char menu_2wizard(void)
{
char out[40];

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background


strncpy_P(out,PSTR("Select an Intake Air Temp Sensor:"),40);
gui_puts(out,1,10,0);



//!< Making some new buttons here
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



LCD_RECT gm_standard;	//outline of GM Standard
	gm_standard.left = 24;
	gm_standard.top = 34;
	gm_standard.right = 150;
	gm_standard.bottom = gm_standard.top+20;
gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,0); 
lcd_rect(gm_standard,1);

/*
LCD_RECT manual;	//outline of manual
	manual.left = 24;
	manual.top = 94;
	manual.right = 150;
	manual.bottom = manual.top + 20;
gui_puts("Manual",manual.left+8,manual.top+7,0);
lcd_rect(manual,1); 
*/


POINT screen_loc;

/*! Wait for ever in the wizard 2 menu */
while(1)
	{
	
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_1WIZARD; 
				}




		else if (pointInRect(screen_loc,gm_standard))
				{
				lcd_fill_rect(gm_standard,0xFF);
				gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,1);
				delay_ms(200);
				lcd_fill_rect(gm_standard,0x00);
				gui_puts("GM Standard",gm_standard.left+8,gm_standard.top+7,0);
				return MENU_3WIZARD;
				}

/*
		else if (pointInRect(screen_loc,manual))
				{
				lcd_fill_rect(manual,0xFF);
				gui_puts("Manual",manual.left+8,manual.top+7,1);
				delay_ms(200);
				lcd_fill_rect(manual,0x00);
				gui_puts("Manual",manual.left+8,manual.top+7,0);
				return MENU_3WIZARD;
				}
*/
		
		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 2 wizard




//////////////////////////////////////////////////////////////////
///		Wizard 3  MAP Sensor
//////////////////////////////////////////////////////////////////
unsigned char menu_3wizard(void)
{
char title[40];


LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background



strncpy_P(title,PSTR("Select a MAP Sensor:"),40);
gui_puts(title,12,8,0); //print the title



//!< Making some new buttons here
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



LCD_RECT bar_standard;	//outline of BAR Standard (2.5 Bar MPX4250 MS STD)
	bar_standard.left = 9;
	bar_standard.top = 24;
	bar_standard.right = 150;
	bar_standard.bottom = bar_standard.top+20;
gui_puts("2.5 Bar MPX4250 MS STD",bar_standard.left+6,bar_standard.top+7,0); 
lcd_rect(bar_standard,1);


LCD_RECT mpx41bar;	//outline of 1.15 Bar MPX4115
	mpx41bar.left = 9;
	mpx41bar.top = 50;
	mpx41bar.right = 150;
	mpx41bar.bottom = mpx41bar.top + 20;
gui_puts("1.15 Bar MPX4115",mpx41bar.left+6,mpx41bar.top+7,0);
lcd_rect(mpx41bar,1); 


LCD_RECT h63a;	//outline of Bar MPXH6300A
	h63a.left = 9;
	h63a.top = 76;
	h63a.right = 150;
	h63a.bottom = h63a.top + 20;
gui_puts("3-Bar MPXH6300A",h63a.left+6,h63a.top+7,0);
lcd_rect(h63a,1); 


LCD_RECT h64a;	//outline of Bar MPXH6400A
	h64a.left = 9;
	h64a.top = 102;
	h64a.right = 150;
	h64a.bottom = h64a.top + 20;
gui_puts("4-Bar MPXH6400A",h64a.left+6,h64a.top+7,0);
lcd_rect(h64a,1); 



POINT screen_loc;

/*! Wait for ever in the wizard 3 menu */
while(1)
	{
	
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_2WIZARD; 
				}



		else if (pointInRect(screen_loc,bar_standard))
				{
				lcd_fill_rect(bar_standard,0xFF);
				gui_puts("2.5 Bar MPX4250 MS STD",bar_standard.left+6,bar_standard.top+7,1);
				delay_ms(200);
				lcd_fill_rect(bar_standard,0x00);
				gui_puts("2.5 Bar MPX4250 MS STD",bar_standard.left+6,bar_standard.top+7,0);
				ms_info.sensors.MAP = MPX4250;
				return MENU_4WIZARD;
				}

		else if (pointInRect(screen_loc,mpx41bar))
				{
				lcd_fill_rect(mpx41bar,0xFF);
				gui_puts("1.15 Bar MPX4115",mpx41bar.left+6,mpx41bar.top+7,1);
				delay_ms(200);
				lcd_fill_rect(mpx41bar,0x00);
				gui_puts("1.15 Bar MPX4115",mpx41bar.left+6,mpx41bar.top+7,0);
				ms_info.sensors.MAP = MPX4115;
				return MENU_4WIZARD;
				}

		else if (pointInRect(screen_loc,h63a))
				{
				lcd_fill_rect(h63a,0xFF);
				gui_puts("3-Bar MPXH6300A",h63a.left+6,h63a.top+7,1);
				delay_ms(200);
				lcd_fill_rect(h63a,0x00);
				gui_puts("3-Bar MPXH6300A",h63a.left+6,h63a.top+7,0);
				ms_info.sensors.MAP = MPXH6300A;
				return MENU_4WIZARD;
				}

		else if (pointInRect(screen_loc,h64a))
				{
				lcd_fill_rect(h64a,0xFF);
				gui_puts("4-Bar MPXH6400A",h64a.left+6,h64a.top+7,1);
				delay_ms(200);
				lcd_fill_rect(h64a,0x00);
				gui_puts("4-Bar MPXH6400A",h64a.left+6,h64a.top+7,0);
				ms_info.sensors.MAP = MPXH6400A;
				return MENU_4WIZARD;
				}

		
		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 3 wizard






//////////////////////////////////////////////////////////////////
///		Wizard 4  OXYGEN SENSOR 1
//////////////////////////////////////////////////////////////////
unsigned char menu_4wizard(void)
{

char title[40];
unsigned char x;

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background

strncpy_P(title,PSTR("Choose"),40);
gui_puts(title,152,8,0);
strncpy_P(title,PSTR("Oxygen Sensor"),40);
gui_puts(title,152,16,0);



//!< Making some new buttons here
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



LCD_RECT O2_rects[O2_SENSOR_CNT];
char * O2_labels[O2_SENSOR_CNT] = {
				PSTR("Narrow Band"),
				PSTR("WB_1_0_LINEAR"),
				PSTR("DYNOJET_LINEAR"),
				PSTR("TECHEDGE_LINEAR"),
				PSTR("INNOVATE 1-2 Linear"),
				PSTR("INNOVATE 0-5 Linear"),
				PSTR("INNOVATE LC1 Default"),
				PSTR("AEM Linear"),
				PSTR("No O2 Sensor"),
				};

unsigned char O2_ids[] = {
			NARROW_BAND,		
			WB_1_0_LINEAR,			
			DYNOJET_LINEAR,			
			TECHEDGE_LINEAR,			
			INNOVATE_1_2_LINEAR,		
			INNOVATE_0_5_LINEAR,		
			INNOVATE_LC1_DEFAULT,	
			AEM_LINEAR,				
			NoEGO,					
			};

char out[40];

for(x=0; x<O2_SENSOR_CNT;x++)
{
O2_rects[x].left = 9;
O2_rects[x].right = 150;
O2_rects[x].top = (x*14)+2;
O2_rects[x].bottom = O2_rects[x].top +11;

lcd_rect(O2_rects[x],1);

strncpy_P(out,O2_labels[x],40);
gui_puts(out,9,O2_rects[x].top+3,0);

}





POINT screen_loc;

/*! Wait for ever in the wizard 4 menu */
while(1)
	{
	
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_3WIZARD; 
				}

		//check the item lists
		for (x=0; x<O2_SENSOR_CNT; x++)
		{
		
		if (pointInRect(screen_loc,O2_rects[x]) )
			{
				lcd_fill_rect(O2_rects[x],0xFF);
				delay_ms(200);
				ms_info.sensors.EGO = O2_ids[x];
				return MENU_5WIZARD;
			}
		}

		

		
		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 4 wizard






//////////////////////////////////////////////////////////////////
///		Wizard 5  Throttle Pos
//////////////////////////////////////////////////////////////////
unsigned char menu_5wizard(void)
{

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background

gui_puts("Calibrate the Throttle Position:",12,8,0);
gui_puts("Fully close the throttle",12,20,0);
gui_puts("and press the 'NEXT' button.",12,29,0);


//!< Making some new buttons here
LCD_RECT next;		//next is in the bottom right button of the menu screen
	next.left = 166;
	next.top = 55;
	next.right = 230;
	next.bottom = 84;
gui_puts("Next",next.left+19,next.top+11,0); 
lcd_rect(next,1); 


LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



unsigned char * buff=USB_getBuff();
char out[4];

POINT screen_loc;

/*! Wait for ever in the wizard 5 */
while(1)
	{
	ms_getSensors(); //fill the USB receive buffer with our MS response

	dtostrf(buff[ms1_TPSADC],3,0,out);
	gui_largeNumS(out,20,60,0);
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_4WIZARD; 
				}

		else if (pointInRect(screen_loc,next))
				{
				tpsMin = buff[ms1_TPSADC];
				lcd_fill_rect(next,0xFF);
				gui_puts("Next",next.left+19,next.top+11,1);
				delay_ms(200);
				lcd_fill_rect(next,0x00);
				gui_puts("Next",next.left+19,next.top+11,0); 
				return MENU_6WIZARD;
				}




		
		
		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 5 wizard




//////////////////////////////////////////////////////////////////
///		Wizard 6  Throttle_pos
//////////////////////////////////////////////////////////////////
unsigned char menu_6wizard(void)
{

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background


gui_puts("Calibrate the Throttle Position:",12,8,0);
gui_puts("Fully open the throttle",12,20,0);
gui_puts("and press the 'NEXT' button.",12,29,0);



//!< Making some new buttons here
LCD_RECT next;		//next is in the bottom right button of the menu screen
	next.left = 166;
	next.top = 55;
	next.right = 230;
	next.bottom = 84;
gui_puts("Next",next.left+19,next.top+11,0); 
lcd_rect(next,1); 


LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 166;
	back.top = 90;
	back.right = 230;
	back.bottom = 119;
gui_puts("Back",back.left+19,back.top+11,0); 
lcd_rect(back,1);



LCD_RECT saving;		//back is in the bottom right button of the menu screen
	saving.left = 30;
	saving.top = 50;
	saving.right = saving.left + 184;
	saving.bottom = saving.top+29;

unsigned char * buff=USB_getBuff();
char out[4];
POINT screen_loc;

/*! Wait for ever in the wizard 6 menu */
while(1)
	{
	ms_getSensors(); //fill the USB receive buffer with our MS response
	dtostrf(buff[ms1_TPSADC],3,0,out);
	gui_largeNumS(out,20,60,0);
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,back))
				{
				lcd_fill_rect(back,0xFF);
				gui_puts("Back",back.left+19,back.top+11,1);
				delay_ms(200);
				lcd_fill_rect(back,0x00);
				gui_puts("Back",back.left+19,back.top+11,0);
				return MENU_5WIZARD; 
				}

		else if (pointInRect(screen_loc,next))
				{
				lcd_fill_rect(next,0xFF);
				gui_puts("Next",next.left+19,next.top+11,1);
				delay_ms(200);
				lcd_fill_rect(next,0x00);
				gui_puts("Next",next.left+19,next.top+11,0);
				lcd_rect(next,1); 

				lcd_fill_rect(saving,0xFF); //!< Erase the background
			//	lcd_rect(saving,1);

				tpsMax = buff[ms1_TPSADC];

				gui_puts("Saving MS1 Settings",saving.left+35,saving.top+11,1);

				ms1_tpsGen(tpsMin,tpsMax); //save the TPS table
				ms_storeMSsensors();			//save the other stuff
				delay_ms(250);
				delay_ms(250);
				return EXIT_MENU;
				}



		
		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu 6 wizard
