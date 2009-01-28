////////////////////////////////////////////////////////////////////
/*! 	\file ms1_defs.h
		\brief Megasquirt I Output Channel location definitions
*/
////////////////////////////////////////////////////////////////////
#ifndef _MS1_DEFS_H_
#define _MS1_DEFS_H_

#define MS1_MIN_RESP_LENGTH 34
#define MS1_MAX_RESP_TIME 	100


///////////////////////////////////
//EGO Sensors
///////////////////////////////////
#define O2_SENSOR_CNT			9 //!< Change this when adding more 02 sensors

#define NARROW_BAND				0
#define WB_1_0_LINEAR			1
#define DYNOJET_LINEAR			2
#define TECHEDGE_LINEAR			3
#define INNOVATE_1_2_LINEAR		4
#define INNOVATE_0_5_LINEAR		5
#define INNOVATE_LC1_DEFAULT	6
#define	AEM_LINEAR				7
#define NoEGO					8

///////////////////////////////////
//MAP Sensors
///////////////////////////////////
#define MPX4250					0	//250KPA Standard
#define MPX4115					1	//115KPA
#define MPXH6300A				2	//300KPA
#define MPXH6400A				3	//400KPA

///////////////////////////////////
//CLT Sensors
///////////////////////////////////
#define GM_CLT					0
#define OTHER_CLT				1	

///////////////////////////////////
//MAT Sensors
///////////////////////////////////
#define GM_MAT					0
#define OTHER_MAT				1










//////////////////////////////////
//Output Channels
///////////////////////////////////
#define ms1_SECL			0
#define ms1_SQUIRT			1
#define ms1_ENGINE			2
#define ms1_BAROADC			3
#define ms1_MAPADC			4
#define ms1_MATADC			5
#define ms1_CLTADC			6
#define ms1_TPSADC			7
#define ms1_BATADC			8
#define ms1_EGOADC			9
#define ms1_EGOCORRECTION	10
#define ms1_AIRCORRECTION	11
#define ms1_WARMUPENRICH	12
#define ms1_RPM100			13
#define ms1_PULSEWIDTH1		14
#define ms1_ACCELENRICH		15
#define ms1_BAROCORRECTION	16
#define ms1_GAMMAENRICH		17
#define ms1_VECURR1			18
#define ms1_PULSEWIDTH2		19
#define ms1_VECURR2			20
#define ms1_IDLEDC			21
#define ms1_ITIME			22
#define ms1_ADVANCE			24
#define ms1_AFRTARGET		25
#define ms1_FUELADC			26
#define ms1_EGTADC			27
#define ms1_CLTIATANGLE		28
#define ms1_KNOCKANGLE		29
#define ms1_EGOCORRECTION2	30
#define ms1_PORTA_raw		31
#define ms1_PORTB_raw		32
#define ms1_PORTC_raw		33
#define ms1_PORTD_raw		34
#define ms1_STACKL			35
#define ms1_TPSLAST			36
#define ms1_ITIMEX			37
#define ms1_bcDC			38




//////////////////////////////
//Extra gauges
///////////////////////////////
#define rpmHiResGauge		60
#define afrGauge1			61
#define mapGauge			62
#define cltGauge			63
#define throttleGauge		64
#define matGauge			65
#define advanceGauge		66
#define rpm100Gauge			67
#define vacBooGauge			68
#define targetAFRGauge		69

#define dutyCycle1Gauge		70
#define dutyCycle2Gauge		71
#define egoCorrGauge1		72
#define gammaEnrichGauge	73
#define pulseWidth1Gauge	74
#define pulseWidth2Gauge	75
#define veBucketGauge		76
#define veGauge				77
#define voltMeter			78
#define warmupEnrichGauge	79
#define fuelGauge			80
#define	egtGauge			81
#define cltIatGauge			82
#define knockGauge			83
#define egoCorrGauge2		84
#define lambdaGauge2		85
#define afrGauge2			86
#define vacuumGauge			87
#define boostGauge			88
#define clockGauge			89
#define deadGauge			90
#define mphGauge			91
#define gphGauge			92
#define mpgGauge			93

#define baroADCGauge		94
#define mapADCGauge			95
#define matADCGauge			96
#define cltADCGauge			97
#define tpsADCGauge			98
#define batADCGauge			99
#define lambdaGauge1		100
#define accelEnrichGauge	101

#define ms1_NONE			255
#endif
