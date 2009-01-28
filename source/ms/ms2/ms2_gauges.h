/*! \file megasquirt_gauges.h
	\brief Gauges for Megasquirt system are initialized here
	When adding new guages, put their min,max,label,units, etc. here
*/

#ifndef _MS2_GAUGES_H_
#define _MS2_GAUGES_H_

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>



///////Begin Gauge Storage///////////////////////////////////////////////
#include "units.h"
#include "ms2_defs.h" 
#include "speedo.h"
#include "sensors_oilpressure.h"

#define MS2_GAUGE_CNT	23 	//!The total number of gauges

GAUGE ms2_GaugeList[]  PROGMEM = {

///////////////////////////////////////////////////////
//1.
	{	
		0, 			 	//min
		7000,		 	//max 
		0,			 	//curr_val
		ms2_RPM,	 	//param_id
		"RPM",	//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//2.
	{	
		100, 			 //min
		240,		 	//max 
		0,			 	//curr_val
		ms2_COOLANT,	//param_id
		"Coolant",		//label
		UNITS_FAREN,		//units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//3.
	{	
		10.0, 			 //min
		19.0,		 	//max 
		0,			 	//curr_val
		ms2_AFR1,		//param_id
		"AFR1",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//4.
	{	
		10.0, 			 //min
		19.0,		 	//max 
		0,			 	//curr_val
		ms2_AFR2,		//param_id
		"AFR2",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//5.
	{	
		0, 			 	//min
		250,		 	//max 
		0,			 	//curr_val
		ms2_MAP,		//param_id
		"MAP",			//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//6.
	{	
		0, 			 		//min
		30,		 			//max 
		0,			 		//curr_val
		ms2_ADVANCE,		//param_id
		"Advance",		//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//7.
	{	
		0, 				//min
		150,		 	//max 
		0,			 	//curr_val
		ms2_MAT,		//param_id
		"Air Temp",	//label
		UNITS_FAREN,		//units
		UNITS_FAREN		//user units
	},
/////////////////////////////////////////////////////
//8.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_EGOCORRECTION1,		//param_id
		"EGO correct 1",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//9.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_EGOCORRECTION2,		//param_id
		"EGO correct 2",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//10.
	{	
		40, 						//min
		200,		 			//max 
		0,			 			//curr_val
		ms2_EAEFUELCORR,		//param_id
		"EAE Fuel Corr",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//11.
	{	
		0, 						//min
		25.5,		 			//max 
		0,			 			//curr_val
		ms2_PULSEWIDTH1,		//param_id
		"Pulse Width 1",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//12.
	{	
		0, 						//min
		25.5,		 			//max 
		0,			 			//curr_val
		ms2_PULSEWIDTH2,		//param_id
		"Pulse Width 2",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//13.
	{	
		0, 						//min
		10,		 			//max 
		0,			 			//curr_val
		ms2_DWELL,		//param_id
		"Dwell",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//14.
	{	
		0, 						//min
		100,		 			//max 
		0,			 			//curr_val
		ms2_TPS,		//param_id
		"TPS",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//15.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_AIRCORRECTION,		//param_id
		"Gair",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//16.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_WARMUPENRICH,		//param_id
		"Gwarm",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//17.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_BAROCORRECTION,		//param_id
		"Gbaro",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//18.
	{	
		50, 						//min
		150,		 			//max 
		0,			 			//curr_val
		ms2_GAMMAENRICH,		//param_id
		"Gammae",			//label
		UNITS_NONE,		//units
		UNITS_NONE		//user units
	},
/////////////////////////////////////////////////////
//19.
	{	
		0, 						//min
		120,		 			//max 
		0,			 			//curr_val
		ms2_VECURR1,		//param_id
		"Gve",			//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////
//20.
	{	
		0, 						//min
		120,		 			//max 
		0,			 			//curr_val
		ms2_VECURR2,		//param_id
		"Gve2",			//label
		UNITS_KPA,		//units
		UNITS_KPA		//user units
	},
/////////////////////////////////////////////////////

//21.
	{	
		0, 						//min
		180,		 			//max 
		0,			 			//curr_val
		LCDASH_SPEEDO,			//param_id
		"Vehicle Speed",			//label
		UNITS_MPH,		//units
		UNITS_MPH		//user units
	},
/////////////////////////////////////////////////////
//22.
	{	
		0, 						//min
		50000,		 			//max 
		0,			 			//curr_val
		LCDASH_VSS_PPM,			//param_id
		"VSS PPM",			//label
		UNITS_MPH,		//units
		UNITS_MPH		//user units
	},
/////////////////////////////////////////////////////
//23.
	{	
		0, 						//min
		150,		 			//max 
		0,			 			//curr_val
		LCDASH_OIL_PRESSURE,			//param_id
		"Oil Pressure",			//label
		UNITS_PSI,		//units
		UNITS_PSI		//user units
	},
/////////////////////////////////////////////////////
};

#endif
