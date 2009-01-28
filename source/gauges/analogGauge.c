#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <math.h>

#include "../t6963.h"
#include "../gui.h"
#include "../touchscreen.h"
#include "../gauge.h"
#include "../string_manip.h"
#include "../LCDash.h"
#include "../bitops.h"
#include "../menu.h"
#include "analogGauge.h"


EEMEM POINT ee_analog_lookup[180];
EEMEM POINT ee_analogOutline_lookup[180];
EEMEM unsigned char ee_isAnalogGenerated;

void gauge_analogGauge_update(ANALOG_GAUGE * g)
{
unsigned char x;
POINT draw_loc;
char out[6];

g->scale_factor = 180/(g->value.max-g->value.min);
g->prev_index = -1; //force a paint

	for (x=0;x<180;x++)
	 {
		eeprom_read_block(&draw_loc,&ee_analogOutline_lookup[x],sizeof(POINT));
		lcd_pixel(g->needle_center.x+draw_loc.x, g->needle_center.y+draw_loc.y, 1);
	 }

	gui_puts(g->value.label,g->bounds.left,g->bounds.top,0);
	

	//draw the user's minimum value
	eeprom_read_block(&draw_loc,&ee_analogOutline_lookup[0],sizeof(POINT)); //read the highest point
	gauge_get_digits(&g->value.min,g->value.param_id,out);
	str_left_justify(out);
	gui_puts(out,g->needle_center.x-48,  g->needle_center.y+44,0);


	//draw the user's maximum value
	eeprom_read_block(&draw_loc,&ee_analogOutline_lookup[179],sizeof(POINT)); //read the highest point
	gauge_get_digits(&g->value.max,g->value.param_id,out);
	str_left_justify(out);
	gui_puts(out,g->needle_center.x+18,  g->needle_center.y-55,0);

//	g->prev_val = 0;
//	g->value.curr_val = 0;
	
	gauge_analogGauge_refresh(g);
}



void gauge_analogGauge_refresh(ANALOG_GAUGE * g)
{

float curr_val = g->value.curr_val;


if (curr_val != g->prev_val || (curr_val == 0))
	{
	int index;
	POINT draw_loc;	
	char	out[6];

	index = (int)( (curr_val-g->value.min)*g->scale_factor);

	gauge_convert_units(&g->value.curr_val,g->value.units,g->value.user_units); //change units
	gauge_get_digits(&g->value.curr_val,g->value.param_id, out); //get the precision
	
		


	gui_puts(out,g->needle_center.x,g->needle_center.y+7,0); //write the value

	if (index>179)
		{
		index = 179;
		}
	else if (index < 0)
		{
		index = 0;
		}
	
	if (index != g->prev_index) //trying to be efficient
		{
		eeprom_read_block(&draw_loc,&ee_analog_lookup[index],sizeof(POINT));
	
			
		//erase the lines
		lcd_line(g->needle_center.x, g->needle_center.y-2, g->needle_center.x+g->gui_prev.x, g->needle_center.y+g->gui_prev.y, 0); 
		lcd_line(g->needle_center.x, g->needle_center.y+2, g->needle_center.x+g->gui_prev.x, g->needle_center.y+g->gui_prev.y, 0); 


		//draw the lines
		lcd_line(g->needle_center.x, g->needle_center.y-2, g->needle_center.x+draw_loc.x,g->needle_center.y+draw_loc.y,1);
		lcd_line(g->needle_center.x, g->needle_center.y+2, g->needle_center.x+draw_loc.x,g->needle_center.y+draw_loc.y,1); 




		g->gui_prev = draw_loc; //current plotting is point now saved.
		g->prev_index = index; //save our last index 

		lcd_circle(g->needle_center.x,g->needle_center.y,2,1);
		lcd_circle(g->needle_center.x,g->needle_center.y,1,0);
		}
		
	}


}




void gauge_analogGauge_buildLookup(unsigned char radius)
{

if (eeprom_read_byte(&ee_isAnalogGenerated) != ANALOG_GENERATED)
	{

	POINT p;

	unsigned int deg;

	for (deg=0;deg<180;deg++)
		{
	
		p.y = (unsigned char)radius*sin((deg+120) * 0.0174);
		p.x = (unsigned char)radius*cos((deg+120) * 0.0174);
	
		eeprom_write_block(&p,&ee_analog_lookup[deg],sizeof(POINT));

		p.y = (unsigned char)(radius+1)*sin((deg+120) * 0.0174);
		p.x = (unsigned char)(radius+1)*cos((deg+120) * 0.0174);
		eeprom_write_block(&p,&ee_analogOutline_lookup[deg],sizeof(POINT));
	
		}
	
	eeprom_write_byte(&ee_isAnalogGenerated,ANALOG_GENERATED); //we've been stored to eeprom
	}

}





