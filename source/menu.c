/*! \file menu.c 
			\brief The "menu" file of the LCDash project
*/
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>


#include "bitops.h"
#include "t6963.h"
#include "touchscreen.h"
#include "gui.h"
#include "bitmaps.h"
#include "LCDash.h"
#include "usb.h"
#include "FT232R.h"
#include "ms/megasquirt.h"
#include "menu.h"
#include "gauge.h"
#include "ms1_wizard.h"
#include "speedo.h"

extern GAUGE * gauge_PTR_array[5]; //an array of pointers pointing to our 'gauge' structure from each GUI gauge
extern unsigned char USB_buff[USB_BUFF_SIZE]; /*!< The actual USB buffer */

/////////////////////////////////////////
//
//Handle the menu syatem
/////////////////////////////////////////
unsigned char handle_menu(unsigned char menu_id)
{

if (menu_id==MENU_MAIN)
	{
	//do something
	return menu_main();
	}
else
if (menu_id==MENU_SYSTEM_INFO)
	{
	return menu_system_info();
	}
else
if (menu_id==EXIT_MENU)
	{
	return EXIT_MENU;
	}
else
if (menu_id==MENU_SYSTEM_SETTINGS)
	{
	return menu_system_settings();
	}
else
if (menu_id==MENU_BRIGHTNESS)
	{
	return menu_brightness();
	}
else
if (menu_id==MENU_GAUGE_SETTINGS)
	{
	return menu_gauge_settings();
	}
else
if (menu_id==MENU_SELECT_LAYOUT)
	{
	return menu_selectLayout();
	}
else
if (menu_id==MENU_1WIZARD)
	{
	return menu_1wizard();
	}
else
if (menu_id==MENU_2WIZARD)
	{
	return menu_2wizard();
	}
else
if (menu_id==MENU_3WIZARD)
	{
	return menu_3wizard();
	}
else
if (menu_id==MENU_4WIZARD)
	{
	return menu_4wizard();
	}
else
if (menu_id==MENU_5WIZARD)
	{
	return menu_5wizard();
	}
else
if (menu_id==MENU_6WIZARD)
	{
	return menu_6wizard();
	}
else
if (menu_id==MENU_SENSORSETUP)
	{
	return menu_sensorsetup();
	}
else
if (menu_id==MENU_CALIBRATE)
	{
	return menu_calibrate();
	}
else
if (menu_id==MENU_CALIBRATEVSS)
	{
	return menu_calibratevss();
	}
else
if (menu_id==MENU_TUNING)
	{
	return menu_tuning();
	}
else
if (menu_id==MENU_EDITTABLE)
	{
	return menu_editTable();
	}



return EXIT_MENU;
}





void menu_selectGaugeLayout()
{

//Select the gauge layout style (4-5 icons)
//or
//Show me the gauge layout styles (Visually show them their choices)


gauge_drawGaugeLayout(LAYOUT_STANDARD);

//style one,   Analog/Digital Mix, Standard Setup

//style two,   Major Analog, 2 Analog Gauges

//style three, Large Digital Gauges, 4 Gauges

//style four,  Scope Mode

}






/////////////////////////////////////////
/*!
   Begin the main menu.
*/
/////////////////////////////////////////
unsigned char menu_main(void)
{


//!< Making some new buttons here
unsigned char width_gauge_settings = 80;
unsigned char height_gauge_settings = 49;
LCD_RECT gauge_settings;	//gauge settings top left button
	gauge_settings.left = 32;
	gauge_settings.top = 11;
	gauge_settings.right = gauge_settings.left + width_gauge_settings;
	gauge_settings.bottom = gauge_settings.top + height_gauge_settings;

unsigned char width_TuningIcon = 80;
unsigned char height_TuningIcon = 49;
LCD_RECT TuningIcon;   //TuningIcon bottom left button
	TuningIcon.left = 32;
	TuningIcon.top = 65;
	TuningIcon.right = TuningIcon.left + width_TuningIcon;
	TuningIcon.bottom = TuningIcon.top + height_TuningIcon;

unsigned char width_system_settings = 96;
unsigned char height_system_settings = 49;
LCD_RECT system_settings;		//System Settings top right button
	system_settings.left = 120;
	system_settings.top = 11;
	system_settings.right = system_settings.left + width_system_settings;
	system_settings.bottom = system_settings.top + height_system_settings;

unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 136;
	exit.top = 75;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 7;
	main_outline.top = 4;
	main_outline.right = 232;
	main_outline.bottom = 122;


lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 


	
	//	Draw the main menu icns
	//	The Icons are drawn all at the same time (after the buttons are setup).  This is done so the LCD screen doesn't appear slow.
	
	gui_drawBMP(ExitButtonIcon, exit.left, exit.top,width_exit, height_exit, 0); // EXIT the menu icon
	
	gui_drawBMP(SystemSettingsIcon2,system_settings.left,system_settings.top,width_system_settings,height_system_settings,0); // system settings icon
	
	gui_drawBMP(GaugeSettingsIcon2,gauge_settings.left,gauge_settings.top,width_gauge_settings,height_gauge_settings,0);  // guage settings icon
		
	gui_drawBMP(TuningIcon2,TuningIcon.left,TuningIcon.top,width_TuningIcon,height_TuningIcon,0);  //TuningIcon icon


POINT screen_loc;

/*! Wait for ever in the main menu */
while(1)
	{
	
	
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon, exit.left, exit.top,width_exit, height_exit,1); // EXIT the menu icon inverted
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon, exit.left, exit.top,width_exit, height_exit,0); // EXIT the menu icon

				return EXIT_MENU; 
				}

		else if (pointInRect(screen_loc,system_settings))
				{
				gui_drawBMP(SystemSettingsIcon2,system_settings.left,system_settings.top,width_system_settings,height_system_settings,1); // system settings the menu icon inverted
				delay_ms(200);
				gui_drawBMP(SystemSettingsIcon2,system_settings.left,system_settings.top,width_system_settings,height_system_settings,0); // system settings the menu icon 

				return MENU_SYSTEM_SETTINGS;
				}

		else if (pointInRect(screen_loc,TuningIcon))
				{				
				gui_drawBMP(TuningIcon2,TuningIcon.left,TuningIcon.top,width_TuningIcon,height_TuningIcon,1);  //TuningIcon icon icon inverted
				delay_ms(200);
				gui_drawBMP(TuningIcon2,TuningIcon.left,TuningIcon.top,width_TuningIcon,height_TuningIcon,0);  //TuningIcon icon

				return MENU_TUNING;
				}

		else if (pointInRect(screen_loc, gauge_settings))
				{
				gui_drawBMP(GaugeSettingsIcon2,gauge_settings.left,gauge_settings.top,width_gauge_settings,height_gauge_settings,1); // guage settings icon inverted
				delay_ms(200);
				gui_drawBMP(GaugeSettingsIcon2,gauge_settings.left,gauge_settings.top,width_gauge_settings,height_gauge_settings,0); // guage settings icon

				return MENU_GAUGE_SETTINGS;
				}
		
		} //end if touch_get_cursor
	
	} // end while 	

//do main menu clean up here...


} // end of main menu 



/////////////////////////////////////////
/*!
   Begin the System Information menu.
*/
/////////////////////////////////////////
unsigned char menu_system_info()
{

LCD_RECT	menu_outline;


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 152;
	exit.top = 78;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	

unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 152;
	back.top = 44;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;
//-----End making new buttons


			// Setup the press region for the first sub menu 
	menu_outline.left = 7;
	menu_outline.top = 4;
	menu_outline.right = 232;
	menu_outline.bottom = 122;


lcd_fill_rect(menu_outline,0x00); //!< Erase the background
lcd_rect(menu_outline,1);  //!< Draw the main menu outline rectangle 

char out[MENU_FIRMWARE_MAX_SIZE];


//	Draw the main menu icns
	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(SystemInformation2,32,17,176,26,0);//system info bannar	

///////////////////////////////////
	gui_puts("MegaSquirt Firmware:",22,49,0);
	ms_getFirmwareString(out);  			//get the ms firmware version
	out[MENU_FIRMWARE_MAX_SIZE-1]=0x00;		//terminate the string just in case
		gui_puts(out,26,57,0);		 //print it
//////////////////////////////////////////

/////////////////////////////////////////////
	gui_puts("LCDash Firmware:",22,69,0);	
	strncpy_P(out,LCDASH_VERSION,30);		//lcdash firmware version
		gui_puts(out,26,77,0);		 //print it
//////////////////////////////////////////////
	
///////////////////////////////////////////////
	gui_puts("VNC1L Firmware:",22,89,0);
	strncpy_P(out,VDAF_VERSION,30);		//VDAF firmware version
		gui_puts(out,26,97,0);		//print it
//////////////////////////////////////////////////

POINT screen_loc;
	
/*! Wait for ever in the information menu */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc, exit))
			{
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
			delay_ms(200);
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon


			return EXIT_MENU;
			}

		else if (pointInRect(screen_loc, back))
			{
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
			delay_ms(200);
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon

			return MENU_SYSTEM_SETTINGS;
			}

		} //end if touch_get_cursor()

	} //end while

} //end menu_system_info()



/////////////////////////////////////////
/*!
   Begin the System Settings menu.
*/
/////////////////////////////////////////
unsigned char menu_system_settings()
{

LCD_RECT	menu_outline;




//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 152;
	exit.top = 78;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	

unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 152;
	back.top = 44;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;

unsigned char width_brightness = 40;
unsigned char height_brightness = 29;
LCD_RECT brightness;		// backlight brightness
	brightness.left = 32;
	brightness.top = 44;
	brightness.right = brightness.left + width_brightness;
	brightness.bottom = brightness.top + height_brightness;

unsigned char width_SystemInformation = 40;
unsigned char height_SystemInformation = 29;
LCD_RECT SystemInformation;		// system information
	SystemInformation.left = 96;
	SystemInformation.top = 78;
	SystemInformation.right = SystemInformation.left + width_SystemInformation;
	SystemInformation.bottom = SystemInformation.top + height_SystemInformation;


//unsigned char width_defaultsettings = 56;
//unsigned char height_defaultsettings = 35;
//LCD_RECT defaultsettings;		// default settings button
//	defaultsettings.left = 24;
//	defaultsettings.top = 44;
//	defaultsettings.right = defaultsettings.left + width_defaultsettings;
//	defaultsettings.bottom = defaultsettings.top + height_defaultsettings;



unsigned char width_calibratets = 40;
unsigned char height_calibratets = 29; 
LCD_RECT calibratets;		// calibrate ts button
	calibratets.left = 96;
	calibratets.top = 44;
	calibratets.right = calibratets.left + width_calibratets;
	calibratets.bottom = calibratets.top + height_calibratets;


unsigned char width_dual = 40;
unsigned char height_dual = 29; 
LCD_RECT dual;		// dual display menu icon
	dual.left = 32;
	dual.top = 78;
	dual.right = dual.left + width_dual;
	dual.bottom = dual.top + height_dual;


//-----End making new buttons

			// Setup the press region for the first sub menu 
	menu_outline.left = 20;
	menu_outline.top = 13; 
	menu_outline.right = 218;
	menu_outline.bottom = 113;

lcd_fill_rect(menu_outline,0x00); //!< Erase the background
lcd_rect(menu_outline,1);  //!< Draw the main menu outline rectangle 



//	Draw the system settings menu icns
	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(SystemSettingsBrightness4,brightness.left, brightness.top,width_brightness, height_brightness,0);
//	gui_drawBMP(SystemSettingsDefault3,defaultsettings.left,defaultsettings.top,width_defaultsettings,height_defaultsettings,0);	//on hold until later time
	gui_drawBMP(SystemSettingsCalibrate_ts5,calibratets.left,calibratets.top,width_calibratets,height_calibratets,0);
	gui_drawBMP(systemsettingsdualdisplay,dual.left,dual.top,width_dual,height_dual,0); //dual display menu icon
	gui_drawBMP(SystemInformation3,SystemInformation.left,SystemInformation.top,width_SystemInformation,height_SystemInformation,0); //SystemInformation3 menu icon
	gui_drawBMP(SystemSettings2,32,17,160,20,0);//system settings bannar

//dualdisplay stuff
if (dualDisplay_check() == TRUE)
	gui_puts("On ",45,94,0);
else
	gui_puts("Off",41,94,0);


POINT screen_loc;
	
/*! Wait for ever in the information menu */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc, exit))
			{
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
			delay_ms(200);
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon


			return EXIT_MENU;
			}

		else if (pointInRect(screen_loc, back))
			{
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
			delay_ms(200);
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon

			return MENU_MAIN;
			}


		else if (pointInRect(screen_loc, calibratets))
			{
			gui_drawBMP(SystemSettingsCalibrate_ts5,calibratets.left,calibratets.top,width_calibratets,height_calibratets,1); // calibratets menu icon
			delay_ms(200);
			gui_drawBMP(SystemSettingsCalibrate_ts5,calibratets.left,calibratets.top,width_calibratets,height_calibratets,0); // calibratets menu icon
			
			lcd_clear_graph();
			lcd_clear_text();
			eeprom_write_byte(&ee_isCalibrated, 9);//clear the eeprom flag of touch calibrated
			touch_init();//initialize the touch screen
			return MENU_SYSTEM_SETTINGS;
			}	
			
			
		else if (pointInRect(screen_loc, brightness))
			{
			gui_drawBMP(SystemSettingsBrightness4,brightness.left, brightness.top,width_brightness, height_brightness,1); // calibratets menu icon
			delay_ms(200);
			gui_drawBMP(SystemSettingsBrightness4,brightness.left, brightness.top,width_brightness, height_brightness,0); // calibratets menu icon
			
			
			return MENU_BRIGHTNESS;			
			}
			
		else if (pointInRect(screen_loc, SystemInformation))
			{
			gui_drawBMP(SystemInformation3,SystemInformation.left,SystemInformation.top,width_SystemInformation,height_SystemInformation,1); //SystemInformation3 menu icon
			delay_ms(200);
			gui_drawBMP(SystemInformation3,SystemInformation.left,SystemInformation.top,width_SystemInformation,height_SystemInformation,0); //SystemInformation3 menu icon
			
			return MENU_SYSTEM_INFO;			
			}

		else if (pointInRect(screen_loc, dual))
			{
			gui_drawBMP(systemsettingsdualdisplay,dual.left,dual.top,width_dual,height_dual,1); // dual display menu icon
			delay_ms(200);
			gui_drawBMP(systemsettingsdualdisplay,dual.left,dual.top,width_dual,height_dual,0); // dual display menu icon
			
			

			if (dualDisplay_check() == ON)
				{
				dualDisplay_off();
				}
			else
				{
				dualDisplay_init();
				}

			return MENU_SYSTEM_SETTINGS;			
			}
					
			

		} //end if touch_get_cursor()

	} //end while

} //end menu_system_settings()



/////////////////////////////////////////
/*!
   Begin the Brightness Settings menu.
*/
/////////////////////////////////////////
unsigned char menu_brightness()
{

LCD_RECT	menu_outline;


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 152;
	exit.top = 78;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	

unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 152;
	back.top = 44;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;

unsigned char width_up = 32;
unsigned char height_up = 31;
LCD_RECT up;		//up button
	up.left = 40;
	up.top = 36;
	up.right = up.left + width_up;
	up.bottom = up.top + height_up;

unsigned char width_down = 32;
unsigned char height_down = 31;
LCD_RECT down;		//down button
	down.left = 40;
	down.top = 74;
	down.right = down.left + width_down;
	down.bottom = down.top + height_down;



// Setup the press region for the first sub menu 
	menu_outline.left = 20;
	menu_outline.top = 13; 
	menu_outline.right = 218;
	menu_outline.bottom = 113;

lcd_fill_rect(menu_outline,0x00); //!< Erase the background
lcd_rect(menu_outline,1);  //!< Draw the main menu outline rectangle 



//	Draw the brightness settings menu icns
	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,0);//up arrow
	gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,0); //down arrow
	gui_drawBMP(brightnessbannaricon,72,18,24,19,0);// brightness bannar icon
	gui_puts("Brightness",95,24,0);

char out[6];
dtostrf(backlightChange(0),3,0,out);
gui_largeNumS(out,80,60,0);
POINT screen_loc;
	
/*! Wait for ever in the information menu */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc, exit))
			{
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
			delay_ms(200);
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
			backlightStore();
			return EXIT_MENU;
			}

		else if (pointInRect(screen_loc, back))
			{
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
			delay_ms(200);
			gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
			backlightStore();
			return MENU_SYSTEM_SETTINGS;
			}

		else if (pointInRect(screen_loc, up))
			{
			gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,1); // Back the menu icon
			delay_ms(150);
			gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,0); // Back the menu icon
			dtostrf(backlightChange(-20),3,0,out);
			gui_largeNumS(out,80,60,0);
			}
			
		else if (pointInRect(screen_loc, down))
			{
			gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,1); // down icon
			delay_ms(150);
			gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,0); // down icon
			dtostrf(backlightChange(20),3,0,out);
			gui_largeNumS(out,80,60,0);	
			}
			
			
			
					
			

		} //end if touch_get_cursor()

	} //end while

} //end menu_brightness()











//////////////////////////////////////////////////////////////////
//
//Ask them if they want to change the parameter or the Min/Max
//
///////////////////////////////////////////////////////////////////

unsigned char menu_modify_gauge(char * description, GAUGE * g, unsigned char * alarm)
{
	char user_units_desc[10];
	char units_desc[10];
	unsigned char otherUnits = gauge_getOtherUnits(g->user_units);
	unsigned char user_units = g->user_units;

	gauge_getUnitsDescription(user_units_desc,	user_units);
	gauge_getUnitsDescription(units_desc,otherUnits);

	//!< Making some new buttons here
	unsigned char width_exit = 64;
	unsigned char height_exit = 29;
	LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
		exit.left = 168;
		exit.top = 91;
		exit.right = exit.left + width_exit;
		exit.bottom = exit.top + height_exit;
	
	unsigned char width_back = 64;
	unsigned char height_back = 29;
	LCD_RECT back;		//back is in the bottom right button of the menu screen
		back.left = 168;
		back.top = 55;
		back.right = back.left + width_back;
		back.bottom = back.top + height_back;
	
	LCD_RECT alarmTog;
		alarmTog.left = 180;
		alarmTog.top = 10;
		alarmTog.right = 235;
		alarmTog.bottom = 40;

	

	// Setup the press region for the first sub menu 
		LCD_RECT main_outline;	//outline of main menu
		main_outline.left = 0;
		main_outline.top = 0;
		main_outline.right = 239;
		main_outline.bottom = 127;
	lcd_fill_rect(main_outline,0x00); //!< Erase the background
	lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 
		
		lcd_rect_diffSize(alarmTog,1,1); //alarm toggle button
		
		if (*alarm == ON)
			{
			lcd_fill_rect(alarmTog,0xFF);
			gui_puts("Alarm is",alarmTog.left+1,alarmTog.top+8,1);
			gui_puts("ON",alarmTog.left+12,alarmTog.top+16,1);
			}
		else
			{
			gui_puts("Alarm is",alarmTog.left+1,alarmTog.top+8,0);
			gui_puts("OFF",alarmTog.left+12,alarmTog.top+16,0);
			}

//Display the Gauge decription
	gui_puts("Gauge:",2,8,0); 
	gui_puts(description,40,8,0); //display the gauge description


	//	Draw the main menu icns
		gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
		gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon


	//Draw some buttons
		LCD_RECT change_parameter_bounds,change_minMax_bounds,change_metric_bounds,change_standard_bounds;	

	change_parameter_bounds.left = 4;		change_parameter_bounds.right = 150;
	change_minMax_bounds.left = 4;			change_minMax_bounds.right = 150;
	change_metric_bounds.left = 45;			change_metric_bounds.right = 91;
	change_standard_bounds.left = 95;		change_standard_bounds.right = 150;

	change_parameter_bounds.top = 38;		change_parameter_bounds.bottom = 61;
	change_minMax_bounds.top = 68;			change_minMax_bounds.bottom = 91;
	change_metric_bounds.top = 98;			change_metric_bounds.bottom = 121;
	change_standard_bounds.top = 98;		change_standard_bounds.bottom = 121;

	
	lcd_rect(change_parameter_bounds,1); 	
  	lcd_rect(change_minMax_bounds,1); 
	lcd_rect(change_metric_bounds,1); 	
  	lcd_rect(change_standard_bounds,1); 

	lcd_fill_rect(change_metric_bounds,0xFF);


	gui_puts("Change Parameter",change_parameter_bounds.left+8,(change_parameter_bounds.bottom-change_parameter_bounds.top)/2 + change_parameter_bounds.top-4,0);//change_parameter

	gui_puts("Change Min/Max",change_minMax_bounds.left+8,(change_minMax_bounds.bottom-change_minMax_bounds.top)/2 + change_minMax_bounds.top-4,0);//change_minMax

	gui_puts(user_units_desc,change_metric_bounds.left+3,(change_metric_bounds.bottom-change_metric_bounds.top)/2 + change_metric_bounds.top-4,1);//change_metric

	gui_puts(units_desc,change_standard_bounds.left+3,(change_standard_bounds.bottom-change_standard_bounds.top)/2 + change_standard_bounds.top-4,0);//change_standard

	gui_puts("Units:",4, 106,0);//Units:


POINT screen_loc; //our touched location
	
/*! Wait for ever until a valid press */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_GAUGE_SETTINGS;
				}

//Check the buttons
		else if (pointInRect(screen_loc, alarmTog))
				{
				if (*alarm == ON)
					{
					lcd_fill_rect(alarmTog,0x00);
					gui_puts("Alarm is",alarmTog.left+1,alarmTog.top+8,0);
					gui_puts("OFF",alarmTog.left+12,alarmTog.top+16,0);
					*alarm = OFF;
					}
				else
					{
					lcd_fill_rect(alarmTog,0xFF);
					gui_puts("Alarm is",alarmTog.left+1,alarmTog.top+8,1);
					gui_puts("ON",alarmTog.left+12,alarmTog.top+16,1);
					*alarm = ON;
					}
				}
		
		else if (pointInRect(screen_loc, change_parameter_bounds))
				{
				lcd_fill_rect(change_parameter_bounds,0xFF);
				gui_puts("Change Parameter",change_parameter_bounds.left+3,(change_parameter_bounds.bottom-change_parameter_bounds.top)/2 + change_parameter_bounds.top-4,1);//change_parameter
				delay_ms(200);
				gui_puts("Change Parameter",change_parameter_bounds.left+3,(change_parameter_bounds.bottom-change_parameter_bounds.top)/2 + change_parameter_bounds.top-4,0);//change_parameter
				return MENU_CHANGE_PARAMETER;
				}

		else if (pointInRect(screen_loc, change_minMax_bounds))
				{
				lcd_fill_rect(change_minMax_bounds,0xFF);
				gui_puts("Change Min/Max",change_minMax_bounds.left+3,(change_minMax_bounds.bottom-change_minMax_bounds.top)/2 + change_minMax_bounds.top-4,1);//change_minMax
				delay_ms(200);
				gui_puts("Change Min/Max",change_minMax_bounds.left+3,(change_minMax_bounds.bottom-change_minMax_bounds.top)/2 + change_minMax_bounds.top-4,0);//change_minMax
				return MENU_CHANGE_MINMAX;
				}

		else if (pointInRect(screen_loc, change_metric_bounds))
				{
				lcd_fill_rect(change_standard_bounds,0x00);
				lcd_rect(change_standard_bounds,1);
				gui_puts(units_desc,change_standard_bounds.left+3,(change_standard_bounds.bottom-change_standard_bounds.top)/2 + change_standard_bounds.top-4,0);//change_standard

				lcd_fill_rect(change_metric_bounds,0xFF);
				gui_puts(user_units_desc,change_metric_bounds.left+3,(change_metric_bounds.bottom-change_metric_bounds.top)/2 + change_metric_bounds.top-4,1);//change_metric
				g->user_units = user_units;
				}

		else if (pointInRect(screen_loc, change_standard_bounds))
				{
				lcd_fill_rect(change_metric_bounds,0x00);
				lcd_rect(change_metric_bounds,1);
				gui_puts(user_units_desc,change_metric_bounds.left+3,(change_standard_bounds.bottom-change_standard_bounds.top)/2 + change_standard_bounds.top-4,0);//change_standard

				lcd_fill_rect(change_standard_bounds,0xFF);
				gui_puts(units_desc,change_standard_bounds.left+3,(change_standard_bounds.bottom-change_standard_bounds.top)/2 + change_standard_bounds.top-4,1);//change_standard
				g->user_units = otherUnits;
				}


		}

	}//end while
} //end 





//////////////////////////////////////////////////////////////////
//
//	Menu     select layout
//
//////////////////////////////////////////////////////////////////

unsigned char menu_selectLayout()
{
//choose the layout here

lcd_clear_graph();

unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 152;
	exit.top = 78;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	

unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 152;
	back.top = 44;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;




unsigned char icon_width = 40;
unsigned char icon_height = 24;

LCD_RECT dlayout;		//default layout
	dlayout.left = 10;
	dlayout.top = 35;
	dlayout.right = dlayout.left + icon_width;
	dlayout.bottom = dlayout.top + icon_height;
	

LCD_RECT scopelayout;		//scope layout
	scopelayout.left = 55;
	scopelayout.top = 35;
	scopelayout.right = scopelayout.left + icon_width;
	scopelayout.bottom = scopelayout.top + icon_height;


LCD_RECT threebarlayout;		//bar layout
	threebarlayout.left = 100;
	threebarlayout.top = 35;
	threebarlayout.right = threebarlayout.left + icon_width;
	threebarlayout.bottom = threebarlayout.top + icon_height;


LCD_RECT barlayout;		//bar layout
	barlayout.left = 10;
	barlayout.top = 65;
	barlayout.right = barlayout.left + icon_width;
	barlayout.bottom = barlayout.top + icon_height;


LCD_RECT digitallayout;		//digital layout
	digitallayout.left = 55;
	digitallayout.top = 65;
	digitallayout.right = digitallayout.left + icon_width;
	digitallayout.bottom = digitallayout.top + icon_height;


LCD_RECT overalayout;		//over a layout
	overalayout.left = 100;
	overalayout.top = 65;
	overalayout.right = overalayout.left + icon_width;
	overalayout.bottom = overalayout.top + icon_height;


LCD_RECT overlayout;		//over layout
	overlayout.left = 10;
	overlayout.top = 95;
	overlayout.right = overlayout.left + icon_width;
	overlayout.bottom = overlayout.top + icon_height;


LCD_RECT XLdigitallayout;		//XLdigital layout
	XLdigitallayout.left = 55;
	XLdigitallayout.top = 95;
	XLdigitallayout.right = XLdigitallayout.left + icon_width;
	XLdigitallayout.bottom = XLdigitallayout.top + icon_height;



//	Draw the layout menu icns

	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	
	gui_drawBMP(default_layout,dlayout.left,dlayout.top,icon_width,icon_height,0);//default layout
	gui_drawBMP(scope_view,scopelayout.left,scopelayout.top,icon_width,icon_height,0); //Scope View Layout
	gui_drawBMP(bar_layout_a,threebarlayout.left,threebarlayout.top,icon_width,icon_height,0); //3 bar
	gui_drawBMP(bar_layout,barlayout.left,barlayout.top,icon_width,icon_height,0); //bar layout
	gui_drawBMP(digital_layout,digitallayout.left,digitallayout.top,icon_width,icon_height,0); //digital Layout
	gui_drawBMP(over_layout_a,overalayout.left,overalayout.top,icon_width,icon_height,0); //over a
	gui_drawBMP(over_layout,overlayout.left,overlayout.top,icon_width,icon_height,0); //over Layout
	gui_drawBMP(XLdigital_layout,XLdigitallayout.left,XLdigitallayout.top,icon_width,icon_height,0); //XLdigital


	gui_puts("Changing Gauge Layout",50,8,0);
	
	
/*! Wait for ever in the layout menu */
POINT screen_loc;


while(1)
	{
	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc, exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				return EXIT_MENU;
				}

			else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				return MENU_GAUGE_SETTINGS;
				}

			else if (pointInRect(screen_loc, dlayout))
				{
				gui_drawBMP(default_layout,dlayout.left,dlayout.top,icon_width,icon_height,1);//default layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_STANDARD);
				gui_drawBMP(default_layout,dlayout.left,dlayout.top,icon_width,icon_height,0);//default layout
				return EXIT_MENU;
				}
			
			else if (pointInRect(screen_loc, scopelayout))
				{
				gui_drawBMP(scope_view,scopelayout.left,scopelayout.top,icon_width,icon_height,1); //Scope View Layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_SCOPE);
				gui_drawBMP(scope_view,scopelayout.left,scopelayout.top,icon_width,icon_height,0); //Scope View Layout
				return EXIT_MENU;
				}

			else if (pointInRect(screen_loc, threebarlayout))
				{
				gui_drawBMP(bar_layout_a,threebarlayout.left,threebarlayout.top,icon_width,icon_height,1); //3 bar
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_3BAR);
				gui_drawBMP(bar_layout_a,threebarlayout.left,threebarlayout.top,icon_width,icon_height,0); //3 bar
				return EXIT_MENU;
				}


			else if (pointInRect(screen_loc, barlayout))
				{
				gui_drawBMP(bar_layout,barlayout.left,barlayout.top,icon_width,icon_height,1); //bar layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_BAR);
				gui_drawBMP(bar_layout,barlayout.left,barlayout.top,icon_width,icon_height,0); //bar layout
				return EXIT_MENU;
				}

			else if (pointInRect(screen_loc, digitallayout))
				{
				gui_drawBMP(digital_layout,digitallayout.left,digitallayout.top,icon_width,icon_height,1); //digital Layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_DIGITAL);
				gui_drawBMP(digital_layout,digitallayout.left,digitallayout.top,icon_width,icon_height,0); //digital Layout
				return EXIT_MENU;
				}

			else if (pointInRect(screen_loc, overalayout))
				{
				gui_drawBMP(over_layout_a,overalayout.left,overalayout.top,icon_width,icon_height,1); //over a
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_SCOPE_BAR_ANALOG);
				gui_drawBMP(over_layout_a,overalayout.left,overalayout.top,icon_width,icon_height,0); //over a
				return EXIT_MENU;
				}
				
			else if (pointInRect(screen_loc, overlayout))
				{
				gui_drawBMP(over_layout,overlayout.left,overlayout.top,icon_width,icon_height,1); //over Layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_SCOPE_MIX);
				gui_drawBMP(over_layout,overlayout.left,overlayout.top,icon_width,icon_height,0); //over Layout
				return EXIT_MENU;
				}
			
			else if (pointInRect(screen_loc, XLdigitallayout))
				{
				gui_drawBMP(XLdigital_layout,XLdigitallayout.left,XLdigitallayout.top,icon_width,icon_height,1); //XL digital Layout
				delay_ms(200);
				gauge_changeGaugeLayout(LAYOUT_XLDIGITAL);
				gui_drawBMP(XLdigital_layout,XLdigitallayout.left,XLdigitallayout.top,icon_width,icon_height,0); //XL digital Layout
				return EXIT_MENU;
				}
	
		} //end if touch_get_cursor()
	} //end while

return 0;
}









/////////////////////////////////////////
/*!
   Begin the Tuning menu.
*/
/////////////////////////////////////////
unsigned char menu_tuning()
{

//LCD_RECT	main_outline;


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	
unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;




// Setup the press region for the first sub menu 
	LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 




//	Draw the main menu icns
	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(TuningIcon3,55,5,120,25,0);//Tuning Bannar
//


//setup and draw the button outlines
	LCD_RECT change_spark_bounds,change_layout_bounds,change_ms_wizard_bounds;	

	change_spark_bounds.left = 4;		change_spark_bounds.right = 161;
	change_layout_bounds.left = 4;		change_layout_bounds.right = 161;
	change_ms_wizard_bounds.left = 4;	change_ms_wizard_bounds.right = 161;

	change_spark_bounds.top = 70;		change_spark_bounds.bottom = 90;
	change_layout_bounds.top = 40;		change_layout_bounds.bottom = 60;
	change_ms_wizard_bounds.top = 100;	change_ms_wizard_bounds.bottom = 120;	

//	lcd_rect(change_spark_bounds,1); 	
  	lcd_rect(change_layout_bounds,1); 
//	lcd_rect(change_ms_wizard_bounds,1);


//	gui_puts("Change Spark",change_spark_bounds.left+3,(change_spark_bounds.bottom-change_spark_bounds.top)/2 + change_spark_bounds.top-3,0);//change_spark
	gui_puts("VE Table 1",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-3,0);//change_layout
//	gui_puts("Real Time Auto Locate OFF",change_ms_wizard_bounds.left+3,(change_ms_wizard_bounds.bottom-change_ms_wizard_bounds.top)/2 + change_ms_wizard_bounds.top-3,0);//change ms1 wizard

POINT screen_loc; //our touched location
	
/*! Wait for ever until a valid press */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_MAIN;
				}


//Gauge Settings Buttons
		else if (pointInRect(screen_loc, change_layout_bounds))
				{
				lcd_fill_rect(change_layout_bounds,0xFF);
				gui_puts("Change Fuel",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-4,1);//change_layout
				delay_ms(200);
				lcd_fill_rect(change_layout_bounds,0x00);
				gui_puts("VE Table 1",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-4,0);//change_layout
				
				return MENU_EDITTABLE;
				}

		} //end if touch_get_cursor()

	} //end while

} //end end tuning()





/////////////////////////////////////////
/*!
   Begin the Gauge Settings menu.
*/
/////////////////////////////////////////
unsigned char menu_gauge_settings()
{

//LCD_RECT	main_outline;


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	
unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;




// Setup the press region for the first sub menu 
	LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 




//	Draw the main menu icns
	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(GaugeSettings2,40,5,160,20,0);//Gauge Settings Bannar
//


//setup and draw the button outlines
	LCD_RECT change_gauge_bounds,change_layout_bounds,change_ms_wizard_bounds;	

	change_gauge_bounds.left = 4;		change_gauge_bounds.right = 115;
	change_layout_bounds.left = 4;		change_layout_bounds.right = 115;
	change_ms_wizard_bounds.left = 4;	change_ms_wizard_bounds.right = 115;

	change_gauge_bounds.top = 40;		change_gauge_bounds.bottom = 60;
	change_layout_bounds.top = 70;		change_layout_bounds.bottom = 90;
	change_ms_wizard_bounds.top = 100;	change_ms_wizard_bounds.bottom = 120;	

	lcd_rect(change_gauge_bounds,1); 	
  	lcd_rect(change_layout_bounds,1); 
	lcd_rect(change_ms_wizard_bounds,1);


	gui_puts("Change Gauge",change_gauge_bounds.left+3,(change_gauge_bounds.bottom-change_gauge_bounds.top)/2 + change_gauge_bounds.top-3,0);//change_gauge
	gui_puts("Change Layout",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-3,0);//change_layout
	gui_puts("Sensor Setup",change_ms_wizard_bounds.left+3,(change_ms_wizard_bounds.bottom-change_ms_wizard_bounds.top)/2 + change_ms_wizard_bounds.top-3,0);//change ms1 wizard

POINT screen_loc; //our touched location
	
/*! Wait for ever until a valid press */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_MAIN;
				}


//Gauge Settings Buttons
		else if (pointInRect(screen_loc, change_gauge_bounds))
				{
				lcd_fill_rect(change_gauge_bounds,0xFF);
				gui_puts("Change Gauge",change_gauge_bounds.left+3,(change_gauge_bounds.bottom-change_gauge_bounds.top)/2 + change_gauge_bounds.top-4,1);//change_gauge
				delay_ms(200);
				lcd_fill_rect(change_gauge_bounds,0x00);
				gui_puts("Change Gauge",change_gauge_bounds.left+3,(change_gauge_bounds.bottom-change_gauge_bounds.top)/2 + change_gauge_bounds.top-4,0);//change_gauge
				//go into gauge changing
				gauge_selectGaugeToChange();
				return EXIT_MENU; //come back
				}

		else if (pointInRect(screen_loc, change_layout_bounds))
				{
				lcd_fill_rect(change_layout_bounds,0xFF);
				gui_puts("Change Layout",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-4,1);//change_layout
				delay_ms(200);
				lcd_fill_rect(change_layout_bounds,0x00);
				gui_puts("Change Layout",change_layout_bounds.left+3,(change_layout_bounds.bottom-change_layout_bounds.top)/2 + change_layout_bounds.top-4,0);//change_layout
				//go into gauge layout selection

				return menu_selectLayout();
				
				}

		else if (pointInRect(screen_loc, change_ms_wizard_bounds))
				{
				lcd_fill_rect(change_ms_wizard_bounds,0xFF);
				gui_puts("Sensor Setup",change_ms_wizard_bounds.left,(change_ms_wizard_bounds.bottom-change_ms_wizard_bounds.top)/2 + change_ms_wizard_bounds.top-3,1);//change ms1 wizard
				delay_ms(200);
				lcd_fill_rect(change_ms_wizard_bounds,0x00);
				gui_puts("Sensor Setup",change_ms_wizard_bounds.left,(change_ms_wizard_bounds.bottom-change_ms_wizard_bounds.top)/2 + change_ms_wizard_bounds.top-3,0);//change ms1 wizard
				
				return MENU_SENSORSETUP;
				
				}

		} //end if touch_get_cursor()

	} //end while

} //end menu_gauge_settings()




unsigned char menu_sensorsetup()
{


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	
unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;





// Setup the press region for the first sub menu 
	LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 

	gui_drawBMP(SensorSetup,20,5,168,31,0); //sensor settings banner

	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon


//setup and draw the button outlines
	LCD_RECT calibration_bounds,change_ms_wizard_bounds;	

	calibration_bounds.left = 4;		calibration_bounds.right = 115;
	change_ms_wizard_bounds.left = 4;	change_ms_wizard_bounds.right = 115;

	calibration_bounds.top = 40;		calibration_bounds.bottom = 60;
	change_ms_wizard_bounds.top = 100;	change_ms_wizard_bounds.bottom = 120;	

	lcd_rect(calibration_bounds,1); 	
	lcd_rect(change_ms_wizard_bounds,1);


	gui_puts("Calibration",calibration_bounds.left+3,(calibration_bounds.bottom-calibration_bounds.top)/2 + calibration_bounds.top-3,0);//calibration
	gui_puts("MS1/Extra Wizard",change_ms_wizard_bounds.left+3,(change_ms_wizard_bounds.bottom-change_ms_wizard_bounds.top)/2 + change_ms_wizard_bounds.top-3,0);//change ms1 wizard


POINT screen_loc; //our touched location


	
/*! Wait for ever until a valid press */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_GAUGE_SETTINGS;
				}


		else if (pointInRect(screen_loc, calibration_bounds))
				{
				lcd_fill_rect(calibration_bounds,0xFF);
				delay_ms(200);
				lcd_clear_graph();
				return MENU_CALIBRATE;
				}	
					
		else if (pointInRect(screen_loc, change_ms_wizard_bounds))
				{
				lcd_fill_rect(change_ms_wizard_bounds,0xFF);
				delay_ms(200);
				lcd_clear_graph();
				return MENU_1WIZARD;
				}

		}



	} //end while

} //end menu_sensorsetup()




unsigned char menu_calibrate()
{


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	
unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;




// Setup the press region for the first sub menu 
	LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 

	gui_drawBMP(calibration,43,5,137,21,0); //draw the calibration banner

	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon


//setup and draw the button outlines
	LCD_RECT vss_bounds;	

	vss_bounds.left = 4;		vss_bounds.right = 130;

	vss_bounds.top = 40;		vss_bounds.bottom = 60;

	lcd_rect(vss_bounds,1); 	


	gui_puts("Vehicle Speed Sensor",vss_bounds.left+3,(vss_bounds.bottom-vss_bounds.top)/2 + vss_bounds.top-3,0);//vss


POINT screen_loc; //our touched location


	
/*! Wait for ever until a valid press */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_SENSORSETUP;
				}


		else if (pointInRect(screen_loc, vss_bounds))
				{
				lcd_fill_rect(vss_bounds,0xFF);
				delay_ms(200);
				lcd_clear_graph();
				return MENU_CALIBRATEVSS;
				}	


		}



	} //end while

} //end menu_calibration()






//////////////////////////////////////////////////////////////////
///		VSS Calibration Menu
//////////////////////////////////////////////////////////////////
unsigned char menu_calibratevss(void)
{

char out[40];
//unsigned char x;

LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;
lcd_fill_rect(main_outline,0x00); //!< Erase the background

strncpy_P(out,PSTR("Calibrate the VSS"),40);
gui_puts(out,7,2,0);
lcd_line(0,10,239,10,1);

strncpy_P(out,PSTR("Drive at 30MPH"),40);
gui_puts(out,12,17,0);
strncpy_P(out,PSTR("and press the 'Calibrate' button"),40);
gui_puts(out,12,26,0);


//!< Making some new buttons here
//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	
unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;

	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon


//!< Making some new buttons here
LCD_RECT cali;		//next is in the bottom right button of the menu screen
	cali.left = 7;
	cali.top = 100;
	cali.right = 70;
	cali.bottom = 122;
lcd_rect(cali,1); 

strncpy_P(out,PSTR("Calibrate"),40);
gui_puts(out,cali.left+5,cali.top+8,0); 

strncpy_P(out,PSTR("pulse/mile"),40);
gui_puts(out,100,52,0);
strncpy_P(out,PSTR("MPH"),40);
gui_puts(out,100,77,0);



POINT screen_loc;

unsigned long num=0;

/*! Wait for ever in the menu */
while(1)
	{
	delay_ms(70);

	num = speedo_getPulsesPerMile();
	ltoa(num,out,10);
	gui_largeNumS(out,5,40,0);
	
	num = speedo_getMPH();
	ltoa(num,out,10);
	gui_largeNumS(out,5,67,0);

	if (touch_get_cursor(&screen_loc))
		{
		if (pointInRect(screen_loc,exit))
				{
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
				delay_ms(200);
				gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
				lcd_clear_graph();
				return EXIT_MENU;
				}

		else if (pointInRect(screen_loc, back))
				{
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,1); // Back the menu icon
				delay_ms(200);
				gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0); // Back the menu icon
				lcd_clear_graph();
				return MENU_CALIBRATE;
				}

		else if (pointInRect(screen_loc,cali))
				{
				//calibrate the VSS
				lcd_fill_rect(cali,0xFF);
				delay_ms(200);
				lcd_fill_rect(cali,0x00);
				lcd_rect(cali,1); 
				strncpy_P(out,PSTR("Calibrate"),40);
				gui_puts(out,cali.left+5,cali.top+8,0); 
				speedo_calibrate();
				}		

				

		} //end if touch_get_cursor
	
	} // end while 	




} // end of menu







/////////////////////////////////////////
//
//Number Entry
//
/////////////////////////////////////////
float menu_getNumberFromUser(char * display_msg)
{
	
	float the_num=0;
	unsigned char x;
	LCD_RECT		bounds[12],entryBox, exitButton, backSpace;
//	unsigned char*	bmps[11]; 	
//	extern unsigned char * large_nums;

	
	lcd_clear_graph();
	
	gui_puts(display_msg,0,0,0);

	entryBox.left = 80;
	entryBox.right = 161;
	entryBox.top = 30;
	entryBox.bottom = 56;
	lcd_rect(entryBox,1);

	exitButton.left = 200;
	exitButton.right = 239;
	exitButton.top = 10;
	exitButton.bottom = 30;
	lcd_rect(exitButton,1);
	

	backSpace.left = 190;
	backSpace.right = 239;
	backSpace.top = 55;
	backSpace.bottom = 75;
	lcd_rect(backSpace,1);

	gui_puts("Save",exitButton.left+5,(exitButton.bottom-exitButton.top)/2+exitButton.top-4,0);
	gui_puts("BackSp",backSpace.left+6,(backSpace.bottom-backSpace.top)/2+backSpace.top-4,0);

	for (x=0;x<12;x++)
		{
		bounds[x].left 	= x*20+1;
		bounds[x].right = bounds[x].left+17;
		bounds[x].top 	= 80;
		bounds[x].bottom = bounds[x].top+26;
		if (x < 10)
			gui_largeNumC(x+48,bounds[x].left+1,bounds[x].top+2,0);
		else
		if (x==10)
			gui_largeNumC('.',bounds[x].left+1,bounds[x].top+2,0);
		else
			gui_largeNumC('-',bounds[x].left+1,bounds[x].top+2,0);

		lcd_rect(bounds[x],1);
		}

unsigned char index=0,c=0, drawIt = TRUE, has_period = FALSE, i;

char ascii_num[6];
POINT p;
for (i=0; i<6; i++)
ascii_num[i]=0x00;

while(1)
{
if(touch_get_cursor(&p))
	{
	
	for(x=0;x<12;x++)
		{
		if (pointInRect(p,bounds[x]))
			{
			lcd_fill_rect(bounds[x],0xFF);
			delay_ms(200);
			lcd_fill_rect(bounds[x],0x00);
			lcd_rect(bounds[x],1);
			if (x < 10)
				{
				c = x+48;
				}
			else
			if (x==10)
				{
				c = '.';
				}
			else
				{
				c = '-';
				}
			
			gui_largeNumC(c,bounds[x].left+1,bounds[x].top+2,0);//refresh number pad
			

			if ((c == '.') && (has_period == TRUE))
				drawIt = FALSE;
			if ((c == '-') && (index != 0))
				drawIt = FALSE;
			
				
			if (drawIt)
				{
				if (index < 5)
					{
						if (c == '.')
							has_period = TRUE;	
					ascii_num[index] = c;
					gui_largeNumC(c,entryBox.left+(index*16)+1,entryBox.top+2,0);
				
					index++;
				
					}
				
				}
			
			drawIt = TRUE;
			
			}
		} //end for

		if (pointInRect(p,exitButton))
			{
			return (float)atoi(ascii_num); //we realize this rounds down to the nearest whole number... we really need to do an ascii to float (atof) but the atof function seems to be awkward in the avr
			}

		if (pointInRect(p,backSpace))
			{
			lcd_fill_rect(backSpace,0xFF);
			delay_ms(200);
			lcd_fill_rect(backSpace,0x00);
			lcd_rect(backSpace,1);
			gui_puts("BackSp",backSpace.left+6,(backSpace.bottom-backSpace.top)/2+backSpace.top-4,0);

				
				if (index>0)
				{
					index--;
					if (ascii_num[index] == '.')
						has_period = FALSE;
					
					ascii_num[index]= 0x00;

					LCD_RECT r;
					r.left = ((index)*16)+entryBox.left+1;
					r.right = r.left+15;
					r.top = entryBox.top+2;
					r.bottom = entryBox.bottom-2;

					lcd_fill_rect(r,0x00);

				}

			}
	}

} //end while
return the_num;

} //end number entry function










/////////////////////////////////////////
/*!
   Begin Select Parameter -Lets ask the user which parameter they want.
   \returns the chosen parameter.
*/
/////////////////////////////////////////
unsigned char menu_selectParameter(GAUGE * g, char * description)
{


//!< Making some new buttons here
unsigned char width_exit = 64;
unsigned char height_exit = 29;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 168;
	exit.top = 91;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;
	

unsigned char width_back = 64;
unsigned char height_back = 29;
LCD_RECT back;		//back is in the bottom right button of the menu screen
	back.left = 168;
	back.top = 55;
	back.right = back.left + width_back;
	back.bottom = back.top + height_back;

unsigned char width_up = 32;
unsigned char height_up = 31;
LCD_RECT up;		//up button
	up.left = 128;	//change the x(up.left) and y(up.top) to move the button to a new location
	up.top = 26;
	up.right = up.left + width_up;
	up.bottom = up.top + height_up;

unsigned char width_down = 32;
unsigned char height_down = 31;
LCD_RECT down;		//down button
	down.left = 128;	//change the x(down.left) and y(down.top) to move the button
	down.top = 64;
	down.right = down.left + width_down;
	down.bottom = down.top + height_down;



			// Setup the press region for the first sub menu 
	LCD_RECT main_outline;	//outline of main menu
	main_outline.left = 0;
	main_outline.top = 0;
	main_outline.right = 239;
	main_outline.bottom = 127;


lcd_fill_rect(main_outline,0x00); //!< Erase the background
lcd_rect(main_outline,1);  //!< Draw the main menu outline rectangle 


//Display the Gauge decription
	gui_puts("Gauge:",2,2,0); 
	gui_puts(description,40,2,0); //display the gauge description


	gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
//	gui_drawBMP(BackButtonIcon,back.left,back.top,width_back,height_back,0);  // back icon
	gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,0);//up arrow
	gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,0); //down arrow



//-----Begin scollbar list setup
LCD_RECT listBounds, scrollRect, scrollIndicator;

char label_array[gauge_getGaugeCount()*GAUGE_LABEL_LENGTH];
unsigned char param_ids[gauge_getGaugeCount()];
unsigned char dispCount=6; //the number of items we want to display
unsigned char x;


LCD_RECT	select_rects[dispCount]; //our pressable 'button' areas


listBounds.right = 100;
listBounds.top = 15;
listBounds.bottom = 120;
listBounds.left = 10;

lcd_rect_diffSize(listBounds,2,1); //draw the outline 1 biggger

scrollRect.left = listBounds.right+3;
scrollRect.top = listBounds.top-1;
scrollRect.right = scrollRect.left+SCROLLBAR_WIDTH;
scrollRect.bottom = listBounds.bottom+1;

scrollIndicator.left = scrollRect.left+1;
scrollIndicator.right = scrollRect.right-1;
scrollIndicator.top = scrollRect.top+2;
scrollIndicator.bottom = scrollIndicator.top + SCROLL_INDICATOR_HEIGHT;

lcd_rect_diffSize(scrollRect,1,1); //draw an outline of the scroll bar
lcd_fill_rect(scrollIndicator,0xFF); //fill the scrollIndicator in


gauge_getGauge_labels(&param_ids[0],&label_array[0]); //builds the list of ids and labels



char * str;

//--Setup the selection rectangles and draw the first labels
unsigned char select_size = (listBounds.bottom - listBounds.top)/dispCount;

for(x=0;x<dispCount;x++)
	{

	select_rects[x].top = listBounds.top+(select_size * x);
	select_rects[x].bottom = select_rects[x].top + select_size;
	select_rects[x].left = listBounds.left;
	select_rects[x].right = listBounds.right;
	
	lcd_rect(select_rects[x],1); //draw the outline 

	str = (label_array+(x*GAUGE_LABEL_LENGTH));
	//str[GAUGE_LABEL_LENGTH-1] =0x00;
	gui_puts(str, select_rects[x].left+5, select_rects[x].top + (select_size/2),0);
	}
//--------------------------------


	
POINT screen_loc;
unsigned char count=0;
	
/*! Wait for ever in the menu */
while(1)
	{
	
	if (touch_get_cursor(&screen_loc))
		{
		
		if (pointInRect(screen_loc, exit))
			{
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
			delay_ms(200);
			gui_drawBMP(ExitButtonIcon,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
			return EXIT_MENU;
			}

		else if (pointInRect(screen_loc, up))
			{
			gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,1); // Back the menu icon
			delay_ms(150);
			gui_drawBMP(up_arrow,up.left,up.top,width_up,height_up,0); // Back the menu icon

			if ( count > 0) //stop them from going too low
				{

				count-=dispCount;
				lcd_fill_rect(scrollRect,0x00); //clear the scrollbar
				unsigned char range;
				range = (scrollRect.bottom-SCROLL_INDICATOR_HEIGHT) - (scrollRect.top);
				scrollIndicator.top = ((range/ (gauge_getGaugeCount()-dispCount))* count + scrollRect.top);
				scrollIndicator.bottom = scrollIndicator.top + SCROLL_INDICATOR_HEIGHT;
				lcd_fill_rect(scrollIndicator,0xFF);
				
				//update the list
				for(x=0;x<dispCount;x++)
					{
					lcd_fill_rect(select_rects[x],0x00); //clear the previous area
					lcd_rect(select_rects[x],1); //draw the outline 

					str = (label_array+((count+x)*GAUGE_LABEL_LENGTH));
					gui_puts(str, select_rects[x].left+5, select_rects[x].top + (select_size/2),0);
					}

				}

			}
			
		else if (pointInRect(screen_loc, down))
			{
			gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,1); // down icon
			delay_ms(150);
			gui_drawBMP(down_arrow,down.left,down.top,width_down,height_down,0); // down icon
			
			if (count < (gauge_getGaugeCount()-dispCount)) //stop them from going too high
//			if (count < (gauge_getGaugeCount() )) //stop them from going too high	
				{

				count+=dispCount;
				lcd_fill_rect(scrollRect,0x00); //clear the scrollbar
				unsigned char range;
				range = (scrollRect.bottom-SCROLL_INDICATOR_HEIGHT-1) - (scrollRect.top+1);
				scrollIndicator.top = ((range/ (gauge_getGaugeCount()-dispCount))* count + scrollRect.top+1);
				scrollIndicator.bottom = scrollIndicator.top + SCROLL_INDICATOR_HEIGHT;
				lcd_fill_rect(scrollIndicator,0xFF);

				//update the list
				for(x=0;x<dispCount;x++)
					{

						lcd_fill_rect(select_rects[x],0x00); //clear the previous area
						lcd_rect(select_rects[x],1); //draw the outline 
						if ( (x+count) < gauge_getGaugeCount() )
							{
							str = (label_array+((count+x)*GAUGE_LABEL_LENGTH));
							gui_puts(str, select_rects[x].left+5, select_rects[x].top + (select_size/2),0);
							}
					}

				}	
			}
		
		//check the rectangles
		for (x=0;x<dispCount;x++)
			{
				if( pointInRect(screen_loc,select_rects[x]) )
					{
					if ( (count+x) < gauge_getGaugeCount() )
						{
						str = (label_array+((count+x)*GAUGE_LABEL_LENGTH));

						lcd_fill_rect(select_rects[x],0xFF);					
						gui_puts(str, select_rects[x].left+5, select_rects[x].top + (select_size/2),1);
						delay_ms(150);
						lcd_fill_rect(select_rects[x],0x00);
						gui_puts(str, select_rects[x].left+5, select_rects[x].top + (select_size/2),0);
						lcd_rect(select_rects[x],1);
					
						gauge_getGauge(g,param_ids[count+x]);
						return 0;
						}
					} 
			}
			
			
					
			

		} //end if touch_get_cursor()

	} //end while

} //end function




unsigned char menu_editTable()
{
//init variables
	LCD_RECT ybin_inc, 	ybin_dec;
	LCD_RECT xbin_inc, 	xbin_dec;
	LCD_RECT value_inc, value_dec;


	ybin_inc.left 	= 92; 
	ybin_inc.right 	= 115;
	ybin_inc.top 	= 20;
	ybin_inc.bottom = 43;

	ybin_dec.left 	= 92; 
	ybin_dec.right 	= 115;
	ybin_dec.top 	= 73;
	ybin_dec.bottom = 96;

	xbin_dec.left	= 108;
	xbin_dec.right	= 131;
	xbin_dec.top	= 103;
	xbin_dec.bottom	= 126;


	xbin_inc.left	= 215;
	xbin_inc.right	= 238;
	xbin_inc.top	= 103;
	xbin_inc.bottom	= 126;
	
	value_inc.left		= 210;
	value_inc.right		= 233;
	value_inc.top		= 30;
	value_inc.bottom	= 53;

	value_dec.left		= 210;
	value_dec.right		= 233;
	value_dec.top		= 58;
	value_dec.bottom	= 81;

//Start of development

unsigned char width_exit = 64;
unsigned char height_exit = 20;
LCD_RECT exit;		//Exit is in the bottom right button of the menu screen
	exit.left = 0;
	exit.top = 103;
	exit.right = exit.left + width_exit;
	exit.bottom = exit.top + height_exit;

unsigned char width_burn = 64;
unsigned char height_burn = 20;
LCD_RECT burn;		//Burn is in the top right screen
	burn.left = 145;
	burn.top = 1;
	burn.right = burn.left + width_burn;
	burn.bottom = burn.top + height_burn;


lcd_clear_graph();

//store the gauge layout
	unsigned char curr_layout = gauge_getGaugeLayout(); //save the current layout
//create new gauge layout
	gauge_changeGaugeLayout(LAYOUT_TUNING);
	gauge_drawGaugeLayout(); //draw the gauges



lcd_rect_diffSize(ybin_inc,1,1);
lcd_rect_diffSize(ybin_dec,1,1);
lcd_rect_diffSize(xbin_inc,1,1);
lcd_rect_diffSize(xbin_dec,1,1);
lcd_rect_diffSize(value_inc,1,1);
lcd_rect_diffSize(value_dec,1,1);
lcd_line(82,0,82,127,1); // realtime/vetune v line
lcd_line(83,0,83,127,1); // realtime/vetune v line


/*
gui_puts("RPM",1,1,0);
gui_largeNumS("3010",0,9,0);

gui_puts("AFR",1,34,0);
gui_largeNumS("12.7",0,42,0);

gui_puts("MAP",1,67,0);
gui_largeNumS("125",0,75,0);
*/

gui_drawBMP(minus_sign,xbin_dec.left,xbin_dec.top+10,24,4,0);
gui_drawBMP(plus_sign,xbin_inc.left,xbin_inc.top,24,24,0);
//gui_largeNumS("2500",145,2,0); //RPM bin
lcd_line(130,97,239,97,1); //bottom h line


gui_drawBMP(plus_sign,ybin_inc.left,ybin_inc.top,24,24,0);
gui_drawBMP(minus_sign,ybin_dec.left,ybin_dec.top+10,24,4,0);
//gui_largeNumS("150",67,70,0); //KPA bin
lcd_line(132,20,132,99,1); //KPA v line

gui_drawBMP(plus_sign,value_inc.left,value_inc.top,24,24,0);
gui_drawBMP(minus_sign,value_dec.left,value_dec.top+10,24,4,0);
gui_puts("Value:", 140,32,0);
//gui_largeNumS("109",140,40,0);
gui_drawBMP(ExitButtonIconThin,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
gui_drawBMP(BurnButtonIcon,burn.left,burn.top,width_burn,height_burn,0); // BURN the menu icon

unsigned char * loc;
unsigned char cmd[] = {'r', 0x00, 0x09, 0x00, 0x00, 0x04, 0x00};  
char out[5];
unsigned int x=0;

unsigned int y_bins[16];
unsigned int x_bins[16];
unsigned char values[256];
unsigned char y_bins_index=0;
unsigned char x_bins_index=0;


while(USB_read(1,1)); //purge
USB_send(cmd,7);
USB_read(1000,1000);




//-----Get the y_bins
loc = USB_getBuff(); //get a pointer to the data
loc+=864;
for (x=0;x<16;x++)
	{
	y_bins[x] = ((*loc) << 8) + (*(loc+1));
	loc+=2;
	
	y_bins[x] /= 10;
	}

dtostrf(y_bins[0],3,0,out);
gui_largeNumS(out,ybin_inc.left-8,ybin_inc.bottom+4,0); 




//-----Get the X_bins
loc = USB_getBuff(); //get a pointer to the data
loc+= 768;
for (x=0;x<16;x++)
	{
	x_bins[x] = ((*loc) << 8) + (*(loc+1));
	loc+=2;
	}

dtostrf(x_bins[0],4,0,out);
gui_largeNumS(out,xbin_dec.right+3,xbin_dec.top,0); 


//get the values
loc = USB_getBuff();

for (x=0;x<256;x++)
	{
	values[x] = *loc;
	loc++;
	}

dtostrf(values[0],3,0,out);
gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);

	

POINT screen_loc;
GAUGE ** gauge_list;
unsigned char gauge_count;



while(1)
	{

//refresh tuning gauges
while(USB_read(1,1)){;} //flush the port

		ms_getSensors(); //fill the USB receive buffer with our MS response
		gauge_list = gauge_getGaugePointers(&gauge_count);
		ms_convertSensors(&gauge_list[0], gauge_count); //use all the param_id's to find/convert thier values from the USB receive buffer
	
		gauge_refreshGaugeLayout(); //refresh all the current gauges


//monitor touchscreen
	if (touch_get_cursor(&screen_loc))
		{

		if (pointInRect(screen_loc, exit))
			{
			gui_drawBMP(ExitButtonIconThin,exit.left,exit.top,width_exit,height_exit,1); // EXIT the menu icon
			delay_ms(200);
			gui_drawBMP(ExitButtonIconThin,exit.left,exit.top,width_exit,height_exit,0); // EXIT the menu icon
			
			gauge_changeGaugeLayout(curr_layout); //restore their layout
			return EXIT_MENU;
			}


		if (pointInRect(screen_loc, xbin_inc))
			{
			if (x_bins_index <15)
				{
				char out[5];
				x_bins_index++;
				dtostrf(x_bins[x_bins_index],4,0,out);
				gui_largeNumS(out,xbin_dec.right+3,xbin_dec.top,0); 
				}
			
			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);


			lcd_fill_rect(xbin_inc,0xFF);
			delay_ms(150);
			lcd_fill_rect(xbin_inc,0x00);
			gui_drawBMP(plus_sign,xbin_inc.left,xbin_inc.top,24,24,0);
			}

		if (pointInRect(screen_loc, xbin_dec))
			{
			if (x_bins_index > 0)
				{
				char out[5];
				x_bins_index--;
				dtostrf(x_bins[x_bins_index],4,0,out);
				gui_largeNumS(out,xbin_dec.right+3,xbin_dec.top,0); 
				}

			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);


			lcd_fill_rect(xbin_dec,0xFF);
			delay_ms(150);
			lcd_fill_rect(xbin_dec,0x00);
			gui_drawBMP(minus_sign,xbin_dec.left,xbin_dec.top+10,24,4,0);
			}

		if (pointInRect(screen_loc, ybin_inc))
			{

			if (y_bins_index <15)
				{
				char out[4];
				y_bins_index++;
				dtostrf(y_bins[y_bins_index],3,0,out);
				gui_largeNumS(out,ybin_inc.left-8,ybin_inc.bottom+4,0); 
				}

			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);

			lcd_fill_rect(ybin_inc,0xFF);
			delay_ms(150);
			lcd_fill_rect(ybin_inc,0x00);
			gui_drawBMP(plus_sign,ybin_inc.left,ybin_inc.top,24,24,0);
			}

		if (pointInRect(screen_loc, ybin_dec))
			{

			if (y_bins_index > 0)
				{
				char out[4];
				y_bins_index--;
				dtostrf(y_bins[y_bins_index],3,0,out);
				gui_largeNumS(out,ybin_inc.left-8,ybin_inc.bottom+4,0); 
				}

			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);

			lcd_fill_rect(ybin_dec,0xFF);
			delay_ms(150);
			lcd_fill_rect(ybin_dec,0x00);
			gui_drawBMP(minus_sign,ybin_dec.left,ybin_dec.top+10,24,4,0);
			}


		if (pointInRect(screen_loc, value_inc))
			{
			//increment the value 
			unsigned char cmd[8];
			unsigned int index = (y_bins_index*16)+x_bins_index;
			
			values[index]++;
			
			cmd[0] = 'w';
			cmd[1] = 0x00;
			cmd[2] = 0x09;
			cmd[3] = index>>8; //high byte
			cmd[4] = index & 0xFF; //low byte
			cmd[5] = 0x00;
			cmd[6] = 0x01;
			cmd[7] = values[index];

			USB_send(cmd,8); //send the msg

			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);

			lcd_fill_rect(value_inc,0xFF);
			delay_ms(150);
			lcd_fill_rect(value_inc,0x00);
			gui_drawBMP(plus_sign,value_inc.left,value_inc.top,24,24,0);			
			}


		if (pointInRect(screen_loc, value_dec))
			{
			//decremement the value
			//increment the value 
			unsigned char cmd[8];
			unsigned int index = (y_bins_index*16)+x_bins_index;
			
			values[index]--;
			
			cmd[0] = 'w';
			cmd[1] = 0x00;
			cmd[2] = 0x09;
			cmd[3] = index>>8; //high byte
			cmd[4] = index & 0xFF; //low byte
			cmd[5] = 0x00;
			cmd[6] = 0x01;
			cmd[7] = values[index];

			USB_send(cmd,8); //send the msg

			dtostrf(values[(y_bins_index*16)+x_bins_index],3,0,out);
			gui_largeNumS(out,value_inc.left-70,value_inc.top+15,0);



			lcd_fill_rect(value_dec,0xFF);
			delay_ms(150);
			lcd_fill_rect(value_dec,0x00);
			gui_drawBMP(minus_sign,value_dec.left,value_dec.top+10,24,4,0);
			}

		
		if (pointInRect(screen_loc, burn))
			{
			///burn command

			cmd[0] = 'b';
			cmd[1] = 0x00;
			cmd[2] = 0x09;

			USB_send(cmd,3); //send the msg


			lcd_fill_rect(burn,0xFF);
			delay_ms(150);
			lcd_fill_rect(burn,0x00);
			gui_drawBMP(BurnButtonIcon,burn.left,burn.top,width_burn,height_burn,0); // BURN the menu icon
			}

		}
	
	
	}

return 0;
}
