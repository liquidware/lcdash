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

#include "scopeGauge.h"



void gauge_scopeGauge_update(SCOPE_GAUGE * scope)
{
char out[7];
unsigned char y, quarter;

//init the data points
//for (y=0;y<SCOPE_MAX_POINTS;y++)
//	scope->points[y]=0x00;

//calculate the scale factor
scope->scale_factor = (scope->bounds.bottom-scope->bounds.top) / (scope->value.max - scope->value.min);

//print the label name
char label[10];
strncpy(label,scope->value.label,9);
label[9]=0x00;
gui_puts(label,scope->bounds.right+2,scope->bounds.top+1,0);

//print the max
gauge_get_digits(&scope->value.max,scope->value.param_id, out); //get the precision
gui_puts(out,scope->bounds.left-33,scope->bounds.top,0);

//print the min
gauge_get_digits(&scope->value.min,scope->value.param_id, out); //get the precision
gui_puts(out,scope->bounds.left-33,scope->bounds.bottom-8,0);

//scope->plot_points = 0; //no points in the gauge

//draw the outline
lcd_rect_diffSize(scope->bounds,1,1);

//draw tick marks



quarter = (scope->bounds.bottom-scope->bounds.top)*0.25;

//25%
y = quarter + scope->bounds.top;
lcd_line(scope->bounds.left-2,y,scope->bounds.left,y,1);

//50%
y +=quarter;
lcd_line(scope->bounds.left-3,y,scope->bounds.left+1,y,1);

//75%
y +=quarter;
lcd_line(scope->bounds.left-2,y,scope->bounds.left,y,1);



}







void gauge_scopeGauge_refresh(SCOPE_GAUGE * scope)
{
unsigned char y_point;
unsigned char i;
char out[7];
unsigned char min = (scope->bounds.left+2);

//calculate the y-point
y_point = (unsigned char)( (scope->value.curr_val-scope->value.min) * scope->scale_factor );
y_point = scope->bounds.bottom - y_point;


//bounds checking
if (y_point < scope->bounds.top)
	y_point = scope->bounds.top;

if (y_point > scope->bounds.bottom)
	y_point = scope->bounds.bottom;


scope->points[SCOPE_MAX_POINTS-1] = y_point; //set the most recent point


if (scope->plot_points < SCOPE_MAX_POINTS-scope->bounds.left-2)
	{
	scope->plot_points++;
	min = SCOPE_MAX_POINTS - scope->plot_points;
	}

//now shift the points
for (i=min;i<(SCOPE_MAX_POINTS-1);i++)
	{
	lcd_pixel(i,scope->points[i],0); //erase the old 
	scope->points[i] = scope->points[i+1]; //shift left one position
	lcd_pixel(i,scope->points[i],1); //draw the new
	}

//print the value
gauge_convert_units(&scope->value.curr_val,scope->value.units,scope->value.user_units); //change units
gauge_get_digits(&scope->value.curr_val,scope->value.param_id, out); //get the precision
gui_puts(out,scope->bounds.right+2,scope->bounds.top+10,0);

}
