#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>

#include "usb.h"
#include "gauge.h"
#include "megasquirt.h"
#include "ms1_defs.h"
#include "ms1_GM_thermistor.h"
#include "ms1_MAP_4250_2.5bar.h"
#include "ms1_EGO_narrowBand.h"
#include "ms1.h"
#include "speedo.h"
#include "sensors_oilpressure.h"

EEMEM unsigned char throttle_factor[255]; 	//our throttle position sensor table stored in EEPROM
extern MS_INFO_STRUCT ms_info; 				//!< An externally created structure from megasquirt.c
EEMEM char ee_ms1_signature[MS1_SIG_SIZE];


void ms1_saveSignature(char* signature)
{

eeprom_write_block(signature, &ee_ms1_signature, MS1_SIG_SIZE);

}

void ms1_restoreSignature(char* signature)
{

eeprom_read_block(signature, &ee_ms1_signature, MS1_SIG_SIZE);

}




void ms1_tpsGen(int low,int high)
{

unsigned char x;
uint8_t temp_tps[255];

	for(x=0;x<255;x++)
	 {
	 	if (x <=low) temp_tps[x]=0;
		else if (x >=high) temp_tps[x]=100;
		else temp_tps[x]=(uint8_t)( (x-low)*100/(high-low) );
	 }

	for (x=0;x<255;x++)
		{
		eeprom_write_byte(&throttle_factor[x],temp_tps[x]);
		}

} //end tpsGen




void ms1_init()
{

//restore info about thier sensors
//ms_info.sensors.MAT = GM_MAT;
//ms_info.sensors.CLT = GM_CLT;
//ms_info.sensors.EGO = INNOVATE_0_5_LINEAR;
//ms_info.sensors.MAP = MPX4250;
ms_restoreMSsensors();

}


void ms1_restoreDefaultGauges()
{

//setup everything for MS1
GAUGE ** gauge_list;
unsigned char gauge_count,x;
	
	gauge_list = gauge_getGaugePointers(&gauge_count); //get pointers to the gauges

//setup gauges for ms1
for (x=0;x<gauge_count;x++)
	{
	switch(x)
		{
		case 0:
		gauge_getGauge(gauge_list[x],rpmHiResGauge);
		break;

		case 1:
		gauge_getGauge(gauge_list[x],cltGauge);
		break;
		
		case 2:
		gauge_getGauge(gauge_list[x],matGauge);
		break;

		case 3:
		gauge_getGauge(gauge_list[x],afrGauge1);
		break;

		case 4:
		gauge_getGauge(gauge_list[x],advanceGauge);
		break;

		case 5:
		gauge_getGauge(gauge_list[x],egoCorrGauge1);
		break;

		default:
		gauge_getGauge(gauge_list[x],rpmHiResGauge);
		break;
		}
	}

}


unsigned char ms1_getTPS(unsigned char adc)
{

return eeprom_read_byte(&throttle_factor[adc]);	

}


int ms1_getTemperature(unsigned char adc)
{

	return pgm_read_word(&GM_thermfactor[adc]);

}


int ms1_getBaro(unsigned char adc)
{
	return(pgm_read_byte(&MAP_4250_lookup[adc]));
}


int ms1_getPressure(unsigned char adc)
{

	if (adc==0) //dead case
		return 0;

	switch(ms_info.sensors.MAP)
	 {
	 case MPX4250:
		return(pgm_read_byte(&MAP_4250_lookup[adc])); //MPX4250 2.5Bar Standard MAP sensor
		break;
	 case MPX4115:
	 	return(0);
		break;
	 case MPXH6300A:
	 	return (int)((adc+1.53)*1.213675);
		break;
	 case MPXH6400A:
	 	return (int)((adc+2.147)*1.6197783);
		break;
	 default:
	 	return 0;
		break;
	 }

	 return 0;

} //end ms1_getPressure()




float ms1_getAFR(unsigned char adc)
{

float afr;

if (adc==0)
	return 0; //a dead value

	switch (ms_info.sensors.EGO)
	 {
		case NARROW_BAND:
		afr = (float)pgm_read_word(&EGO_narrowBand_lookup[adc])/100.0;
		break;
		case WB_1_0_LINEAR:
		afr = 1.5 - 5.0 * (float)adc/255.0;
		break;
		case DYNOJET_LINEAR:
		afr = (float)adc * 0.031373 + 10;
		break;
		case TECHEDGE_LINEAR:
		afr = (float)adc* 0.039216 + 9;
		break;
		case INNOVATE_1_2_LINEAR:
		afr = ( (float)adc * 0.1961);
		break;
		case INNOVATE_0_5_LINEAR:
		afr = 10 + ( (float)adc * 0.039216);
		break;
		case INNOVATE_LC1_DEFAULT:
		afr =  7.35 + (float)adc * 0.057647;
		break;
		case AEM_LINEAR:
		afr = 9.72 + (float)adc * 0.038666;
		break;
		default:
		afr = 0;
		break;
	}

return afr;
}


void ms1_convertSensors(GAUGE * g_list[],unsigned char gauge_count)
{

GAUGE * g;

unsigned char * buff=USB_getBuff();

while(gauge_count--)
	{
	g = g_list[gauge_count];

	
	if (g->param_id == rpmHiResGauge)
		{
		unsigned int  iTime;
		unsigned int iTimeX;
		float iTimeFull;
		float RPM;

		iTime = ((int)buff[ms1_ITIME+ms_info.system.hr_offSet1+ms_info.system.hr_offSet2] <<8) + (int)buff[ms1_ITIME+1 +ms_info.system.hr_offSet1+ms_info.system.hr_offSet2];
		iTimeX = (int)buff[ms1_ITIMEX+ms_info.system.hr_offSet1+ms_info.system.hr_offSet2];
		iTimeFull = (float)(iTimeX*(65536.0)) + iTime;
		
		if (iTimeFull > 0) 
			RPM = (float)( (60000000.0*(2.0-ms_info.engine.twostroke1) )/((float)iTimeFull * ms_info.engine.ncylinders1)); //RPMhiRes
		else RPM=0;


		g->curr_val = RPM;
		}
	else

	if (g->param_id == pulseWidth1Gauge)
		{

		if (ms_info.system.ms_firmware_sig ==MS1_EXTRA_SIG)
		 	{
			g->curr_val = (float)(buff[ms1_PULSEWIDTH1])*0.1;
		 	}
		else if (ms_info.system.ms_firmware_sig ==MS1_HIRES_SIG)
			{
		 	unsigned int pw1_temp;
			pw1_temp = (buff[ms1_PULSEWIDTH1] << 8) + buff[ms1_PULSEWIDTH1+1];
			g->curr_val = (float)pw1_temp*0.001;
		 	}
		}
	else

	if (g->param_id == pulseWidth2Gauge)
		{

		if (ms_info.system.ms_firmware_sig ==MS1_EXTRA_SIG)
		 	{
			g->curr_val = (float)(buff[ms1_PULSEWIDTH2])*0.1;
		 	}
		else if (ms_info.system.ms_firmware_sig ==MS1_HIRES_SIG)
			{
		 	unsigned int pw1_temp;
			pw1_temp = (buff[ms1_PULSEWIDTH2] << 8) + buff[ms1_PULSEWIDTH2+1];
			g->curr_val = (float)pw1_temp*0.001;
		 	}
		}
	else

	if (g->param_id == advanceGauge)
		{
		g->curr_val = (float)(buff[ms1_ADVANCE+ms_info.system.hr_offSet1+ms_info.system.hr_offSet2]* 0.352)-10;
		}
	else

	if (g->param_id == cltGauge)
		{
		g->curr_val = (float)ms1_getTemperature(buff[ms1_CLTADC]);
		}
	else

	if (g->param_id == matGauge)
		{
		g->curr_val = (float)ms1_getTemperature(buff[ms1_MATADC]);
		}
	else

	if (g->param_id == mapGauge)
		{
		g->curr_val = (float)ms1_getPressure(buff[ms1_MAPADC]);
		}
	else

	if (g->param_id == throttleGauge)
		{
		g->curr_val = (float)ms1_getTPS(buff[ms1_TPSADC]);
		}
	else

	if (g->param_id == afrGauge1)
		{
		g->curr_val = ms1_getAFR(buff[ms1_EGOADC]);
		}
	else
		
	if (g->param_id == vacBooGauge)
		{
		int map 		= ms1_getPressure(buff[ms1_MAPADC]);
		int baro		= ms1_getBaro(buff[ms1_BAROADC]);
		
		if (  map < baro )
			{
			//return vacuum
			g->curr_val = (float)( (baro-map) * 0.2953007 * -1);
			}
		else
			{
			//return booost
			g->curr_val = (float)( ( map-baro) * 0.1450377);
			}
		
		}
	else

	if (g->param_id == LCDASH_SPEEDO)
		{
		g->curr_val = (float)speedo_getMPH();
		}
	else

	if (g->param_id ==LCDASH_VSS_PPM)
		{
		g->curr_val = (float)speedo_getPulsesPerMile(); //get the vehicle speed
		}
	else

	if (g->param_id ==LCDASH_OIL_PRESSURE)
		{
		g->curr_val = (float)oil_pressure_getPressure(); //get the vehicle speed
		}

	} //end while
} //ms1_convertSensors()
