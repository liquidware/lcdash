#ifndef _MUT_GAUGES_H_
#define _MUT_GAUGES_H_

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

//bat injpw o2sensor o2sensr2 mpg
#define MUT_TPS				0x17
#define MUT_RPM				0x21
#define MUT_MAP				0x38
#define MUT_ACCELENRICH		0x1D
#define MUT_TIMINGADV		0x06
#define MUT_FUELTRIM_LOW	0x0E
#define MUT_FUELTRIM_MED	0x0D
#define MUT_FUELTRIM_HIGH	0x0C
#define MUT_O2FEEDBACKTRIM	0x0F
#define MUT_O2SENSOR		0x13
#define MUT_O2SENSOR2		0x1F
#define MUT_VSS				0x2F
#define MUT_CLT				0x07
#define MUT_AIRFLOW			0x1A
#define MUT_MAT				0x3A
#define MUT_INJPW			0x29
#define MUT_BAT				0x14
#define MUT_AFRMAP			0x32
#define MUT_ECULOAD			0x00
#define MUT_AIRVOL			0x2C
#define MUT_BARO			0x15
#define MUT_KNOCKSUM		0x26
#define MUT_ISCSTEPS		0x16
#define MUT_OCTANELEVEL		0x27
#define MUT_TARGETIDLERPM	0x24
#define MUT_KNOCKVOLTAGE	0x30
#define MUT_EGRTEMP			0x12


#define MUT_ESTHP			0xFA
#define MUT_INJDUTYCYCLE	0xFB
#define MUT_MPG				0xFC
#define MUT_RPM2B			0xFD //special
#define MUT_ECULOAD2B		0x01 //special

#define MUT_NONE			0xFF //!A blank gauge

///////Begin Gauge Storage///////////////////////////////////////////////
#include "units.h"
#include "gauge.h"

#define MUT_GAUGE_CNT	32 	//!The total number of gauges

GAUGE mut_GaugeList[]  PROGMEM = {

///////////////////////////////////////////////////////
//1.
	{	
		-1, 			 	//min
		1,		 	//max 
		0,			 	//curr_val
		MUT_NONE,	 	//param_id
		"No Gauge",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
///////////////////////////////////////////////////////
//2.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_TPS,	 	//param_id
		"TPS",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//3.
	{	
		0, 			 	//min
		7500,		 	//max 
		0,			 	//curr_val
		MUT_RPM,	 	//param_id
		"RPM",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//4.
	{	
		0, 			 	//min
		300,		 	//max 
		0,			 	//curr_val
		MUT_MAP,	 	//param_id
		"Boost(MDP)",		//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//5.
	{	
		0, 			 	//min
		150,		 	//max 
		0,			 	//curr_val
		MUT_ACCELENRICH,	 	//param_id
		"AccelEnrich",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//6.
	{	
		0, 			 	//min
		50,		 		//max 
		0,			 	//curr_val
		MUT_TIMINGADV,	 	//param_id
		"Advance",		//label
		UNITS_DEG,		//units
		UNITS_DEG		//user units
	},
/////////////////////////////////////////////////////
//7.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_FUELTRIM_LOW,	 	//param_id
		"FuelTrim_Low",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//8.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_FUELTRIM_MED,	 	//param_id
		"FuelTrim_Med",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//9.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_FUELTRIM_HIGH,	 	//param_id
		"FuelTrim_Hi",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//10.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_O2FEEDBACKTRIM,	 	//param_id
		"02 FB Trim",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//11.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_O2SENSOR,	 	//param_id
		"02 Sensor",		//label
		UNITS_VOLTS,		//units
		UNITS_VOLTS		//user units
	},
/////////////////////////////////////////////////////
//12.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_O2SENSOR2,	 	//param_id
		"02 Sensor2",		//label
		UNITS_VOLTS,		//units
		UNITS_VOLTS		//user units
	},
/////////////////////////////////////////////////////
//13.
	{	
		0, 			 	//min
		180,		 	//max 
		0,			 	//curr_val
		MUT_VSS,	 	//param_id
		"VSS",		//label
		UNITS_MPH,		//units
		UNITS_MPH		//user units
	},
/////////////////////////////////////////////////////
//14.
	{	
		0, 			 	//min
		280,		 	//max 
		0,			 	//curr_val
		MUT_CLT,	 	//param_id
		"CoolantTemp",		//label
		UNITS_FAREN,		//units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//15.
	{	
		0, 			 	//min
		1650,		 	//max 
		0,			 	//curr_val
		MUT_AIRFLOW,	 	//param_id
		"AirFlow Hz",		//label
		UNITS_HZ,		//units
		UNITS_HZ		//user units
	},
/////////////////////////////////////////////////////
//16.
	{	
		0, 			 	//min
		200,		 	//max 
		0,			 	//curr_val
		MUT_MAT,	 	//param_id
		"Air Temp",		//label
		UNITS_FAREN,		//units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//17.
	{	
		0, 			 	//min
		66,		 	//max 
		0,			 	//curr_val
		MUT_INJPW,	 	//param_id
		"Inj PW",		//label
		UNITS_mSEC,		//units
		UNITS_mSEC		//user units
	},
/////////////////////////////////////////////////////
//18.
	{	
		0, 			 	//min
		15,		 	//max 
		0,			 	//curr_val
		MUT_BAT,	 	//param_id
		"BattVolts",		//label
		UNITS_VOLTS,		//units
		UNITS_VOLTS		//user units
	},
/////////////////////////////////////////////////////
//19.
	{	
		0, 			 	//min
		255,		 	//max 
		0,			 	//curr_val
		MUT_AFRMAP,	 	//param_id
		"AFR MAP",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//20.
	{	
		0, 			 	//min
		160,		 	//max 
		0,			 	//curr_val
		MUT_ECULOAD,	 	//param_id
		"ECULoad",		//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//21.
	{	
		0, 			 	//min
		400,		 	//max 
		0,			 	//curr_val
		MUT_ECULOAD2B,	 	//param_id
		"ECULoad2B",		//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//22.
	{	
		0, 			 	//min
		255,		 	//max 
		0,			 	//curr_val
		MUT_AIRVOL,	 	//param_id
		"AirVolume",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//23.
	{	
		0, 			 	//min
		160,		 	//max 
		0,			 	//curr_val
		MUT_BARO,	 	//param_id
		"Barometer",		//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//24.
	{	
		0, 			 	//min
		50,		 	//max 
		0,			 	//curr_val
		MUT_KNOCKSUM,	 	//param_id
		"KnockSum",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//25.
	{	
		0, 			 	//min
		255,		 	//max 
		0,			 	//curr_val
		MUT_ISCSTEPS,	 	//param_id
		"ISC STEPS",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//26.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_OCTANELEVEL,	 	//param_id
		"OctaneLevel",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//27.
	{	
		0, 			 	//min
		3000,		 	//max 
		0,			 	//curr_val
		MUT_TARGETIDLERPM,	 	//param_id
		"TargetIdleRPM",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//28.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_KNOCKVOLTAGE,	 	//param_id
		"KnockVolts",		//label
		UNITS_VOLTS,		//units
		UNITS_VOLTS		//user units
	},
/////////////////////////////////////////////////////
//29.
	{	
		0, 			 	//min
		300,		 	//max 
		0,			 	//curr_val
		MUT_EGRTEMP,	 	//param_id
		"EGR Temp",		//label
		UNITS_FAREN,		//units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//30.
	{	
		0, 			 	//min
		50,		 	//max 
		0,			 	//curr_val
		MUT_MPG,	 	//param_id
		"MPG (US)",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//31.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		MUT_INJDUTYCYCLE,	 	//param_id
		"InjDutyCycle",		//label
		UNITS_PERCENT,		//units
		UNITS_PERCENT		//user units
	},
/////////////////////////////////////////////////////
//32.
	{	
		150, 			 	//min
		600,		 	//max 
		0,			 	//curr_val
		MUT_ESTHP,	 	//param_id
		"Est HP",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
};



#endif
