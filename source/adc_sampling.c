#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "bitops.h"
#include "touchscreen.h"
#include "sensors_oilpressure.h"
#include "adc_sampling.h"
#include "mut.h"
#include "usb.h"
#include "LCDash.h"

unsigned char adcCurrChan=0;
unsigned char adc_delay=0;
unsigned int mutKeepAlive=0;
extern volatile unsigned char mut_status;
volatile unsigned long millisVal=0;
unsigned char millisCounter=0;

////////////////////////////////////////////////
//This is our 128uS time base from timer 1
//timer 1 also is doing backlight PWM
////////////////////////////////////////////////
SIGNAL (SIG_OUTPUT_COMPARE1A) //every 128us
{

if (adc_delay == ADC_DELAY_MAX)
	{

	adc_delay = 0; //reset the counter
	adcCurrChan++; //next channel
	if (adcCurrChan == ADC_TOTAL_CHANNELS) 
		adcCurrChan = 0; //loop back around to the first channel



	switch(adcCurrChan)
		{

		case ADC_CHAN0:
		//Touchscreen: XPLUS
		touchscreen_setup_x();
		SETBIT(ADCSRA,ADSC); //start conversion!
		break;



		case ADC_CHAN1:
		//Touchscreen: YMINUS
		touchscreen_setup_y();
		SETBIT(ADCSRA,ADSC); //start conversion!
		break;

		case ADC_CHAN2:
		oil_pressure_setupADC();
		SETBIT(ADCSRA,ADSC); //start conversion!
		break;

		}//end switch

	



	}
else
	{
	adc_delay++; //increment the counter
	}


//millisecond counter
if (millisCounter >= MILLISMAX)
	{
	millisVal++; //add one mS
	millisCounter = 0; //reset counter
	}
else
	millisCounter++;


if (LCDash_descrProtocol() == PROTOCOL_MUT)
	{
	mutKeepAlive++;
	if (mutKeepAlive>=MUT_KEEPALIVEMAX)
		{
		if (mut_status == MUT_RECVOK)
			{
			USB_putc(0x17);	//ask for TPS to keep alive, only if we're not currently recving
			}
		mutKeepAlive = 0;
		}
	}

} //end interrupt






////////////////////////////////////////////////
//ADC sample complete!
/////////////////////////////////////////////////
SIGNAL (SIG_ADC) 
{

	switch(adcCurrChan)
		{

		case ADC_CHAN0:
		touchscreen_process_x(ADCH); 
		break;

		case ADC_CHAN1:
		touchscreen_process_y(ADCH);
		break;


		case ADC_CHAN2:
		oil_pressure_processADC(ADC);
		break;

		}

}





unsigned long millis(void)
{
	return millisVal;
}
