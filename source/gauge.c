#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <math.h>

#include "t6963.h"
#include "gui.h"
#include "touchscreen.h"

#include "analogGauge.h"
#include "barGauge.h"
#include "digiGauge.h"
#include "XLdigiGauge.h"
#include "scopeGauge.h"
#include "gauge.h"

#include "ms2_defs.h"
#include "ms1_defs.h"
#include "string_manip.h"
#include "LCDash.h"
#include "bitops.h"
#include "menu.h"
#include "megasquirt.h"
#include "messageBar.h"
#include "units.h"
#include "obd.h"
#include "mut.h"



//Make some new gauges
XLDIGI_GAUGE 	XLdigi_gauge1;
DIGI_GAUGE digi_gauge1,digi_gauge2,digi_gauge3,digi_gauge4,tuning_digi_gauge1, tuning_digi_gauge2, tuning_digi_gauge3;
BAR_GAUGE	bar_g1,bar_g2, bar_g3;
ANALOG_GAUGE analog_g1;
SCOPE_GAUGE	 scope_g1,scope_g2;
GAUGE * gauge_PTR_array[5]; //an array of pointers pointing to our 'gauge' structure from each GUI gauge
unsigned char currentLayout = LAYOUT_XLDIGITAL; 

//eeprom gauge settings storage
EEMEM unsigned char ee_currentLayout;

EEMEM GAUGE 		ee_analog_g1,
					ee_bar_g1,ee_bar_g2, ee_bar_g3,
					ee_digi_gauge1, ee_digi_gauge2, ee_digi_gauge3, ee_digi_gauge4,
					ee_tuning_digi_gauge1, ee_tuning_digi_gauge2, ee_tuning_digi_gauge3, ee_tuning_digi_gauge4,
					ee_scope_g1, ee_scope_g2,
					ee_XLdigi_gauge1;

EEMEM unsigned char ee_analog_g1Saved,
					ee_bar_g1Saved, ee_bar_g2Saved, ee_bar_g3Saved,
					ee_digi_gauge1Saved, ee_digi_gauge2Saved, ee_digi_gauge3Saved, ee_digi_gauge4Saved,
					ee_tuning_digi_gauge1Saved, ee_tuning_digi_gauge2Saved, ee_tuning_digi_gauge3Saved, ee_tuning_digi_gauge4Saved,
					ee_scope_g1Saved, ee_scope_g2Saved,
					ee_XLdigi_gauge1Saved;

//Gauge alarms
unsigned char alarmsStatus[5]; 

/*
if (currentLayout == LAYOUT_STANDARD)
	{
	;
	}
else
if (currentLayout == LAYOUT_ANALOG)
	{
	;
	}
else
if (currentLayout == LAYOUT_DIGITAL)
	{
	;
	}
else
if (currentLayout == LAYOUT_SCOPE)
	{
	;
	}
*/


unsigned char 	gauge_getGaugeLayout()
{
	return currentLayout;
}


void gauge_saveGauge(GAUGE  * g, unsigned char gauge_id)
{

GAUGE g1;
memcpy(&g1,g,sizeof(GAUGE));

	//check for each gauge id
	if (gauge_id== DIGI_GAUGE1_ID)
		{
		eeprom_write_block(&g1,&ee_digi_gauge1,sizeof(GAUGE));
		eeprom_write_byte(&ee_digi_gauge1Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== DIGI_GAUGE2_ID)
		{
		eeprom_write_block(&g1,&ee_digi_gauge2,sizeof(GAUGE));
		eeprom_write_byte(&ee_digi_gauge2Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== DIGI_GAUGE3_ID)
		{
		eeprom_write_block(&g1,&ee_digi_gauge3,sizeof(GAUGE));
		eeprom_write_byte(&ee_digi_gauge3Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== DIGI_GAUGE4_ID)
		{
		eeprom_write_block(&g1,&ee_digi_gauge4,sizeof(GAUGE));
		eeprom_write_byte(&ee_digi_gauge4Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== BAR_G1_ID)
		{
		eeprom_write_block(&g1,&ee_bar_g1,sizeof(GAUGE));
		eeprom_write_byte(&ee_bar_g1Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== BAR_G2_ID)
		{
		eeprom_write_block(&g1,&ee_bar_g2,sizeof(GAUGE));
		eeprom_write_byte(&ee_bar_g2Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== BAR_G3_ID)
		{
		eeprom_write_block(&g1,&ee_bar_g3,sizeof(GAUGE));
		eeprom_write_byte(&ee_bar_g3Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== ANALOG_G1_ID)
		{
		eeprom_write_block(&g1,&ee_analog_g1,sizeof(GAUGE));
		eeprom_write_byte(&ee_analog_g1Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== SCOPE_G1_ID)
		{
		eeprom_write_block(&g1,&ee_scope_g1,sizeof(GAUGE));
		eeprom_write_byte(&ee_scope_g1Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== SCOPE_G2_ID)
		{
		eeprom_write_block(&g1,&ee_scope_g2,sizeof(GAUGE));
		eeprom_write_byte(&ee_scope_g2Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== XLDIGI_GAUGE1_ID)
		{
		eeprom_write_block(&g1,&ee_XLdigi_gauge1,sizeof(GAUGE));
		eeprom_write_byte(&ee_XLdigi_gauge1Saved,EE_GAUGE_SAVED);		
		}
	else
	if (gauge_id== TUNING_DIGI_GAUGE1_ID)
		{
		eeprom_write_block(&g1,&ee_tuning_digi_gauge1,sizeof(GAUGE));
		eeprom_write_byte(&ee_tuning_digi_gauge1Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== TUNING_DIGI_GAUGE2_ID)
		{
		eeprom_write_block(&g1,&ee_tuning_digi_gauge2,sizeof(GAUGE));
		eeprom_write_byte(&ee_tuning_digi_gauge2Saved,EE_GAUGE_SAVED);
		}
	else
	if (gauge_id== TUNING_DIGI_GAUGE3_ID)
		{
		eeprom_write_block(&g1,&ee_tuning_digi_gauge3,sizeof(GAUGE));
		eeprom_write_byte(&ee_tuning_digi_gauge3Saved,EE_GAUGE_SAVED);
		}
}

void gauge_getGaugeDescription(char * dest_str,unsigned char gauge_id)
{

//Descriptions will be different depending on the current layout

if (currentLayout == LAYOUT_STANDARD)
	{

	//check for each gauge id
	if (gauge_id== DIGI_GAUGE1_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Digital Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== DIGI_GAUGE2_ID)
		{
		strncpy_P(dest_str,PSTR("Top Digital Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Top Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G2_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== ANALOG_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Analog Gauge"),DESCRIPTION_MAX_LENGTH);
		}


	}
else
if (currentLayout == LAYOUT_ANALOG)
	{
	;
	}
else
if (currentLayout == LAYOUT_DIGITAL)
	{

	if (gauge_id== DIGI_GAUGE1_ID)
		{
		strncpy_P(dest_str,PSTR("Top-Left Digital Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== DIGI_GAUGE2_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom-Left Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== DIGI_GAUGE3_ID)
		{
		strncpy_P(dest_str,PSTR("Top-Right Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== DIGI_GAUGE4_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom-Right Gauge"),DESCRIPTION_MAX_LENGTH);
		}


	}
else
if (currentLayout == LAYOUT_SCOPE)
	{
	if (gauge_id== SCOPE_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Scope Gauge, Top"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== SCOPE_G2_ID)
		{
		strncpy_P(dest_str,PSTR("Scope Gauge, Bottom"),DESCRIPTION_MAX_LENGTH);
		}
	}
else
if (currentLayout == LAYOUT_BAR)
	{

	if (gauge_id== BAR_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Top Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G2_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}

	}
else

if (currentLayout == LAYOUT_3BAR)
	{

	if (gauge_id== BAR_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Top Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G2_ID)
		{
		strncpy_P(dest_str,PSTR("Middle Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G3_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}

	}
else

if (currentLayout == LAYOUT_SCOPE_MIX)
	{

	if (gauge_id== DIGI_GAUGE1_ID)
		{
		strncpy_P(dest_str,PSTR("Top Digital Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== DIGI_GAUGE2_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== SCOPE_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Scope Gauge"),DESCRIPTION_MAX_LENGTH);
		}


	}
else

if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
	{

	if (gauge_id== ANALOG_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Analog Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== SCOPE_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Scope Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G1_ID)
		{
		strncpy_P(dest_str,PSTR("Top Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	else
	if (gauge_id== BAR_G2_ID)
		{
		strncpy_P(dest_str,PSTR("Bottom Bar Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	}
else

if (currentLayout == LAYOUT_XLDIGITAL)
	{
	
	if (gauge_id== XLDIGI_GAUGE1_ID)
		{
		strncpy_P(dest_str,PSTR("Digital Gauge"),DESCRIPTION_MAX_LENGTH);
		}
	}

} //end gauge_getGaugeDescription()






GAUGE* gauge_checkPointInGauge(POINT p, unsigned char * gauge_id, unsigned char ** alarm)
{
//depending on current_layout
if (currentLayout == LAYOUT_STANDARD)
	{
	if ( pointInRect(p, digi_gauge1.bounds) )
		{
		lcd_fill_rect(digi_gauge1.bounds,0xFF);
		*gauge_id = DIGI_GAUGE1_ID;
		*alarm = &digi_gauge1.alarm.status;
		return &digi_gauge1.value;
		}
	else

	if ( pointInRect(p, digi_gauge2.bounds) )
		{
		lcd_fill_rect(digi_gauge2.bounds,0xFF);
		*gauge_id = DIGI_GAUGE2_ID;
		*alarm = &digi_gauge2.alarm.status;
		return &digi_gauge2.value;
		}
	else

	if ( pointInRect(p, bar_g1.bounds) )
		{
		lcd_fill_rect(bar_g1.bounds,0xFF);
		*gauge_id = BAR_G1_ID;
		*alarm = &bar_g1.alarm.status;
		return &bar_g1.value;
		}
	else

	if ( pointInRect(p,bar_g2.bounds) )
		{
		lcd_fill_rect(bar_g2.bounds,0xFF);
		*gauge_id = BAR_G2_ID;
		*alarm = &bar_g2.alarm.status;
		return &bar_g2.value;
		}
	else

	if ( pointInRect(p,analog_g1.bounds) )
		{
		lcd_fill_rect(analog_g1.bounds,0xFF);
		*gauge_id = ANALOG_G1_ID;
		*alarm = &analog_g1.alarm.status;
		return &analog_g1.value;
		}
	}
else


if (currentLayout == LAYOUT_ANALOG)
	{
	;
	}
else


if (currentLayout == LAYOUT_DIGITAL)
	{

	if ( pointInRect(p, digi_gauge1.bounds) )
		{
		lcd_fill_rect(digi_gauge1.bounds,0xFF);
		*gauge_id = DIGI_GAUGE1_ID;
		*alarm = &digi_gauge1.alarm.status;
		return &digi_gauge1.value;
		}
	else

	if ( pointInRect(p, digi_gauge2.bounds) )
		{
		lcd_fill_rect(digi_gauge2.bounds,0xFF);
		*gauge_id = DIGI_GAUGE2_ID;
		*alarm = &digi_gauge2.alarm.status;
		return &digi_gauge2.value;
		}
	else

	if ( pointInRect(p, digi_gauge3.bounds) )
		{
		lcd_fill_rect(digi_gauge3.bounds,0xFF);
		*gauge_id = DIGI_GAUGE3_ID;
		*alarm = &digi_gauge3.alarm.status;
		return &digi_gauge3.value;
		}
	else

	if ( pointInRect(p, digi_gauge4.bounds) )
		{
		lcd_fill_rect(digi_gauge4.bounds,0xFF);
		*gauge_id = DIGI_GAUGE4_ID;
		*alarm = &digi_gauge4.alarm.status;
		return &digi_gauge4.value;
		}

	}
else


if (currentLayout == LAYOUT_SCOPE)
	{
	if ( pointInRect(p,scope_g1.bounds) )
		{
		lcd_fill_rect(scope_g1.bounds,0xFF);
		*gauge_id = SCOPE_G1_ID;
		*alarm = &scope_g1.alarm.status;
		return &scope_g1.value;
		}
	else
	if ( pointInRect(p,scope_g2.bounds) )
		{
		lcd_fill_rect(scope_g2.bounds,0xFF);
		*gauge_id = SCOPE_G2_ID;
		*alarm = &scope_g2.alarm.status;
		return &scope_g2.value;
		}
	}
else

if (currentLayout == LAYOUT_BAR)
	{

	if ( pointInRect(p, bar_g1.bounds) )
		{
		lcd_fill_rect(bar_g1.bounds,0xFF);
		*gauge_id = BAR_G1_ID;
		*alarm = &bar_g1.alarm.status;
		return &bar_g1.value;
		}
	else

	if ( pointInRect(p,bar_g2.bounds) )
		{
		lcd_fill_rect(bar_g2.bounds,0xFF);
		*gauge_id = BAR_G2_ID;
		*alarm = &bar_g2.alarm.status;
		return &bar_g2.value;
		}

	}
else

if (currentLayout == LAYOUT_3BAR)
	{

	if ( pointInRect(p, bar_g1.bounds) )
		{
		lcd_fill_rect(bar_g1.bounds,0xFF);
		*gauge_id = BAR_G1_ID;
		*alarm = &bar_g1.alarm.status;
		return &bar_g1.value;
		}
	else

	if ( pointInRect(p,bar_g2.bounds) )
		{
		lcd_fill_rect(bar_g2.bounds,0xFF);
		*gauge_id = BAR_G2_ID;
		*alarm = &bar_g2.alarm.status;
		return &bar_g2.value;
		}
	else

	if ( pointInRect(p,bar_g3.bounds) )
		{
		lcd_fill_rect(bar_g3.bounds,0xFF);
		*gauge_id = BAR_G3_ID;
		*alarm = &bar_g3.alarm.status;
		return &bar_g3.value;
		}

	}
else

if (currentLayout == LAYOUT_SCOPE_MIX)
	{

	if ( pointInRect(p, digi_gauge1.bounds) )
		{
		lcd_fill_rect(digi_gauge1.bounds,0xFF);
		*gauge_id = DIGI_GAUGE1_ID;
		*alarm = &digi_gauge1.alarm.status;
		return &digi_gauge1.value;
		}
	else

	if ( pointInRect(p, digi_gauge2.bounds) )
		{
		lcd_fill_rect(digi_gauge2.bounds,0xFF);
		*gauge_id = DIGI_GAUGE2_ID;
		*alarm = &digi_gauge2.alarm.status;
		return &digi_gauge2.value;
		}
	else
	if ( pointInRect(p,scope_g1.bounds) )
		{
		lcd_fill_rect(scope_g1.bounds,0xFF);
		*gauge_id = SCOPE_G1_ID;
		*alarm = &scope_g1.alarm.status;
		return &scope_g1.value;
		}


	}
else

if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
	{

	if ( pointInRect(p,scope_g1.bounds) )
		{
		lcd_fill_rect(scope_g1.bounds,0xFF);
		*gauge_id = SCOPE_G1_ID;
		*alarm = &scope_g1.alarm.status;
		return &scope_g1.value;
		}
	else
	if ( pointInRect(p, bar_g1.bounds) )
		{
		lcd_fill_rect(bar_g1.bounds,0xFF);
		*gauge_id = BAR_G1_ID;
		*alarm = &bar_g1.alarm.status;
		return &bar_g1.value;
		}
	else
	if ( pointInRect(p,bar_g2.bounds) )
		{
		lcd_fill_rect(bar_g2.bounds,0xFF);
		*gauge_id = BAR_G2_ID;
		*alarm = &bar_g2.alarm.status;
		return &bar_g2.value;
		}
	else
	if ( pointInRect(p,analog_g1.bounds) )
		{
		lcd_fill_rect(analog_g1.bounds,0xFF);
		*gauge_id = ANALOG_G1_ID;
		*alarm = &analog_g1.alarm.status;
		return &analog_g1.value;
		}


	}
else

if (currentLayout == LAYOUT_XLDIGITAL)
	{

	if ( pointInRect(p, XLdigi_gauge1.bounds) )
		{
		lcd_fill_rect(XLdigi_gauge1.bounds,0xFF);
		*gauge_id = XLDIGI_GAUGE1_ID;
		*alarm = &XLdigi_gauge1.alarm.status;
		return &XLdigi_gauge1.value;
		}
	}


return FALSE;
}



void gauge_restoreLayout()
{

unsigned char layout = eeprom_read_byte(&ee_currentLayout);


if (layout == LAYOUT_STANDARD)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_DIGITAL)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_SCOPE)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_BAR)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_3BAR)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_SCOPE_MIX)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_SCOPE_BAR_ANALOG)
	{
	gauge_changeGaugeLayout(layout);
	}
else
if (layout == LAYOUT_XLDIGITAL)
	{
	gauge_changeGaugeLayout(layout);
	}
else
//if (layout == LAYOUT_TUNING)
//	{
//	gauge_changeGaugeLayout(layout);
//	}
//else
	{
	gauge_changeGaugeLayout(LAYOUT_STANDARD);
	}

}




unsigned char gauge_selectGaugeToChange()
{
POINT p;

//Letting them select a gauge to modify
//maybe display a message here telling them what to do
lcd_clear_graph();
gauge_drawGaugeLayout(); //draw the current layout
messageBar_addMsg("Tap a gauge to change"); 

	while(1)
		{
		if (touch_get_cursor(&p))
			{

			GAUGE * g;
			unsigned char gauge_id;
			unsigned char * alarm;
						
			g = gauge_checkPointInGauge(p,&gauge_id,&alarm);
			if (g)
				{
				delay_ms(200); //a wait for selection visibility
				 
				char description[DESCRIPTION_MAX_LENGTH]; //storage for the description
				gauge_getGaugeDescription(description,gauge_id); //get the description


				//////////////////////////////////////
				//They chose a gauge, now lets ask them some stuff
				//Make a menu for the user to choose...
				//-change the min/max
				//-change the parameter
				////////////////////////////////////////
				unsigned char menu_result;
				menu_result = menu_modify_gauge(description, g,alarm); //find out which they want to do

					lcd_clear_graph();
					gauge_saveGauge(g,gauge_id);
				
				if (menu_result == MENU_CHANGE_MINMAX)
					{
					; //gauge_changeMinMax(g, description);
					float num;
					
				//get the Minimum value
					num = menu_getNumberFromUser("Enter Minimum Value");
					g->min = num;

				//get the Maximum Value
					num = menu_getNumberFromUser("Enter Maximum Value");
					g->max = num;
					lcd_clear_graph();
					gauge_saveGauge(g,gauge_id);
					messageBar_addMsg(" ");
					return EXIT_MENU;
					}
				else
				if (menu_result == MENU_CHANGE_PARAMETER)
					{
					menu_selectParameter(g, description);			
					lcd_clear_graph();
					gauge_saveGauge(g,gauge_id);
					messageBar_addMsg(" ");
					return EXIT_MENU;
					}
				else
					{
					return menu_result; //let the previous menu handle this !!need to implement this
					}
				

				}

			}
		}//end while

return EXIT_MENU; //shouldn't get here
} //end gauge_changeGauge







GAUGE** gauge_getGaugePointers(unsigned char * count)
{


	if (currentLayout == LAYOUT_STANDARD)
		{
		*count = 5;	
		}
	else
	if (currentLayout == LAYOUT_ANALOG)
		{
		*count = 0;
		}
	else
	if (currentLayout == LAYOUT_DIGITAL)
		{
		*count = 4;
		}
	else
	if (currentLayout == LAYOUT_SCOPE)
		{
		*count = 2;
		}
	else
	if (currentLayout == LAYOUT_BAR)
		{
		*count = 2;
		}
	else
	if (currentLayout == LAYOUT_3BAR)
		{
		*count = 3;
		}
	else
	if (currentLayout == LAYOUT_SCOPE_MIX)
		{
		*count = 3;
		}
	else
	if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
		{
		*count = 4;
		}
	else
	if (currentLayout == LAYOUT_XLDIGITAL)
		{
		*count = 1;
		}
	else
	if (currentLayout == LAYOUT_TUNING)
		{
		*count = 3;
		}

	return gauge_PTR_array;

} //end gauge_getGaugePointers()




void gauge_refreshGaugeLayout()
{

	if (currentLayout == LAYOUT_STANDARD)
		{
			gauge_analogGauge_refresh(&analog_g1); //refresh me with a new value
			gauge_digiGauge_refresh(&digi_gauge1);
			gauge_digiGauge_refresh(&digi_gauge2);		
			gauge_barGauge_refresh(&bar_g1);
			gauge_barGauge_refresh(&bar_g2);
		}
	else
	if (currentLayout == LAYOUT_ANALOG)
		{
		;
		}
	else
	if (currentLayout == LAYOUT_DIGITAL)
		{
			gauge_digiGauge_refresh(&digi_gauge1);
			gauge_digiGauge_refresh(&digi_gauge2);	
			gauge_digiGauge_refresh(&digi_gauge3);
			gauge_digiGauge_refresh(&digi_gauge4);
			lcd_line(5,60,234,60,1); //horiz
			lcd_line(115,3,115,116,1); //vert	
		}
	else
	if (currentLayout == LAYOUT_SCOPE)
		{
		gauge_scopeGauge_refresh(&scope_g1);
		gauge_scopeGauge_refresh(&scope_g2);
		}
	else
	if (currentLayout == LAYOUT_BAR)
		{
		gauge_barGauge_refresh(&bar_g1);
		gauge_barGauge_refresh(&bar_g2);		
		}
	else
	if (currentLayout == LAYOUT_3BAR)
		{
		gauge_barGauge_refresh(&bar_g1);
		gauge_barGauge_refresh(&bar_g2);
		gauge_barGauge_refresh(&bar_g3);		
		}
	else
	if (currentLayout == LAYOUT_SCOPE_MIX)
		{
			gauge_digiGauge_refresh(&digi_gauge1);
			gauge_digiGauge_refresh(&digi_gauge2);	
			gauge_scopeGauge_refresh(&scope_g1);
		}
	else
	if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
		{
		gauge_scopeGauge_refresh(&scope_g1);
		gauge_barGauge_refresh(&bar_g1);
		gauge_barGauge_refresh(&bar_g2);
		gauge_analogGauge_refresh(&analog_g1); //refresh me with a new value
		}
	else
	if (currentLayout == LAYOUT_XLDIGITAL)
		{
			gauge_XLdigiGauge_refresh(&XLdigi_gauge1);
		}
	else
	if (currentLayout == LAYOUT_TUNING)
		{
			gauge_digiGauge_refresh(&tuning_digi_gauge1);
			gauge_digiGauge_refresh(&tuning_digi_gauge2);	
			gauge_digiGauge_refresh(&tuning_digi_gauge3);
		}
}


void gauge_changeGaugeLayout(unsigned char the_layout)
{

currentLayout = the_layout; //set the current layout in RAM

//setup the gauges for the desired layout


//add a message to help them on the message bar
	eeprom_write_byte(&ee_currentLayout,currentLayout); //burn the current layout to eeprom
	char out[40];
	strncpy_P(out,PSTR("Tap Menu->Gauge Settings->Change Gauges"),40);




///////////////////////////////////////////////////////////
///
/// Standard Layout
///
///////////////////////////////////////////////////////////
	if (currentLayout == LAYOUT_STANDARD)
		{
			//---analog_g1
			gauge_analogGauge_buildLookup(50); //build the analog gauge sin/cos lookup table with this radius
			analog_g1.bounds.left = 0;
			analog_g1.bounds.top = 0;
			analog_g1.bounds.bottom = 114;
			analog_g1.bounds.right = 100;
			analog_g1.needle_center.x = 53;
			analog_g1.needle_center.y = 63;
			if (eeprom_read_byte(&ee_analog_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&analog_g1.value,&ee_analog_g1, sizeof(GAUGE));
			else
				gauge_getGauge(&analog_g1.value,analog_g1.value.param_id);


			//---Bar Graph 1
			bar_g1.bounds.left = 100;
			bar_g1.bounds.right = 237;
			bar_g1.bounds.top = 8;
			bar_g1.bounds.bottom = 14;
			if (eeprom_read_byte(&ee_bar_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g1.value,&ee_bar_g1, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g1.value,bar_g1.value.param_id);	//restore gauge from progmem


			//---Bar Graph 2
			bar_g2.bounds.left = 100;
			bar_g2.bounds.right = 237;
			bar_g2.bounds.top = 33;
			bar_g2.bounds.bottom = 39;
			if (eeprom_read_byte(&ee_bar_g2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g2.value,&ee_bar_g2, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g2.value,bar_g2.value.param_id);	//restore gauge from progmem
			

			//---DigiGauge 1
			digi_gauge1.bounds.left = 96;
			digi_gauge1.bounds.right = 180;
			digi_gauge1.bounds.top = 92;
			digi_gauge1.bounds.bottom = 115; 
			if (eeprom_read_byte(&ee_digi_gauge1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge1.value,&ee_digi_gauge1, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge1.value,digi_gauge1.value.param_id);	//restore gauge from progmem
			

			//---DigiGauge 2
			digi_gauge2.bounds.left = 96;
			digi_gauge2.bounds.right = 180;
			digi_gauge2.bounds.top = 60;
			digi_gauge2.bounds.bottom = 90; 
			if (eeprom_read_byte(&ee_digi_gauge2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge2.value,&ee_digi_gauge2, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge2.value,digi_gauge2.value.param_id);	//restore gauge from progmem
			

			//build the gauge pointer table; storing each gauge
			gauge_PTR_array[0] = &analog_g1.value;
			gauge_PTR_array[1] = &bar_g1.value;
			gauge_PTR_array[2] = &bar_g2.value;
			gauge_PTR_array[3] = &digi_gauge1.value;
			gauge_PTR_array[4] = &digi_gauge2.value;

			messageBar_addMsg(out);

		}
	else

	if (currentLayout == LAYOUT_ANALOG)
		{
		;
		}
	else

	if (currentLayout == LAYOUT_DIGITAL)
		{


			//---DigiGauge 1
			digi_gauge1.bounds.left = 8;
			digi_gauge1.bounds.right = 100;
			digi_gauge1.bounds.top = 20;
			digi_gauge1.bounds.bottom = 52; 
			if (eeprom_read_byte(&ee_digi_gauge1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge1.value,&ee_digi_gauge1, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge1.value,digi_gauge1.value.param_id);	//restore gauge from progmem
			

			//---DigiGauge 2
			digi_gauge2.bounds.left = 8;
			digi_gauge2.bounds.right = 100;
			digi_gauge2.bounds.top = 80;
			digi_gauge2.bounds.bottom = 112; 
			if (eeprom_read_byte(&ee_digi_gauge2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge2.value,&ee_digi_gauge2, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge2.value,digi_gauge2.value.param_id);	//restore gauge from progmem


			//---DigiGauge 3
			digi_gauge3.bounds.left = 120;
			digi_gauge3.bounds.right = 210;
			digi_gauge3.bounds.top = 20;
			digi_gauge3.bounds.bottom = 52; 
			if (eeprom_read_byte(&ee_digi_gauge3Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge3.value,&ee_digi_gauge3, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge3.value,digi_gauge3.value.param_id);	//restore gauge from progmem


			//---DigiGauge 4
			digi_gauge4.bounds.left = 120;
			digi_gauge4.bounds.right = 210;
			digi_gauge4.bounds.top = 80;
			digi_gauge4.bounds.bottom = 112; 
			if (eeprom_read_byte(&ee_digi_gauge4Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge4.value,&ee_digi_gauge4, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge4.value,digi_gauge4.value.param_id);	//restore gauge from progmem




			gauge_PTR_array[0] = &digi_gauge1.value;
			gauge_PTR_array[1] = &digi_gauge2.value;
			gauge_PTR_array[2] = &digi_gauge3.value;
			gauge_PTR_array[3] = &digi_gauge4.value;

			messageBar_addMsg(out);
		}
	else

	if (currentLayout == LAYOUT_SCOPE)
		{



		//scope gauge 1
		scope_g1.bounds.left = 31;
		scope_g1.bounds.top  = 5;
		scope_g1.bounds.bottom = 55;
		scope_g1.bounds.right = SCOPE_MAX_POINTS;
		
		if (eeprom_read_byte(&ee_scope_g1Saved) == EE_GAUGE_SAVED)
			{
			eeprom_read_block(&scope_g1.value,&ee_scope_g1, sizeof(GAUGE)); //restore gauge from eeprom
			}
		else
			{
			scope_g1.value.param_id = 0;
			gauge_getGauge(&scope_g1.value,scope_g1.value.param_id);	//restore gauge from progmem
			}


		//scope gauge 2
		scope_g2.bounds.left = 31;
		scope_g2.bounds.top  = 60;
		scope_g2.bounds.bottom = 110;
		scope_g2.bounds.right = SCOPE_MAX_POINTS;
		scope_g2.value.param_id = 0;
		gauge_getGauge(&scope_g2.value,scope_g2.value.param_id);
		
		if (eeprom_read_byte(&ee_scope_g2Saved) == EE_GAUGE_SAVED)
			{
			eeprom_read_block(&scope_g2.value,&ee_scope_g2, sizeof(GAUGE)); //restore gauge from eeprom
			}
		else
			{
			scope_g2.value.param_id = 0;
			gauge_getGauge(&scope_g2.value,scope_g2.value.param_id);	//restore gauge from progmem
			}		
		


		//build the gauge pointer table; storing each gauge
		gauge_PTR_array[0] = &scope_g1.value;
		gauge_PTR_array[1] = &scope_g2.value;
		
		messageBar_addMsg(out);		
				
		}
	else


	if (currentLayout == LAYOUT_BAR)
		{




			//---Bar Graph 1
			bar_g1.bounds.left = 5;
			bar_g1.bounds.right = 234;
			bar_g1.bounds.top = 10;
			bar_g1.bounds.bottom = 40;
			if (eeprom_read_byte(&ee_bar_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g1.value,&ee_bar_g1, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g1.value,bar_g1.value.param_id);	//restore gauge from progmem


			//---Bar Graph 2
			bar_g2.bounds.left = 5;
			bar_g2.bounds.right = 234;
			bar_g2.bounds.top = 70;
			bar_g2.bounds.bottom = 100;
			if (eeprom_read_byte(&ee_bar_g2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g2.value,&ee_bar_g2, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g2.value,bar_g2.value.param_id);	//restore gauge from progmem
		
		
		//build the gauge pointer table; storing each gauge
		gauge_PTR_array[0] = &bar_g1.value;
		gauge_PTR_array[1] = &bar_g2.value;

		messageBar_addMsg(out);

		}
	else

	if (currentLayout == LAYOUT_3BAR)
		{




			//---Bar Graph 1
			bar_g1.bounds.left = 5;
			bar_g1.bounds.right = 234;
			bar_g1.bounds.top = 10;
			bar_g1.bounds.bottom = 30;
			if (eeprom_read_byte(&ee_bar_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g1.value,&ee_bar_g1, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g1.value,bar_g1.value.param_id);	//restore gauge from progmem


			//---Bar Graph 2
			bar_g2.bounds.left = 5;
			bar_g2.bounds.right = 234;
			bar_g2.bounds.top = 49;
			bar_g2.bounds.bottom = 69;
			if (eeprom_read_byte(&ee_bar_g2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g2.value,&ee_bar_g2, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g2.value,bar_g2.value.param_id);	//restore gauge from progmem

			//---Bar Graph 3
			bar_g3.bounds.left = 5;
			bar_g3.bounds.right = 234;
			bar_g3.bounds.top = 88;
			bar_g3.bounds.bottom = 108;
			if (eeprom_read_byte(&ee_bar_g3Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g3.value,&ee_bar_g3, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g3.value,bar_g3.value.param_id);	//restore gauge from progmem
		
		
		//build the gauge pointer table; storing each gauge
		gauge_PTR_array[0] = &bar_g1.value;
		gauge_PTR_array[1] = &bar_g2.value;
		gauge_PTR_array[2] = &bar_g3.value;

		messageBar_addMsg(out);

		}
	else


	if (currentLayout == LAYOUT_SCOPE_MIX)
		{

			//---DigiGauge 1
			digi_gauge1.bounds.left = 8;
			digi_gauge1.bounds.right = 100;
			digi_gauge1.bounds.top = 20;
			digi_gauge1.bounds.bottom = 52; 
			if (eeprom_read_byte(&ee_digi_gauge1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge1.value,&ee_digi_gauge1, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge1.value,digi_gauge1.value.param_id);	//restore gauge from progmem
			

			//---DigiGauge 2
			digi_gauge2.bounds.left = 10;
			digi_gauge2.bounds.right = 102;
			digi_gauge2.bounds.top = 80;
			digi_gauge2.bounds.bottom = 110; 
			if (eeprom_read_byte(&ee_digi_gauge2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&digi_gauge2.value,&ee_digi_gauge2, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&digi_gauge2.value,digi_gauge2.value.param_id);	//restore gauge from progmem



			//scope gauge 1
			scope_g1.bounds.left = 128;
			scope_g1.bounds.top  = 3;
			scope_g1.bounds.bottom = 100;
			scope_g1.bounds.right = SCOPE_MAX_POINTS;
		
			if (eeprom_read_byte(&ee_scope_g1Saved) == EE_GAUGE_SAVED)
				{
				eeprom_read_block(&scope_g1.value,&ee_scope_g1, sizeof(GAUGE)); //restore gauge from eeprom
				}
			else
				{
				scope_g1.value.param_id = 0;
				gauge_getGauge(&scope_g1.value,scope_g1.value.param_id);	//restore gauge from progmem
				}
		

		//build the gauge pointer table; storing each gauge
		gauge_PTR_array[0] = &scope_g1.value;
		gauge_PTR_array[1] = &digi_gauge1.value;
		gauge_PTR_array[2] = &digi_gauge2.value;

		messageBar_addMsg(out);
		}
	else

	if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
		{
			//---analog_g1
			gauge_analogGauge_buildLookup(50); //build the analog gauge sin/cos lookup table with this radius
			analog_g1.bounds.left = 0;
			analog_g1.bounds.top = 0;
			analog_g1.bounds.bottom = 114;
			analog_g1.bounds.right = 100;
			analog_g1.needle_center.x = 53;
			analog_g1.needle_center.y = 63;
			if (eeprom_read_byte(&ee_analog_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&analog_g1.value,&ee_analog_g1, sizeof(GAUGE));
			else
				gauge_getGauge(&analog_g1.value,analog_g1.value.param_id);


			//---Bar Graph 1
			bar_g1.bounds.left = 100;
			bar_g1.bounds.right = 237;
			bar_g1.bounds.top = 8;
			bar_g1.bounds.bottom = 14;
			if (eeprom_read_byte(&ee_bar_g1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g1.value,&ee_bar_g1, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g1.value,bar_g1.value.param_id);	//restore gauge from progmem


			//---Bar Graph 2
			bar_g2.bounds.left = 100;
			bar_g2.bounds.right = 237;
			bar_g2.bounds.top = 33;
			bar_g2.bounds.bottom = 39;
			if (eeprom_read_byte(&ee_bar_g2Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&bar_g2.value,&ee_bar_g2, sizeof(GAUGE));  //restore gauge from eeprom
			else
				gauge_getGauge(&bar_g2.value,bar_g2.value.param_id);	//restore gauge from progmem


			//scope gauge 1
			scope_g1.bounds.left = 120;
			scope_g1.bounds.top  = 60;
			scope_g1.bounds.bottom = 115;
			scope_g1.bounds.right = SCOPE_MAX_POINTS;
		
			if (eeprom_read_byte(&ee_scope_g1Saved) == EE_GAUGE_SAVED)
				{
				eeprom_read_block(&scope_g1.value,&ee_scope_g1, sizeof(GAUGE)); //restore gauge from eeprom
				}
			else
				{
				scope_g1.value.param_id = 0;
				gauge_getGauge(&scope_g1.value,scope_g1.value.param_id);	//restore gauge from progmem
				}
			

			//build the gauge pointer table; storing each gauge
			gauge_PTR_array[0] = &analog_g1.value;
			gauge_PTR_array[1] = &bar_g1.value;
			gauge_PTR_array[2] = &bar_g2.value;
			gauge_PTR_array[3] = &scope_g1.value;

			messageBar_addMsg(out);
		}
	else

	if (currentLayout == LAYOUT_XLDIGITAL)
		{


			//---XLDigiGauge 1
			XLdigi_gauge1.bounds.left = 0;
			XLdigi_gauge1.bounds.right = 239;
			XLdigi_gauge1.bounds.top = 16;
			XLdigi_gauge1.bounds.bottom = 100; 
			if (eeprom_read_byte(&ee_XLdigi_gauge1Saved) == EE_GAUGE_SAVED)
				eeprom_read_block(&XLdigi_gauge1.value,&ee_XLdigi_gauge1, sizeof(GAUGE)); //restore gauge from eeprom
			else
				gauge_getGauge(&XLdigi_gauge1.value,XLdigi_gauge1.value.param_id);	//restore gauge from progmem
			
			gauge_PTR_array[0] = &XLdigi_gauge1.value;
			
			messageBar_addMsg(out);
		}
	else

		if (currentLayout == LAYOUT_TUNING)
		{


			//---DigiGauge 1
			tuning_digi_gauge1.bounds.left = 0;
			tuning_digi_gauge1.bounds.right = 100;
			tuning_digi_gauge1.bounds.top = 9;
			tuning_digi_gauge1.bounds.bottom = 52; 
				gauge_getGauge(&tuning_digi_gauge1.value,ms2_RPM);	//restore gauge from progmem
			

			//---DigiGauge 2
			tuning_digi_gauge2.bounds.left = 0;
			tuning_digi_gauge2.bounds.right = 100;
			tuning_digi_gauge2.bounds.top = 43;
			tuning_digi_gauge2.bounds.bottom = 112; 
				gauge_getGauge(&tuning_digi_gauge2.value,ms2_AFR1);	//restore gauge from progmem


			//---DigiGauge 3
			tuning_digi_gauge3.bounds.left = 0;
			tuning_digi_gauge3.bounds.right = 100;
			tuning_digi_gauge3.bounds.top = 76;
			tuning_digi_gauge3.bounds.bottom = 127; 
				gauge_getGauge(&tuning_digi_gauge3.value,ms2_MAP);	//restore gauge from progmem


			gauge_PTR_array[0] = &tuning_digi_gauge1.value;
			gauge_PTR_array[1] = &tuning_digi_gauge2.value;
			gauge_PTR_array[2] = &tuning_digi_gauge3.value;
	}




	gauge_drawGaugeLayout(); //now draw the layout

}





void gauge_drawGaugeLayout()
{

if (currentLayout == LAYOUT_STANDARD)
	{
		gauge_barGauge_update(&bar_g1);
		gauge_barGauge_update(&bar_g2);
		gauge_digiGauge_update(&digi_gauge1);
		gauge_digiGauge_update(&digi_gauge2);
		gauge_analogGauge_update(&analog_g1);
		messageBar_refresh();	
	}
else
if (currentLayout == LAYOUT_ANALOG)
	{
	;
	}
else
if (currentLayout == LAYOUT_DIGITAL)
	{
		gauge_digiGauge_update(&digi_gauge1);
		gauge_digiGauge_update(&digi_gauge2);
		gauge_digiGauge_update(&digi_gauge3);
		gauge_digiGauge_update(&digi_gauge4);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_SCOPE)
	{
		gauge_scopeGauge_update(&scope_g1);
		gauge_scopeGauge_update(&scope_g2);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_BAR)
	{
		gauge_barGauge_update(&bar_g1);
		gauge_barGauge_update(&bar_g2);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_3BAR)
	{
		gauge_barGauge_update(&bar_g1);
		gauge_barGauge_update(&bar_g2);
		gauge_barGauge_update(&bar_g3);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_SCOPE_MIX)
	{
		lcd_line(0,58,90,58,1);
		lcd_line(0,57,90,57,1);
		lcd_line(90,0,90,120,1);
		lcd_line(91,0,91,120,1);
		gauge_digiGauge_update(&digi_gauge1);
		gauge_digiGauge_update(&digi_gauge2);
		gauge_scopeGauge_update(&scope_g1);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
	{
		gauge_barGauge_update(&bar_g1);
		gauge_barGauge_update(&bar_g2);	
		gauge_scopeGauge_update(&scope_g1);
		gauge_analogGauge_update(&analog_g1);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_XLDIGITAL)
	{
		gauge_XLdigiGauge_update(&XLdigi_gauge1);
		messageBar_refresh();
	}
else
if (currentLayout == LAYOUT_TUNING)
	{
		gauge_digiGauge_update(&tuning_digi_gauge1);
		gauge_digiGauge_update(&tuning_digi_gauge2);
		gauge_digiGauge_update(&tuning_digi_gauge3);
	}
}



void gauge_getUnitsDescription(char * dest, unsigned char units_id)
{

		switch (units_id)
			{
			case UNITS_KPA:
			strncpy_P(dest,PSTR("KPA"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;
			case UNITS_PSI:
			strncpy_P(dest,PSTR("PSI"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;
			case UNITS_CELCIUS:
			strncpy_P(dest,PSTR("Celcius"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;	
			case UNITS_FAREN:
			strncpy_P(dest,PSTR("Faren"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;			
			case UNITS_KPH:
			strncpy_P(dest,PSTR("KPH"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;
			case UNITS_MPH:
			strncpy_P(dest,PSTR("MPH"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			break;
			default:
			strncpy_P(dest,PSTR("None"),GAUGE_UNITS_DESCRIPTION_LENGTH);
			}

}

unsigned char gauge_getOtherUnits(unsigned char units_id)
{

		switch (units_id)
			{
			case UNITS_KPA:
				return UNITS_PSI;
			break;
			case UNITS_PSI:
				return UNITS_KPA;
			break;

			case UNITS_CELCIUS:
				return UNITS_FAREN;
			break;
			case UNITS_FAREN:
				return UNITS_CELCIUS;
			break;
						
			case UNITS_KPH:
				return UNITS_MPH;
			break;
			case UNITS_MPH:
				return UNITS_KPH;
			break;

			default:
				return 0;
			break;
			}
}



void gauge_convert_units(float* number,unsigned char units, unsigned char user_units)
{

	//check on the units
	if (units != user_units)
		{
		switch (user_units)
			{
			case UNITS_PSI:
			//KPA -> PSI
				if (*number > 100)
					{
					//conver to psi
					*number = (*number - 100) * 0.1450377;
					}
				else
					{
					//convert to mmHg
					*number = (100 - *number) * 0.2953007 * -1;
					}
			break;
			case UNITS_CELCIUS:
			//FAREN -> CELCIUS
			*number = (*number - 32) * 0.555;
			break;			
			case UNITS_KPH:
			//MPH -> KPM
			*number = *number * 1.6093;
			break;
			}
		} //end switch


}

void gauge_get_digits(float* number,unsigned char param_id, char* string)
{
	


	unsigned char precision = 0;		//!< The number of digits after the decimal point

	/*! Allow decimal points 
		on the following param_id's
	*/
	if (LCDash_descrProtocol() == PROTOCOL_MEGASQUIRT)
		{

		if ( 	param_id == ms2_AFR1 || param_id == ms2_AFR2 || 
				param_id == afrGauge1 || param_id == vacBooGauge || 
				param_id == pulseWidth1Gauge || param_id == pulseWidth2Gauge )
			{
			precision = 1;
			}
		}
	else
//bat injpw o2sensor o2sensr2 mpg
	if (LCDash_descrProtocol() == PROTOCOL_MUT)
		{
		precision = mut_getPrecision(param_id);
		}


	dtostrf(*number,5,precision,string); 					//!< Convert the value to ascii

}



GAUGE* gauge_getGaugeList()
{

if (LCDash_descrProtocol() == PROTOCOL_MEGASQUIRT)
	return ms_getGaugeList();
else
if (LCDash_descrProtocol() == PROTOCOL_OBD)
	return obd_getGaugeList();
else
if (LCDash_descrProtocol() == PROTOCOL_MUT)
	{
	return mut_getGaugeList();
	}

return 0;
}

unsigned char gauge_getGaugeCount()
{

if (LCDash_descrProtocol() == PROTOCOL_MEGASQUIRT)
	return ms_getGaugeCount();
else
if (LCDash_descrProtocol() == PROTOCOL_OBD)
	return obd_getGaugeCount();
else
if (LCDash_descrProtocol() == PROTOCOL_MUT)
	{
	return mut_getGaugeCount();
	}


return 0;
}


void gauge_getGauge(GAUGE * g,unsigned char param_id)
{

unsigned char x;
GAUGE ram_g;
GAUGE * gaugeList=0;
unsigned char gaugeCount=0;

gaugeList = gauge_getGaugeList();
gaugeCount = gauge_getGaugeCount();	


	for (x=0; x< gaugeCount;x++)
		{
		memcpy_P(&ram_g,&gaugeList[x],sizeof(GAUGE));

		if ( param_id == ram_g.param_id)
			{
			memcpy(g,&ram_g,sizeof(GAUGE));
			}
		}

	
}


void gauge_getDefaultMinMax(unsigned char param_id, float * min, float * max)
{

unsigned char x;
GAUGE * gaugeList=0;
unsigned char gaugeCount=0;

gaugeList = gauge_getGaugeList();
gaugeCount = gauge_getGaugeCount();	

	for (x=0; x< gaugeCount;x++)
		{

		if ( pgm_read_byte(gaugeList[x].param_id) == param_id )
			{
			memcpy_P(min,&gaugeList[x].min,4); 
			memcpy_P(max,&gaugeList[x].max,4);
			}

		}



}





void gauge_getGauge_labels(unsigned char paramIdList[], char * labelList)
{
unsigned char x;
GAUGE g;
GAUGE * gaugeList=0;
unsigned char gaugeCount=0;


gaugeList = gauge_getGaugeList();
gaugeCount = gauge_getGaugeCount();



for (x=0; x< gaugeCount;x++)
	{
	memcpy_P(&g,&gaugeList[x],sizeof(GAUGE));
	paramIdList[x] = g.param_id;
	
	strncpy(labelList+(x*GAUGE_LABEL_LENGTH),g.label,GAUGE_LABEL_LENGTH); //get the label string
	}

	
}	




//////////////////////////////////////
//Begin Gauge Alarm Functions
////////////////////////

ALARMBANK theAlarmBank;

void gauge_checkAlarms()
{

unsigned char x;
GAUGE ** gaugeList;
unsigned char gaugeCount=0;
unsigned char alarmCountPrev = theAlarmBank.alarmCount;

gaugeList = gauge_getGaugePointers(&gaugeCount);

//dev!
gauge_getAlarmsStatus(); //update the alarm status
/*
alarmsStatus[0]=TRUE; //turn the first alarm
alarmsStatus[1]=TRUE; //turn the second alarm
alarmsStatus[2]=FALSE; //turn the first alarm
alarmsStatus[3]=FALSE; //turn the second alarm
alarmsStatus[4]=FALSE; //turn the first alarm
alarmsStatus[5]=FALSE; //turn the second alarm
*/

	for (x=0; x< gaugeCount;x++)
		{
		if (alarmsStatus[x])
			{
			//Gauge alarm is allowed
			if (gaugeList[x]->curr_val >= gaugeList[x]->max || gaugeList[x]->curr_val <= gaugeList[x]->min)
				{
					gauge_addAlarmToBank(gaugeList[x]); //alarming, add it 
				}
			else
				{
				//Allowed to alarm, but not in range
				char index = gauge_findAlarmInBank(gaugeList[x]);
				if (index >= 0)
					gauge_removeAlarmFromBank(index); //remove it
				}
			} //end if alarm status
		else
			{
			//the alarm is turned off, 
			//See if we can pop it out
		//	char index = gauge_findAlarmInBank(gaugeList[x]);
		//	if (index >= 0)
			//	gauge_removeAlarmFromBank(index); //remove it
			}
		
		} //end for each gauge


//GUI update section
if ( (theAlarmBank.alarmCount == 0) && (alarmCountPrev > 0))
	{
	LCD_RECT win;
	win.left = 130; win.right = 237; win.top = 60; win.bottom = 117;	
	lcd_fill_rect(win,0x00); //clear the rectangle
	gauge_drawGaugeLayout();
	}
else
	{
	if ( theAlarmBank.alarmCount > 0 )
		gauge_refreshAlarmWindow(); //update gui
	}
} //end function



void gauge_removeAlarmFromBank(char index)
{

unsigned char i;

if (index < 0)
	return; //just in case

theAlarmBank.alarmCount--; //less one alarm


for (i=(unsigned char)index; i< theAlarmBank.alarmCount; i++)
	{
	theAlarmBank.items[i] = theAlarmBank.items[i+1]; //shift each item up one
	}

theAlarmBank.items[GAUGE_MAXALARMCOUNT] = 0;
} //end function removeAlarm





unsigned char gauge_addAlarmToBank(GAUGE * g)
{

if (theAlarmBank.alarmCount < GAUGE_MAXALARMCOUNT)
	{

	if (gauge_findAlarmInBank(g) >= 0)
		{
		return FALSE; //param_id already present, don't add it
		}
	else
		{
		theAlarmBank.items[theAlarmBank.alarmCount] = g; //save the gauge address
		theAlarmBank.alarmCount++; //increment the alarm count
		return TRUE; //we added an alarm
		}
	}

return FALSE;
}



char gauge_findAlarmInBank(GAUGE * g)
{
unsigned char x;

for (x=0; x<theAlarmBank.alarmCount; x++)
	{
	if (theAlarmBank.items[x] == g)
		return x; //return the index of the found alarm
	}

return -1; //alarm not found
}



void gauge_refreshAlarmWindow()
{

LCD_RECT win;
GAUGE ** gauge_list;
unsigned char x,gauge_count,y;
char out[6];

gauge_list = gauge_getGaugePointers(&gauge_count);

win.left = 130;
win.right = 237;
win.top = 60;
win.bottom = 117;

unsigned char y_loc=win.top;

lcd_rect_diffSize(win,+1,0); //clear window
lcd_fill_rect(win,0xFF); //draw outline
gui_puts("Warning!",win.left+32,win.top+1,1);
lcd_line(win.left,win.top+9,win.right,win.top+9,0);

itoa(theAlarmBank.alarmCount,out,10);
gui_puts(out,win.left,win.top,1);

for (x=0;x<theAlarmBank.alarmCount;x++)
	{
	for (y=0;y<gauge_count;y++)
		{
		if (theAlarmBank.items[x] == gauge_list[y])
			{
			char out[6];
			y_loc+=11;
			dtostrf(gauge_list[y]->curr_val,5,1,out); 

			gui_puts(gauge_list[y]->label,win.left+2,y_loc,1);		
			gui_puts(out,win.left+60,y_loc,1);
			//lcd_line(win.left,win.top+30,win.right,y_loc+12,1);
		
			}
		}
	}
}




void gauge_getAlarmsStatus()
{

if (currentLayout == LAYOUT_STANDARD)
	{
		alarmsStatus[0] = analog_g1.alarm.status;
		alarmsStatus[1] = bar_g1.alarm.status;
		alarmsStatus[2] = bar_g2.alarm.status;
		alarmsStatus[3] = digi_gauge1.alarm.status;
		alarmsStatus[4] = digi_gauge2.alarm.status;
	}
else
if (currentLayout == LAYOUT_ANALOG)
	{
	;
	}
else
if (currentLayout == LAYOUT_DIGITAL)
	{
		alarmsStatus[0] = digi_gauge1.alarm.status;
		alarmsStatus[1] = digi_gauge2.alarm.status;
		alarmsStatus[2] = digi_gauge3.alarm.status;
		alarmsStatus[3] = digi_gauge4.alarm.status;
	}
else
if (currentLayout == LAYOUT_SCOPE)
	{
		alarmsStatus[0] = scope_g1.alarm.status;
		alarmsStatus[1] = scope_g2.alarm.status;
	}
else
if (currentLayout == LAYOUT_BAR)
	{
		alarmsStatus[0] = bar_g1.alarm.status;
		alarmsStatus[1] = bar_g2.alarm.status;
	}
else
if (currentLayout == LAYOUT_3BAR)
	{
		alarmsStatus[0] = bar_g1.alarm.status;
		alarmsStatus[1] = bar_g2.alarm.status;
		alarmsStatus[2] = bar_g3.alarm.status;
	}
else
if (currentLayout == LAYOUT_SCOPE_MIX)
	{
		alarmsStatus[0] = scope_g1.alarm.status; 
		alarmsStatus[1] = digi_gauge1.alarm.status;
		alarmsStatus[2] = digi_gauge2.alarm.status;
	}
else
if (currentLayout == LAYOUT_SCOPE_BAR_ANALOG)
	{
		alarmsStatus[0] = analog_g1.alarm.status;
		alarmsStatus[1] = bar_g1.alarm.status;
		alarmsStatus[2] = bar_g2.alarm.status;
		alarmsStatus[3] = scope_g1.alarm.status; 
	}
else
if (currentLayout == LAYOUT_XLDIGITAL)
	{
		alarmsStatus[0] = XLdigi_gauge1.alarm.status;
	}
else
if (currentLayout == LAYOUT_TUNING)
	{
	;
	}
}
