/*! \file megasquirt.h
	\brief Megasquirt related functions, structures here.
*/

#ifndef _MEGASQUIRT_H_
#define _MEGASQUIRT_H_

#include "../gauge.h"

#define MS1_EXTRA_SIG 1
#define MS1_HIRES_SIG 2
#define MS2_SIG		  3

/*! Set the Megasquirt version */
void ms_init();

void ms_restoreDefaultGauges();

/*! Restores standard parameters into thier gauges */
void ms2_restoreDefaultGauges(); 	

/*! Gets one character from a page in Megasquirt memory */
unsigned char ms_getChar(unsigned char page,unsigned char index);

/*! Sends the commands to full our receive buffer with megasquirt data */
void ms_getSensors();

/*! Converts megasquirt parameters from a list of gauges */
void ms_convertSensors(GAUGE * g_list[],unsigned char gauge_count);

/*! Converts megasquirt I parameters from a list of gauges */
void ms1_convertSensors(GAUGE * g_list[],unsigned char gauge_count);

/*! Converts megasquirt II parameters from a list of gauges */
void ms2_convertSensors(GAUGE * g_list[],unsigned char gauge_count);

/*! Gets the list of param_ids and labels for each type of watchable gauge */
//void ms_getGauge_labels(unsigned char paramIdList[], char * labelList);

//void ms_getDefaultMinMax(unsigned char param_id, float * min, float * max);

/*! Gets the entire gauge containing min, max, label, etc. based on the param_id */
//void ms_getGauge(GAUGE * g,unsigned char param_id);

/*! Gets the number of supported gauges based on the Megasquirt Firmware version */
unsigned char ms_getGaugeCount();

/*! Gives the list of gauges depending on the MegaSquirt Firmware version */
GAUGE* ms_getGaugeList();

/*! Some datalog testing */
void ms_dataLog(float* num);

void ms_startDatalog();

/*! Get the firmware string */
void ms_getFirmwareString(char * str);

/*! Returns the status of the connection */
unsigned char ms_isConnected();

void ms_storeMSsensors();
void ms_restoreMSsensors();

///////////////////////////////////
//
//Structures
//
///////////////////////////////////

typedef struct MS_System
	{
	unsigned char ms_firmware_sig;	//!< The current megasquirt firmware type
	unsigned char hr_offSet1;		//!< A high res offset of the output variables
	unsigned char hr_offSet2;		//!< Another high res offset of the output variables
	} MS_SYSTEM;

typedef struct MS_comms
	{
	unsigned char ms_connected;		//!< Are we connected?
	unsigned char ms_min_resp_len; 
	unsigned char ms_max_resp_time;
	unsigned char ms_receive_command[3];
	unsigned char ms_receive_command_len;
	unsigned char pageActivationDelay;
	} MS_COMMS;

typedef struct MS_Engine
	{
	unsigned char ncylinders1; 		//!< MS1, number of cylinders
	unsigned char twostroke1;		//!< MS1, for calculating RPM
	unsigned char injopen1;			//!< MS1, for calculating pulsewidth
	} MS_ENGINE;

typedef struct MS_sensors
	{
	unsigned char MAP; 				//!< MS1, Their map sensor type
	unsigned char CLT;				//!< MS1, Their coolant sensor type
	unsigned char MAT;				//!< MS1, Their MAT sensor type
	unsigned char EGO;				//!< MS1, Their EGO sensor type
	} MS_SENSORS;

typedef struct Ms_Info
	{
	MS_COMMS	comms;
	MS_SYSTEM 	system;
	MS_ENGINE 	engine;
	MS_SENSORS 	sensors;
	} MS_INFO_STRUCT;





#endif
