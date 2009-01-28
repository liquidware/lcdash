/*! \file megasquirt_gauges.h
	\brief Gauges for Megasquirt system are initialized here
	When adding new guages, put their min,max,label,units, etc. here
*/

#ifndef _MS1_GAUGES_H_
#define _MS1_GAUGES_H_

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>



///////Begin Gauge Storage///////////////////////////////////////////////
#include "ms1_defs.h" 
#include "speedo.h"
#include "sensors_oilpressure.h"

#define MS1_GAUGE_CNT	16 	//!The total number of gauges

GAUGE ms1_GaugeList[]  PROGMEM = {

///////////////////////////////////////////////////////
//1.
	{	
		0, 			 	//min
		7000,		 	//max 
		0,			 	//curr_val
		rpmHiResGauge,	 	//param_id
		"RPM",	//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//2.
	{	
		100, 			 //min
		240,		 	//max 
		0,			 	//curr_val
		cltGauge,	//param_id
		"Coolant",		//label
		UNITS_FAREN,		//user units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//3.
	{	
		10.0, 			 //min
		19.0,		 	//max 
		0,			 	//curr_val
		afrGauge1,		//param_id
		"AFR1",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//4.
	{	
		10.0, 			 //min
		19.0,		 	//max 
		0,			 	//curr_val
		afrGauge2,		//param_id
		"AFR2",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//5.
	{	
		0, 			 	//min
		250,		 	//max 
		0,			 	//curr_val
		mapGauge,		//param_id
		"MAP",			//label
		UNITS_KPA,		//user units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//6.
	{	
		0, 			 		//min
		30,		 			//max 
		0,			 		//curr_val
		advanceGauge,		//param_id
		"Advance",		//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//7.
	{	
		0, 				//min
		150,		 	//max 
		0,			 	//curr_val
		matGauge,		//param_id
		"Air Temp",	//label
		UNITS_FAREN,		//user units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//8.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		egoCorrGauge1,		//param_id
		"EGO correct 1",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//9.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		egoCorrGauge2,		//param_id
		"EGO correct 2",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//10.
	{	
		-40, 						//min
		30,		 			//max 
		0,			 			//curr_val
		vacBooGauge,		//param_id
		"Vacuum/Boost",			//label
		UNITS_PSI,		//user units
		UNITS_PSI		//user units
	},
/////////////////////////////////////////////////////
//11.
	{	
		0, 						//min
		25.5,		 			//max 
		0,			 			//curr_val
		pulseWidth1Gauge,		//param_id
		"Pulse Width 1",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//12.
	{	
		0, 						//min
		25.5,		 			//max 
		0,			 			//curr_val
		pulseWidth2Gauge,		//param_id
		"Pulse Width 2",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//13.
	{	
		0, 						//min
		180,		 			//max 
		0,			 			//curr_val
		LCDASH_SPEEDO,			//param_id
		"Vehicle Speed",			//label
		UNITS_MPH,		//user units
		UNITS_MPH		//user units
	},
/////////////////////////////////////////////////////
//14.
	{	
		0, 						//min
		50000,		 			//max 
		0,			 			//curr_val
		LCDASH_VSS_PPM,			//param_id
		"VSS PPM",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//15.
	{	
		0, 						//min
		150,		 			//max 
		0,			 			//curr_val
		LCDASH_OIL_PRESSURE,			//param_id
		"Oil Pressure",			//label
		UNITS_PSI,		//user units
		UNITS_PSI		//user units
	},
/////////////////////////////////////////////////////
//16.
	{	
		0, 						//min
		100,		 			//max 
		0,			 			//curr_val
		throttleGauge,		//param_id
		"throttle",			//label
		UNITS_NONE,		//user units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
};

#endif
