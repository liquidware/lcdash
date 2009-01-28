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

#include "barGauge.h"

void gauge_barGauge_paintBar(unsigned char gui_curr,unsigned char gui_prev, LCD_RECT bounds)
{
int fill_diff;
unsigned char fill_pattern;

	fill_diff = gui_curr - gui_prev; //the difference that we need to paint

	bounds.top++;		//make the rect smaller than the outline
	bounds.bottom--;	//same thing here
	
	if (fill_diff < 0)
		{
		//value is decreasing, erase	
		bounds.left 		= gui_curr;
		bounds.right 		= gui_prev;
		fill_pattern = 0x00;
		}
	else
		{
		//value is increasing, paint
		bounds.left 		= gui_prev;
		bounds.right 		= gui_curr;
		fill_pattern = 0xff;
		}

	lcd_fill_rect(bounds,fill_pattern); //paint the difference

}

void gauge_barGauge_refresh(BAR_GAUGE * g)
{

if (g->value.curr_val != g->prev_val)
	{

	char out[6];
	int gui_curr;
	
	gui_curr = ((g->value.curr_val-g->value.min) * g->scale_factor) + g->bounds.left; //calculate the new x coordinate
	
	if (gui_curr < g->bounds.left+1)
		gui_curr = g->bounds.left+1;
	else
	if (gui_curr > g->bounds.right-3)
		gui_curr = g->bounds.right-3;
			
	gauge_barGauge_paintBar(gui_curr,g->gui_prev,g->bounds);


	g->gui_prev = gui_curr; //Current x drawing point is now our previous point

	gauge_convert_units(&g->value.curr_val,g->value.units,g->value.user_units); //change units
	gauge_get_digits(&g->value.curr_val,g->value.param_id, out); //get the precision
	
	gui_puts(out,g->bounds.right-30,g->bounds.top-8,0); //print the right justified number on the top of the bar
	
	} //end if
 
} //end refresh



void gauge_barGauge_update(BAR_GAUGE * g)
{
	//number stuff
	int div;
	int num;
	char out[6];

	num = g->bounds.right - g->bounds.left; 
	div = g->value.max - g->value.min;
	
	if (div != 0)  //catch a divide by zero
		g->scale_factor = (float)num/(float)div;
	

	//gui stuff
	lcd_fill_rect(g->bounds,0x00); //erase the area

	lcd_line(g->bounds.left+2, g->bounds.top, g->bounds.right-2, g->bounds.top,1); //top line
	lcd_line(g->bounds.left+2, g->bounds.bottom, g->bounds.right-2, g->bounds.bottom,1); //bottom

	lcd_line(g->bounds.left, g->bounds.top+2, g->bounds.left, g->bounds.bottom-2,1); //left line
	lcd_line(g->bounds.right, g->bounds.top+2, g->bounds.right, g->bounds.bottom-2,1); //right

	//Making rounded corners
	lcd_pixel(g->bounds.left+1,g->bounds.top+1,1); //top left
	lcd_pixel(g->bounds.right-1,g->bounds.top+1,1); //top right
	lcd_pixel(g->bounds.left+1,g->bounds.bottom-1,1); //bottom left
	lcd_pixel(g->bounds.right-1,g->bounds.bottom-1,1); //bottom right


	//Make the tick marks
	unsigned char quarter,x_loc;

	quarter = (g->bounds.right - g->bounds.left)/4; // 25%

	x_loc = g->bounds.left + quarter; 
	lcd_line(x_loc, g->bounds.bottom-2, x_loc, g->bounds.bottom+2, 1); // %25 tick mark

	x_loc += quarter;
	lcd_line(x_loc, g->bounds.bottom-2, x_loc, g->bounds.bottom+2, 1); // %50 tick mark

	x_loc += quarter;
	lcd_line(x_loc, g->bounds.bottom-2, x_loc, g->bounds.bottom+2, 1); // %75 tick mark

	//label stuff
	gui_puts(g->value.label,g->bounds.left, g->bounds.top-8, 0); //draw the label
	
	//draw the user's minimum
	gauge_get_digits(&g->value.min,g->value.param_id, out); //get the precision
	str_left_justify(out);
	gui_puts(out,g->bounds.left,g->bounds.bottom+2,0);	

	//draw the user's max
	gauge_get_digits(&g->value.max,g->value.param_id, out); //get the precision
	gui_puts(out,g->bounds.right-30,g->bounds.bottom+2,0);
	

	//resetting stuff
	g->prev_val = 0;
//	g->value.curr_val = 0;
	g->gui_prev = g->bounds.left;	

	gauge_barGauge_refresh(g);
}
