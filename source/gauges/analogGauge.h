/*! \file analogGauge.h
	\brief Analog Gauge building, drawing, updating functions
*/
#ifndef _ANALOGGAUGE_H_
#define _ANALOGGAUGE_H_


#define ANALOG_GENERATED 20 //!< An eeprom flag

#include "../gauge.h"

////////////////////////////////////////
/*! An analog needle type Gauge */
////////////////////////////////////////
typedef struct AnalogGauge{
	LCD_RECT	bounds;
	POINT		needle_center;	//!< The center of the neddle, this is our drawing center
	POINT		gui_prev;		//!< the previous plotted point, used to track changes
	GAUGE		value;			//!< The present value and it's associated properties
	unsigned char radius;		//!< The radius -not used yet, it is fixed
	float		prev_val;		//!< The previous value
	int		 	prev_index;		//!< The previous index in the lookup table
	float		scale_factor;	//!< The scale factor based on the max, min, radius

//	unsigned char lookup_tbl[]; //!< It's own lookup table -not used yet
	GAUGE_ALARM	alarm;			//!< The Alarm
}ANALOG_GAUGE;




/*! Builds the x,y lookup table for an analog gauge*/
void gauge_analogGauge_buildLookup(unsigned char radius);

/*! Repaints the Analog Gauge with any changes to label, min, max */
void gauge_analogGauge_update(ANALOG_GAUGE * g);

/*! Refreshes the gauge with it's new value */
void gauge_analogGauge_refresh(ANALOG_GAUGE * g);

#endif
