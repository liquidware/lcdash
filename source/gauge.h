/*! \file gauge.h
	\brief	Functions relating to GUI gauges
*/
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "touchscreen.h"

#define GAUGE_LABEL_LENGTH	14 /*! This is the length of the label for the gauges */
#define DESCRIPTION_MAX_LENGTH	30
#define GAUGE_UNITS_DESCRIPTION_LENGTH	10
#define GAUGE_MAXALARMCOUNT	5

#define LAYOUT_STANDARD 		0
#define LAYOUT_ANALOG			1
#define LAYOUT_DIGITAL			2
#define LAYOUT_SCOPE			3
#define LAYOUT_BAR				4
#define LAYOUT_SCOPE_MIX		5
#define LAYOUT_3BAR				6
#define LAYOUT_SCOPE_BAR_ANALOG	7
#define LAYOUT_XLDIGITAL		8
#define LAYOUT_TUNING			9

//the Gauge ID's to tell what type of gauge it is
#define DIGI_GAUGE1_ID		1
#define DIGI_GAUGE2_ID		2
#define DIGI_GAUGE3_ID		3
#define DIGI_GAUGE4_ID		4
#define BAR_G1_ID			5
#define BAR_G2_ID			6
#define BAR_G3_ID			7
#define ANALOG_G1_ID		8
#define SCOPE_G1_ID 		9
#define SCOPE_G2_ID 		10
#define XLDIGI_GAUGE1_ID	11
#define TUNING_DIGI_GAUGE1_ID		12
#define TUNING_DIGI_GAUGE2_ID		13
#define TUNING_DIGI_GAUGE3_ID		14


#define EE_GAUGE_SAVED 16


////////////////////////////////////////////////////////////////////////////
/* The basic gauge structure to which all GUI gauges are built off of */
////////////////////////////////////////////////////////////////////////////
typedef struct Gauge{
	float			min;		//!< The minimum gauge value
	float			max;		//!< The maximum gauge value
	float		  	curr_val; 	//!< Its present value
	unsigned char	param_id;  	//!< The parameter id this gauge is based on
	char		  	label[GAUGE_LABEL_LENGTH];	//!< Its displayed ascii label
	unsigned char 	units;		//!< The units of the gauge
	unsigned char 	user_units;		//!< The units of the gauge
	}GAUGE;



typedef struct AlarmBank{
	GAUGE	* items[GAUGE_MAXALARMCOUNT+1];
	unsigned char alarmCount;	//current count of alarms
	unsigned int displayTime;	//alarm window display time
	} ALARMBANK;



///////////////////////////////////////////////////////////////
/*! A gauge alarm is to allow 
	the triggering of a defined alarming point.
*/
//////////////////////////////////////////////////////////////////

typedef struct GaugeAlarm{
	unsigned char			status;	//!< On, off, frozen
	} GAUGE_ALARM;








GAUGE** 	gauge_getGaugePointers(unsigned char * count);

void 			gauge_changeGaugeLayout(unsigned char the_layout);
unsigned char 	gauge_getGaugeLayout();
void 			gauge_drawGaugeLayout();
void 			gauge_refreshGaugeLayout();
void 			gauge_restoreLayout();

GAUGE * 	gauge_checkPointInGauge(POINT p, unsigned char * gauge_id,unsigned char ** alarm);
void 	gauge_changeParameter(GAUGE * g);
unsigned char gauge_selectGaugeToChange();
unsigned char gauge_selectLayout();
void gauge_getGaugeDescription(char * dest_str,unsigned char gauge_id);
void gauge_saveGauge(GAUGE  * g, unsigned char gauge_id);


/*! Gets the correct number of precision 
	depending on the param_id. Precision is 1 digit for things like AFR
*/
void 	gauge_get_digits(float* number,unsigned char param_id, char* string);
void 	gauge_convert_units(float* number,unsigned char units, unsigned char user_units);


void gauge_getUnitsDescription(char * dest, unsigned char units_id);

unsigned char gauge_getOtherUnits(unsigned char units_id);

unsigned char gauge_getGaugeCount();
void gauge_getGauge_labels(unsigned char paramIdList[], char * labelList);
GAUGE* gauge_getGaugeList();
void gauge_getDefaultMinMax(unsigned char param_id, float * min, float * max);
void gauge_getGauge(GAUGE * g,unsigned char param_id);


//alarm functions
void gauge_checkAlarms();
void gauge_removeAlarmFromBank(char index);
unsigned char gauge_addAlarmToBank(GAUGE * g);
char gauge_findAlarmInBank(GAUGE * g);
void gauge_refreshAlarmWindow();
void gauge_getAlarmsStatus();

#endif
