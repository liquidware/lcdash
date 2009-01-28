#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>

#include "speedo.h"
#include "bitops.h"

unsigned int TF_slope= 7031; //FORD VSS = 7031

volatile unsigned char mph=0; //the actual MPH
unsigned char pulse_count=0; //our pulse counter
unsigned int period_saved=0; //storage for our timer 3 value
unsigned char vehicle_calSpeed = 30; //mph to calibrate at

unsigned char timed_wait = TRUE;



unsigned char speedo_getMPH()
{
	return mph;
}


unsigned int speedo_getPulsesPerMile()
{
	//Drive at 30 MPH
	//This will return your pulses per mile
	//Equation: ppm = (56250000/vehicle_speed)/timer3_value;
	
	//return 937500/period_saved; //@60mph
	
	return (56250000/vehicle_calSpeed)/period_saved;
}



void speedo_calibrate()
{
	//calibrating at the vehicle_calSpeed (60mph for right now)
	unsigned int ppm = speedo_getPulsesPerMile();

	TF_slope = 56250000/ppm;	
}



void speedo_init()
{

//Setup the timer
	
	//----Speed input capture timer (Timer-3)
	//Using timer 3, prescale f-osc by 1024 = 64usec tick
	TCCR3B = (1<<CS32) | (0<<CS31) | (1<<CS30);

	SETBIT(TIMSK3,TOIE3); //enable timer1 overflow interrupt 


//PD2 (INT2 pin) as speedo input
EIMSK |= (0 << INT2); //disable interrupt 2
EICRA |= (1<<ISC21) | (1<<ISC20); //rising edge triggered
EIFR |= (1<<INT2);
EIMSK |= (1 << INT2); //(re)enable interrupt 2

period_saved = 0;
pulse_count = 0; //init the pulse count
mph = 0; //init mph
TCNT3 = 0; //reset the timer
timed_wait = TRUE;
}


/////////////////////////////////////////////////
//PD2 (INT2 pin) as speedo input
//
//Interrupt on rising edge, 
//process the time between rising edges -use Timer 3 (16-bit) for timing
/////////////////////////////////////////////////
SIGNAL (SIG_INTERRUPT2)
{


if (timed_wait)
	{
	
	if (TCNT3 > 1560) 
		{
		timed_wait = FALSE;
		TCNT3= 0; //start the timer
		}


	}
else
	{

	pulse_count++;

	if (pulse_count >= AVERAGE_SAMPLES)
		{
		
		unsigned int averaged_time = TCNT3/AVERAGE_SAMPLES;
		
		mph = TF_slope/averaged_time; //take the slope and divide it by the timer3 period
		period_saved = averaged_time; //save the averaged time period
		
		TCNT3 = 0; //reset the timer
		pulse_count = 0; //reset the pulse counter
		timed_wait = TRUE; //now we wait
		}

	}


}




////////////////////////////////////////////////////
/*! 
	Timer 3 Overflow 16mhz/1024 = 15625hz = 64usec tick, ~4 second rollover
*/
//////////////////////////////////////////////////////
SIGNAL (SIG_OVERFLOW3)
{
	//we rolled over, flush everything
	mph = 0;
	pulse_count = 0;
}





