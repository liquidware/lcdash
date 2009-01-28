#ifndef _ODB_GAUGES_H_
#define _ODB_GAUGES_H_

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define PID_SHOWPIDS		0x00
#define PID_ENGINELOAD		0x04
#define PID_ENGINECLT		0x05
#define PID_STFUELTRIMB1	0x06
#define PID_LTFUELTRIMB1	0x07
#define PID_STFUELTRIMB2	0x08
#define PID_LTFUELTRIMB2	0x09
#define PID_FUELPRESS		0x0A
#define PID_MAP				0x0B
#define PID_RPM				0x0C
#define PID_VSS				0x0D
#define PID_ADVANCE			0x0E
#define PID_MAT				0x0F
#define PID_MAF				0x10
#define PID_TPS				0x11
#define PID_NONE			0xFF //!A blank gauge

///////Begin Gauge Storage///////////////////////////////////////////////
#include "units.h"
#include "gauge.h"

#define OBD_GAUGE_CNT	15 	//!The total number of gauges

GAUGE obd_GaugeList[]  PROGMEM = {

///////////////////////////////////////////////////////
//1.
	{	
		-1, 			 	//min
		1,		 	//max 
		0,			 	//curr_val
		PID_NONE,	 	//param_id
		"No Gauge",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//2.
	{	
		0, 			 	//min
		7000,		 	//max 
		0,			 	//curr_val
		PID_RPM,	 	//param_id
		"RPM",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//3.
	{	
		0, 			 	//min
		150,		 	//max 
		0,			 	//curr_val
		PID_VSS,	 	//param_id
		"VSS",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//4.
	{	
		-10, 			 	//min
		50,		 	//max 
		0,			 	//curr_val
		PID_ADVANCE,	 	//param_id
		"Advance",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//5.
	{	
		-40, 			 	//min
		215,		 	//max 
		0,			 	//curr_val
		PID_MAT,	 	//param_id
		"MAT",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//6.
	{	
		0, 			 	//min
		655,		 	//max 
		0,			 	//curr_val
		PID_MAF,	 	//param_id
		"MAF",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//7.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_TPS,	 	//param_id
		"TPS",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//8.
	{	
		0, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_ENGINELOAD,	 	//param_id
		"EngLoad",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//9.
	{	
		-40, 			 	//min
		215,		 	//max 
		0,			 	//curr_val
		PID_ENGINECLT,	 	//param_id
		"CLT",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//10.
	{	
		0, 			 	//min
		765,		 	//max 
		0,			 	//curr_val
		PID_FUELPRESS,	 	//param_id
		"Fuel Pressure",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//11.
	{	
		0, 			 	//min
		255,		 	//max 
		0,			 	//curr_val
		PID_MAP,	 	//param_id
		"MAP",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//12.
	{	
		-100, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_STFUELTRIMB1,	 	//param_id
		"ST FuelTrimB1",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//13.
	{	
		-100, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_LTFUELTRIMB1,	 	//param_id
		"LT FuelTrimB1",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//14.
	{	
		-100, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_STFUELTRIMB2,	 	//param_id
		"ST FuelTrimB2",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//15.
	{	
		-100, 			 	//min
		100,		 	//max 
		0,			 	//curr_val
		PID_LTFUELTRIMB2,	 	//param_id
		"LT FuelTrimB2",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////

};



#endif
