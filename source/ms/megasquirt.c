#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>

#include "usb.h"
#include "t6963.h"
#include "touchscreen.h"
#include "gui.h"
#include "bitops.h"
#include "gauge.h"
#include "messageBar.h"
#include "FT232R.h"
#include "LCDash.h"
#include "speedo.h"
#include "sensors_oilpressure.h"
#include "adc_sampling.h"

#include "megasquirt.h"
#include "ms2_gauges.h"
#include "ms1_gauges.h"
#include "ms1.h"


MS_INFO_STRUCT ms_info; //!< A structure to hold information about the MegaSquirt we're talking to
unsigned char file_opened=FALSE;
extern unsigned char dualDisplay_status;
EEMEM unsigned char ee_previous_firmware_sig;

EEMEM MS_INFO_STRUCT ee_ms_sensors;


//unsigned char ms_pageActivationDelay = 50;
long datalog_cnt=0;
unsigned char dualDisplayEnabled= FALSE;


void ms_restoreMSsensors()
{
eeprom_read_block(&ms_info.sensors,&ee_ms_sensors,sizeof(MS_SENSORS));
}

void ms_storeMSsensors()
{

eeprom_write_block(&ms_info.sensors,&ee_ms_sensors,sizeof(MS_SENSORS));
}


void ms_init()
{

	//setup the system with some defaults
	ms_info.system.ms_firmware_sig = MS2_SIG;
	ms_info.comms.ms_connected = FALSE;

}



void ms_restoreDefaultGauges()
{

if (ms_info.system.ms_firmware_sig == MS2_SIG)
	{
	ms2_restoreDefaultGauges();
	}
else
	ms1_restoreDefaultGauges();

}


void ms2_restoreDefaultGauges()
{
//setup everything for MS2
GAUGE ** gauge_list;
unsigned char gauge_count,x;
	
	gauge_list = gauge_getGaugePointers(&gauge_count); //get pointers to the gauges

//setup gauges for ms2
for (x=0;x<gauge_count;x++)
	{
	switch(x)
		{
		case 0:
		gauge_list[x]->param_id = ms2_RPM;
		break;

		case 1:
		gauge_list[x]->param_id = ms2_AFR1;
		break;
		
		case 2:
		gauge_list[x]->param_id = ms2_COOLANT;
		break;

		case 3:
		gauge_list[x]->param_id = ms2_MAP;
		break;

		case 4:
		gauge_list[x]->param_id = ms2_MAT;
		break;

		case 5:
		gauge_list[x]->param_id = ms2_ADVANCE;
		break;

		default:
		gauge_list[x]->param_id = ms2_RPM;
		break;
		}
	}
}	


void ms_getFirmwareString(char * str)
{

unsigned char len=0;

if ( (ms_info.system.ms_firmware_sig == MS2_SIG) )
	{
	USB_putc('Q');
	len = USB_read(20,200);
	}
else
	{
	USB_putc('T');
	len = USB_read(32,200);	
	}

strncpy(str,(char*)USB_getBuff(),len+1);

}



void ms_findFirmware()
{
char * buff = (char*)USB_getBuff();
char firmware_string[30];
unsigned char resp_len = 0;

	//-------------------Check for MS1/extra
	FT232R_setBaud(B9600);
	USB_putc('T'); 		//ask for the signature
	resp_len = USB_read(32,100); 	



////////Check for MS1/Extra///////////////////////////////////////////////////////////////////	
	if(strstr(buff,"Extra")) 
		{

		ms_info.system.hr_offSet1 = 0; //no hiRes offset
		ms_info.system.hr_offSet2 = 0;
		ms_info.system.ms_firmware_sig = MS1_EXTRA_SIG;
		

		ms_info.comms.ms_min_resp_len = MS1_MIN_RESP_LENGTH;
		ms_info.comms.ms_max_resp_time = MS1_MAX_RESP_TIME;
		ms_info.comms.ms_receive_command[0]='R';
		ms_info.comms.ms_receive_command[1]=0x00;
		ms_info.comms.ms_receive_command[2]=0x00;
		ms_info.comms.ms_receive_command_len = 1;
		ms_info.comms.pageActivationDelay = 50;

		if (strstr(buff,"hr")) //Check for MS1/Extra HiRes
			{

			ms_info.system.hr_offSet1 = 1; //yes, hiRes offset
			ms_info.system.hr_offSet2 = 1;
			ms_info.system.ms_firmware_sig = MS1_HIRES_SIG;
			}		
		
		strncpy(firmware_string,buff,30); //save the firmware string for display before we clear the buffer
		firmware_string[29] = 0x00;

		//get some info about their car setup.....
		//twostroke1
		ms_info.engine.twostroke1 =  (ms_getChar(1,182) & 0b00000100) >> 2;
		//ncylinders1
		ms_info.engine.ncylinders1 = ((ms_getChar(1,182) & 0b11110000) >> 4) + 1;
		//injoopen1 
		ms_info.engine.injopen1 = 	ms_getChar(1,151);	//!need to scale by 0.1 when using
		


		//check to see if they changed from MS2 
		if (eeprom_read_byte(&ee_previous_firmware_sig) != MS1_EXTRA_SIG &&
				eeprom_read_byte(&ee_previous_firmware_sig) != MS1_HIRES_SIG)
			{
			messageBar_addMsg("Previous MS was an MS2 variant");
			delay_ms(250);

			//their firmware used to be MS2, now it's MS1, so setup their gauges appropriately
			ms1_restoreDefaultGauges();
			eeprom_write_byte(&ee_previous_firmware_sig, ms_info.system.ms_firmware_sig); //save the firmware
			}

		ms1_init();
		ms_info.comms.ms_connected = TRUE; //now we're connected
		lcd_clear_graph(); //clear the screen
		gauge_drawGaugeLayout(); //redraw the screen
		messageBar_addMsg(firmware_string);
		return;
		}


	//-------------------Check for MS2
	FT232R_setBaud(B115200);
	USB_putc('Q'); 		//ask for the signature
	USB_read(20,100); 
	
	if (strstr(buff,"MS"))
		{
		if (strstr(buff,"MSII")) //check for MegaSquirt-II
			{
			ms_info.comms.ms_receive_command[0]='a';
			ms_info.comms.ms_receive_command[1]=0x00;
			ms_info.comms.ms_receive_command[2]=0x06;
			ms_info.comms.ms_receive_command_len = 3;
			}
		else
		if (strstr(buff,"MS2"))
			{
			ms_info.comms.ms_receive_command[0]='A';
			ms_info.comms.ms_receive_command[1]=0x00;
			ms_info.comms.ms_receive_command[2]=0x00;
			ms_info.comms.ms_receive_command_len = 1;
			}

		ms_info.comms.ms_min_resp_len = MS2_MIN_RESP_LENGTH;
		ms_info.comms.ms_max_resp_time = MS2_MAX_RESP_TIME;
		ms_info.system.ms_firmware_sig = MS2_SIG;
		ms_info.comms.pageActivationDelay = 100;
		
		if (eeprom_read_byte(&ee_previous_firmware_sig) != MS2_SIG)
			{
			//their firmware used to be MS1, now it's MS2, so setup their gauges appropriately
			ms2_restoreDefaultGauges();
			eeprom_write_byte(&ee_previous_firmware_sig,ms_info.system.ms_firmware_sig);
			}

		ms_info.comms.ms_connected = TRUE; //now we're connected
		lcd_clear_graph(); //clear the screen
		gauge_drawGaugeLayout(); //redraw the screen
		messageBar_addMsg(buff); //display their MS firmware		
		return;
		}


} //end ms_findFirmware()

unsigned char ms_isConnected()
{
return ms_info.comms.ms_connected;
}


unsigned char ms_getChar(unsigned char page,unsigned char index)
{
unsigned char * buff = USB_getBuff();
unsigned char recv_bytes=0;

	USB_putc('P');
	delay_ms(1);
	USB_putc(page);

	delay_ms(ms_info.comms.pageActivationDelay+30);
	USB_putc('V');


	recv_bytes = USB_read(189,300); //!!Delay for MS1/Extra only
	if (recv_bytes < 189)
		{
		//not enough bytes read.... comms too slow?
		char out[30];
		strncpy_P(out,PSTR("Not enough bytes from page"),30);
		messageBar_addMsg(out);
		}

	return buff[index];
}



void ms_getSensors()
{
	
	if (ms_info.comms.ms_connected == FALSE)
		{
		messageBar_addMsg("Searching for MegaSquirt");
		ms_findFirmware();
		}
	else
		{
			

			//////////////-Begin Dual display test-/////////////////////////////////////////////////
			
			//Find out if there's a request from the other port
			unsigned char * buff = USB_getBuff();
			char dualDisplay_request = FALSE;
			unsigned char receive_cnt = 0;			
			unsigned char req_str[1];
			
			if (dualDisplay_status == ON)
			{

			USB_selectOne(); //select the other 
			
			receive_cnt = USB_read(1,1); //get the message (if any)
			
			if (receive_cnt)
				{
				//gui_puts("                 ",0,0,0);
				//memcpy(req_str,buff,receive_cnt); //copy message into a temp string
				req_str[0] = buff[0];
				while(USB_read(1,1)); //clear the rest of dualdisplay's serial buffer
			
				unsigned char recv_amt=0;

				if ( req_str[0] == 0x00)
					{
					//nothing
					}
				else
				if ( req_str[0] == 'R' || req_str[0] == 'A')
					{
				//	gui_puts("Request -R/A-",0,8,0);
					dualDisplay_request = TRUE;
					}
				else
					{ 

						
						//send the message out to MegaSquirt, then back to the other display
						USB_selectZero(); //select the main
						
						/*
						for(x=0;x<receive_cnt;x++)
							{
							USB_putc(req_str[x]);
							delay_ms(1);
							}
						*/

						if (req_str[0] == 'P')
							{
						//	gui_puts("other command -P-",0,8,0);
							delay_ms(ms_info.comms.pageActivationDelay+30);
							USB_putc('V');
							recv_amt = 189;
							}
/*
						if (req_str[0] == 'T')
							{
							gui_puts("other command -T-",0,8,0);
							USB_putc('T');
							recv_amt = 30;
							}
							*/
						if (req_str[0] == 'Q')
							{
						//	gui_puts("other command -Q-",0,8,0);
							USB_putc('Q');
							recv_amt = 20;
							}


						//??!! What length of delay here, since we don't know what their asking??
						receive_cnt = USB_read(recv_amt, recv_amt); //get their message

						dualDisplay_send(buff,receive_cnt);	//send their response to the dual display
						}

				} //end if receive_cnt


			USB_selectZero(); //select the main
			//////////////End Dual display test-/////////////////////////////////////////////////
			}


			
			
			
			USB_putc(ms_info.comms.ms_receive_command[0]);

			if (ms_info.comms.ms_receive_command[2] != 0x00) //for MSII standard
				{
				delay_ms(1);
				USB_putc(ms_info.comms.ms_receive_command[1]);
				delay_ms(1);
				USB_putc(ms_info.comms.ms_receive_command[2]);
				}
			
			receive_cnt = USB_read(200, ms_info.comms.ms_max_resp_time);
			
			if (receive_cnt < ms_info.comms.ms_min_resp_len) //reading a large response, ensuring we timeout
				{
					ms_info.comms.ms_connected = FALSE; //we didn't get enough values back, maybe ms is off
				}
			else
				{
				//testing dual display
					if (dualDisplay_request == TRUE)
						dualDisplay_send(buff,receive_cnt);
				}

			
				

		}

} //end ms_getSensors()



void ms_convertSensors(GAUGE * g_list[],unsigned char gauge_count)
{

if (ms_info.comms.ms_connected == TRUE) //only if we're connected 
	{
	if (ms_info.system.ms_firmware_sig == MS2_SIG)
		{
		ms2_convertSensors(g_list,gauge_count);
		}
	else
	if ( (ms_info.system.ms_firmware_sig == MS1_EXTRA_SIG) || (ms_info.system.ms_firmware_sig == MS1_HIRES_SIG) )
		ms1_convertSensors(g_list,gauge_count);
	}
else
	{
	//---Zero out all the values
	unsigned char x;
	for (x=0;x<gauge_count;x++)
		g_list[x]->curr_val = 0; //zero out the gauges
	}


} //end ms_convertSensors()







void ms2_convertSensors(GAUGE * g_list[],unsigned char gauge_count)
{

GAUGE * g;

unsigned char * buff=USB_getBuff();

while(gauge_count--)
	{
	g = g_list[gauge_count];

	if (g->param_id == ms2_RPM)
		{
		g->curr_val = (float)( (unsigned int)( (buff[ms2_RPM] << 8) +buff[ms2_RPM+1]));
		}
	else

	if (g->param_id == ms2_COOLANT)
		{
		g->curr_val = (float)((signed int)( (buff[ms2_COOLANT]<<8) + buff[ms2_COOLANT+1])*0.1);
		}
	else

	if (g->param_id == ms2_AFR1)
		{
		g->curr_val = (float)((signed int)((buff[ms2_AFR1]<<8)+buff[ms2_AFR1+1] )*0.1); 
		}
	else
	
	if (g->param_id == ms2_AFR2)
		{
		g->curr_val = (float)((signed int)((buff[ms2_AFR2]<<8)+buff[ms2_AFR2+2] )*0.1); 
		}
	else

	if (g->param_id == ms2_MAP)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_MAP]<<8) + buff[ms2_MAP+1])*0.1); 
		}
	else

	if (g->param_id == ms2_ADVANCE)
		{
		g->curr_val = (float)( 0.1*(signed int)( (buff[ms2_ADVANCE]<<8)+buff[ms2_ADVANCE+1]) );
		}
	else

	if (g->param_id == ms2_MAT)
		{
		g->curr_val = (float)((signed int)( (buff[ms2_MAT]<<8) + buff[ms2_MAT+1])*0.1);
		}
	else

	if (g->param_id ==ms2_EGOCORRECTION1)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_EGOCORRECTION1]<<8) + buff[ms2_EGOCORRECTION1+1]) );
		}
	else

	if (g->param_id ==ms2_EGOCORRECTION2)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_EGOCORRECTION2]<<8) + buff[ms2_EGOCORRECTION2+1]) );
		}
	else

	if (g->param_id ==ms2_EAEFUELCORR)
		{
		g->curr_val = (float)( (unsigned int)((buff[ms2_EAEFUELCORR]<<8) + buff[ms2_EAEFUELCORR+1]) );
		}
	else

	if (g->param_id ==ms2_PULSEWIDTH1)
		{
		g->curr_val = (float)( (unsigned int)((buff[ms2_PULSEWIDTH1]<<8) + buff[ms2_PULSEWIDTH1+1]) * 0.000666);
		}
	else

	if (g->param_id ==ms2_PULSEWIDTH2)
		{
		g->curr_val = (float)( (unsigned int)((buff[ms2_PULSEWIDTH2]<<8) + buff[ms2_PULSEWIDTH2+1]) * 0.000666);
		}
	else

	if (g->param_id ==ms2_DWELL)
		{
		g->curr_val = (float)( (unsigned int)((buff[ms2_DWELL]<<8) + buff[ms2_DWELL+1]) * 0.0666);
		}
	else

	if (g->param_id ==ms2_TPS)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_TPS]<<8) + buff[ms2_TPS+1]) * 0.1);
		}
	else

	if (g->param_id ==ms2_AIRCORRECTION)
		{
		g->curr_val = (float)(signed int)((buff[ms2_AIRCORRECTION]<<8) + buff[ms2_AIRCORRECTION+1]);
		}
	else

	if (g->param_id ==ms2_WARMUPENRICH)
		{
		g->curr_val = (float)(signed int)((buff[ms2_WARMUPENRICH]<<8) + buff[ms2_WARMUPENRICH+1]);
		}
	else

	if (g->param_id ==ms2_BAROCORRECTION)
		{
		g->curr_val = (float)(signed int)((buff[ms2_BAROCORRECTION]<<8) + buff[ms2_BAROCORRECTION+1]);
		}
	else

	if (g->param_id ==ms2_GAMMAENRICH)
		{
		g->curr_val = (float)(signed int)((buff[ms2_GAMMAENRICH]<<8) + buff[ms2_GAMMAENRICH+1]);
		}
	else

	if (g->param_id ==ms2_VECURR1)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_VECURR1]<<8) + buff[ms2_VECURR1+1])*0.1);
		}
	else

	if (g->param_id ==ms2_VECURR2)
		{
		g->curr_val = (float)( (signed int)((buff[ms2_VECURR2]<<8) + buff[ms2_VECURR2+1])*0.1);
		}
	else

	if (g->param_id ==LCDASH_SPEEDO)
		{
		g->curr_val = (float)speedo_getMPH(); //get the vehicle speed
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


}





/*
void ms_getGauge(GAUGE * g,unsigned char param_id)
{

unsigned char x;
GAUGE ram_g;
GAUGE * gaugeList= ms_getGaugeList();
	

	for (x=0; x< ms_getGaugeCount();x++)
		{
		memcpy_P(&ram_g,&gaugeList[x],sizeof(GAUGE));

		if ( param_id == ram_g.param_id)
			{
			memcpy(g,&ram_g,sizeof(GAUGE));
			}
		}
}
*/

/*
void ms_getDefaultMinMax(unsigned char param_id, float * min, float * max)
{

unsigned char x;
GAUGE * gaugeList = ms_getGaugeList();

	for (x=0; x<ms_getGaugeCount();x++)
		{

		if ( pgm_read_byte(gaugeList[x].param_id) == param_id )
			{
			memcpy_P(min,&gaugeList[x].min,4); 
			memcpy_P(max,&gaugeList[x].max,4);
			}

		}
}
*/


GAUGE* ms_getGaugeList()
{

	if (ms_info.system.ms_firmware_sig == MS2_SIG)
		return &ms2_GaugeList[0];
	else
	if ( (ms_info.system.ms_firmware_sig == MS1_EXTRA_SIG) || (ms_info.system.ms_firmware_sig == MS1_HIRES_SIG) )
		return &ms1_GaugeList[0];

return 0; //should never get here
}


/*
void ms_getGauge_labels(unsigned char paramIdList[], char * labelList)
{

unsigned char x;

GAUGE * gaugeList= ms_getGaugeList();
GAUGE g;

for (x=0; x<ms_getGaugeCount();x++)
	{
	memcpy_P(&g,&gaugeList[x],sizeof(GAUGE));
	paramIdList[x] = g.param_id;
	
	strncpy(labelList+(x*GAUGE_LABEL_LENGTH),g.label,GAUGE_LABEL_LENGTH); //get the label string
	}
}
*/

unsigned char ms_getGaugeCount()
{

	//decide based on current ms firmware
	if ( ms_info.system.ms_firmware_sig == MS2_SIG)
		return MS2_GAUGE_CNT;
	else
	if ( (ms_info.system.ms_firmware_sig == MS1_EXTRA_SIG) || (ms_info.system.ms_firmware_sig == MS1_HIRES_SIG) )
		return MS1_GAUGE_CNT;

return 0; //should never get here
}


void ms_startDatalog()
{
unsigned char x;
char messages[100];
//char* buff = (char*)USB_getBuff();

	for (x=0;x<100;x++)
	messages[x]=0x00;


	USB_commandMode();

		USB_puts("SC 1\r"); //select the flash drive
		USB_read(5,500);


	USB_puts("CD ..\r");
	USB_read(5,500);

	USB_puts("OPW Datalog.xls\r");
	USB_read(5,500);

	USB_puts("WRF 5\r");
	USB_puts("RPM\r\n\r");
	USB_read(5,500);

	USB_puts("CLF Datalog.xls\r");
	USB_read(5,500);

		USB_puts("SC 0\r"); //select the FT232R
		USB_read(5,500);

	while(USB_read(1,1)); //clear the buffer

	USB_dataMode(); //back into datamode
	
}







void ms_dataLog(float* num)
{


//if (ms_info.comms.ms_connected == TRUE)
//	{
//	unsigned long cnt=0;
	unsigned int x=0;
			char value_out[8];
//			char size[4];
	unsigned char item;
	unsigned char * buff = USB_getBuff();

				USB_commandMode();

					USB_puts("SC 1\r"); //select the flash drive
					USB_read(5,500);

				USB_puts("OPW log.xls\r");
				USB_read(5,500);

					while(1)
						{
						USB_puts("E\r");
						USB_read(1,1);
						if (buff[0] == 'E')
							{
							while(USB_read(1,1)); //clear the buffer
							break;
							}
						}



	for(x=0;x<20;x++)
		{
			
			dtostrf(millis(),6,0,value_out);
			//unsigned char len = strnlen(value_out,7);
			
			USB_puts("WRF "); USB_puts("37"); USB_putc('\r');

			for (item=0;item<5;item++)
				{	
					USB_puts(value_out); USB_putc(9);
				}

			USB_puts("\r\n"); USB_putc('\r');
			USB_read(5,500);

					while(1)
						{
						USB_puts("E\r");
						USB_read(1,1);
						if (buff[0] == 'E')
							{
							while(USB_read(1,5)); //clear the buffer
							break;
							}
						}

		} 


				USB_puts("CLF log.xls\r");
				USB_read(5,500);
				while(USB_read(1,1)); //clear the buffer
}
