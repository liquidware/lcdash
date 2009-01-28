/*! \file barGauge.h
	\brief Functions relating to bar gauge drawing, updating, refreshing
*/

#ifndef _BARGAUGE_H_
#define _BARGAUGE_H_

#include "../gauge.h"

////////////////////////////////////////
/*! A Bar graph gauge*/
////////////////////////////////////////
typedef struct BarGauge{
	LCD_RECT 	bounds;			//!< The Boundary
	GAUGE	 	value;			//!< The present value and it's associated properties
	GAUGE_ALARM	alarm;			//!< The Alarm
	int gui_prev;		//!< storage for the previous x location
	unsigned char fill_pattern; //!< The bar fill pattern

	float		prev_val;		//!< Its previous value (to see if it changed)
	float		scale_factor;	//!< The scale factor based on the max,min, and size of the bar graph
}BAR_GAUGE;




/*! Repaints the gauge on the screen with it's new current value */
void gauge_barGauge_refresh(BAR_GAUGE * g);

/*! Updates the label on screen and refreshes the gauge background*/
void gauge_barGauge_update(BAR_GAUGE * g);

/*! Does the actual painting of the bar */
void gauge_barGauge_paintBar(unsigned char gui_curr,unsigned char gui_prev, LCD_RECT bounds);


#endif
