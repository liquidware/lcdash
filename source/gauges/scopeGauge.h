/*! \file scopeGauge.h
	This is the header for the scopeGauge
*/

#define SCOPE_MAX_POINTS	180
#define SCOPE_HEIGHT		119

typedef struct ScopeGauge{
	LCD_RECT	bounds;		//!< The boundary of the scope gauge
	GAUGE		value;		//!< Gauge structure for parameter 1
	unsigned char points[SCOPE_MAX_POINTS]; //point data for gauge 1
	float		scale_factor;
	unsigned char plot_points;
	GAUGE_ALARM	alarm;			//!< The Alarm
} SCOPE_GAUGE;


void gauge_scopeGauge_changeDispCnt(SCOPE_GAUGE * scope,unsigned char count);
void gauge_scopeGauge_update();
void gauge_scopeGauge_refresh();
