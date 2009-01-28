/*! \file digiGauge.h
	\brief Functions relating to digi gauge drawing, updating, refreshing
*/

#include "../gauge.h"

////////////////////////////////////////
/*! A Digital Gauge */
////////////////////////////////////////
typedef struct XLDigiGauge{
	LCD_RECT 	bounds;			//!< The Boundary
	GAUGE	 	value;			//!< The present value and it's associated properties
	GAUGE_ALARM	alarm;			//!< The Alarm
	float		prev_val;		//!< The previous value 
}XLDIGI_GAUGE;




/*! Repaints the gauge on the screen with it's new current value*/
void gauge_XLdigiGauge_refresh(XLDIGI_GAUGE * g);

/*! Updates the label on screen and refreshes the gauge */
void gauge_XLdigiGauge_update(XLDIGI_GAUGE * g);
