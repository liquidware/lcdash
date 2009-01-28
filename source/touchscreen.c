#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "touchscreen.h"
#include "t6963.h"
#include "bitops.h"
#include "calibrate.h"
#include "gui.h"
#include "menu.h"


volatile unsigned char press;
volatile unsigned char x_loc;
volatile unsigned char y_loc;
volatile unsigned char yy_loc, xx_loc;
volatile unsigned char x_loc_max, y_loc_max;
volatile unsigned int sample_count;

MATRIX	matrix;

extern EEMEM unsigned char ee_isCalibrated;
EEMEM MATRIX ee_matrix;


void touchscreen_process_x(unsigned char adc_value)
{

			x_loc = adc_value;	//save the ADC result
			if (x_loc>15)   //The natural idle value of the touch screens is about 2 for the x and 2 for the y 
				//when there as been a press greater than 15 it is on the region of the screen (assuming the y also has a press greater than 15)
				{
				sample_count++; //this keeps track of how many samples the processor took durring a press
				if (x_loc>x_loc_max) //here we look for the max x value durring a press (based on the max of all the sample counts)
					{ //save the new maximums
						x_loc_max = x_loc; //save the new maximum x
						y_loc_max = y_loc; //save the previous y
					}
					if (sample_count > 10000)//10,000 samples
					{
						eeprom_write_byte(&ee_isCalibrated, 23);
						lcd_clear_text();
						lcd_clear_graph();


						char msg[40];
						strncpy_P(msg,PSTR("Touchscreen has been reset"),40);
						gui_puts(msg,35,40,0);

						strncpy_P(msg,PSTR("Please reset the LCDash"),40);
						gui_puts(msg,45,50,0);
						while (1)
						{
						;
						}
					
					}


				}
			else// there wasn't a press because the screen was still in an idle state
				{
				//idle
				if (x_loc_max<=15)//not just pressed because x_loc_max would have a value of16 or greater from above
					{
					}
				else //this sample is the first sample after a touch took place 
					{
					if (sample_count > 30) //30makes sure the press is greater than 80 samples long (shorter press's are usually triggered by noise or other errors)
						{
							xx_loc = x_loc_max; //save the most recent touch screen x sample
							yy_loc = y_loc_max; //save the most recent touch screen y sample
							x_loc_max = 0;  //reinitialize the program to be ready to sample again
							y_loc_max = 0;  //reinitialize the program to be ready to sample again
							sample_count = 0;  //reinitialize the program to be ready to sample again
							press = yes;  //flag the pressed status from   no -> yes
						}
					}						
				}


}






void touchscreen_process_y(unsigned char adc_value)
{

	y_loc = ADCH;	//save the ADC result

}





void touch_init()  // initialize the touch algorithm
{

	// set all the variables to zero
	x_loc_max = 0;
	y_loc_max = 0;
	xx_loc = 0;
	yy_loc = 0;
	press = no;
	sample_count = 0;

	sei(); //enable global interupts	because this is an inturrupt based saple method




//////////////////////////////////////////////

	//Timer 1 (also used for backlight PWM) 
	//is used for timing ADC conversions
	TIMSK1 = (1 << OCIE1A); //enable interrupts on timer 1 overflow

	ADCSRA = (1<<ADEN) | (1<<ADIE) | 
			(1<<ADPS2) | (1<<ADPS1) |(1<<ADPS0); //clock freq fosc/128

	//---------------------Backlight and Button Brightness
//	SETBIT(DDRB,PB5);	
	//DDRB = 0xFF;

	//timer1 (32.7KHz freq)

	TCCR1A  = (1<<COM1A1 | 0<<COM1A0 | 1<<WGM10 | 0<<WGM11); // set OC1A pin on compare match, clear them at bottom
			    			  
							   // 8 bit PWM
		   
	TCCR1B = (0<<CS12   | 	1<<CS11   | 0<<CS10 //Prescaler
			| 0<<WGM13 | 1<<WGM12);                        //FAST PWM


	//note:
	//OCR1A controls the duty cycle 
	// of PB1, OCR1B for PB2 and OCR2 for PB3
	//8-bit PWM Gives 0-255 duty cycle

//	ICR1H=255; //This is 16-bit TOP
/////////////////////////////////////////////


//	setup_x();//setup the touch screen to sample in the x-direction

	if (eeprom_read_byte(&ee_isCalibrated) != TOUCHSCR_IS_CALIBRATED)// check to see if the interupt has been previously calibrated
	{
	touch_calibrate(); //calibrate the touch screen
	}
	else
	{
	eeprom_read_block (&matrix, &ee_matrix, sizeof(MATRIX)); // read the previous saved calibration values in from memory
	}

}//end touch_init




//initial sampling direction
void touchscreen_setup_y()	// set up a sample in the y-direction
{
	unsigned char x;
	
	/*           basically you 
					- ground the y plus pin
	                - set +5V to the y minus pin
					- ground the x plus pin
					- and read the voltage at the x minus pin     
	
	*/


		//swithing channels,
		//drive the x+ high, x- low
		SETBIT(DDRF,XPLUS_PIN);
		SETBIT(DDRF,XMINUS_PIN);

	//	CLRBIT(PORTF,XPLUS_PIN);
		
		SETBIT(PORTF,XPLUS_PIN);
		CLRBIT(PORTF,XMINUS_PIN);
		
		CLRBIT(PORTF,YPLUS_PIN);//grounding out screen
		
		for (x=0;x<25;x++)//delay before internal avr caoaciters can discharge
			{
				asm("nop");
			}
		
		//Y+ input
		CLRBIT(DDRF,YPLUS_PIN);
		//Y- floating
		CLRBIT(DDRF,YMINUS_PIN);
	ADMUX = YMINUS;	
		

	
} //end setup_y


void touchscreen_setup_x() //set up a sample in the x direction
{

unsigned char x;
	
	/*basically you 
					- ground the y plus pin
	                - set +5V to the y minus pin
					- ground the x plus pin
					- and read the voltage at the x minus pin     
	
	*/
	

		//swithing channels,
		//drive the y+ high, y- low
		SETBIT(DDRF,YPLUS_PIN);
		SETBIT(DDRF,YMINUS_PIN);

		SETBIT(PORTF,YPLUS_PIN);
		CLRBIT(PORTF,YMINUS_PIN);
		
	CLRBIT(PORTF,XPLUS_PIN);//grounding out touch screen
		
		for (x=0;x<25;x++)//delay before internal avr caoaciters can discharge
			{
				asm("nop");
			}
		
		//X+ input
		CLRBIT(DDRF,XPLUS_PIN);
		//X- floating
		CLRBIT(DDRF,XMINUS_PIN);
	ADMUX = XPLUS;	
 

}//end setup_x











/****************************************************/
/*                                                  */
/* Delay milisecond                                 */
/*                                                  */
/****************************************************/
//this is a really random place to put this function but we needed it for touch calibrate

void delay_ms(unsigned char x)
{
unsigned char w,y,g;

for (w=0; w<x ; w++)
{
	for (y=0; y<13; y++)
	{
		for (g=0; g<255; g++)
		{
		asm("nop");
		}
	}
}

}


/****************************************************/
/*                                                  */
/* Touch Calibrate                                  */
/*                                                  */
/****************************************************/

/* calibrating the touch screen compensates for 4 major issues 
     relating a touch location to a pixel location.
		1)  An offset in the x-aligenment
		2)  An offset in the y-aligenment
		3)  A rotational offset between the rows of pixels and the touch screen
		4)  Compensates the offset of each press location on the touch screen to a pixel location on the LCD screen
*/


char touch_calibrate(void)
{




//array of input points
POINT32	screenSample[3];





//array of expected correct answers
POINT32	displaySample[3] =  {
							{35,60},
							{113,20},
							{197,80}
								};


		/* An array of perfect input screen points used to obtain a first pass   */
        /*  calibration matrix good enough to collect calibration samples.       */

POINT32 perfectScreenSample[3] =	{
                                            { 100, 100 },
												{ 900, 500 },
                                            { 500, 900 }
                                } ;



		/* An array of perfect display points used to obtain a first pass        */
        /*  calibration matrix good enough to collect calibration samples.       */

POINT32 perfectDisplaySample[3] = {
                                            { 100, 100 },
												{ 900, 500 },
                                            { 500, 900 }
	                            } ;


//initial pass
setCalibrationMatrix( &perfectDisplaySample[0], 
						  &perfectScreenSample[0], 
						  &matrix ) ;


lcd_print(10,7,"Touch the circle to calibrate");
lcd_print(10,8,"the touch screen.");

//gui_puts("",0,120,0);//print the idle state of the touch screen 


lcd_circle(35,60,4,1);
press = no;
while(!press)
	{
	;
	}

//got a release
screenSample[0].x = xx_loc; 
screenSample[0].y = yy_loc;

lcd_circle(35,60,3,1);
delay_ms(20);
lcd_circle(35,60,2,1);
delay_ms(20);
lcd_circle(35,60,1,1);
delay_ms(60);
lcd_circle(35,60,4,0);
delay_ms(20);
lcd_circle(35,60,3,0);
delay_ms(20);
lcd_circle(35,60,2,0);
delay_ms(20);
lcd_circle(35,60,1,0);

delay_ms(250);





lcd_circle(113,20,4,1);
press = no;
while(!press)
	{
	;
	}

//got a release
screenSample[1].x = xx_loc; 
screenSample[1].y = yy_loc;

lcd_circle(113,20,3,1);
delay_ms(20);
lcd_circle(113,20,2,1);
delay_ms(20);
lcd_circle(113,20,1,1);
delay_ms(60);
lcd_circle(113,20,4,0);
delay_ms(20);
lcd_circle(113,20,3,0);
delay_ms(20);
lcd_circle(113,20,2,0);
delay_ms(20);
lcd_circle(113,20,1,0);

delay_ms(250);





lcd_circle(197,80,4,1);
press = no;
while(!press)
	{
	;
	}

//got a release
screenSample[2].x = xx_loc; 
screenSample[2].y = yy_loc;


lcd_circle(197,80,3,1);
delay_ms(20);
lcd_circle(197,80,2,1);
delay_ms(20);
lcd_circle(197,80,1,1);
delay_ms(60);
lcd_circle(197,80,4,0);
delay_ms(20);
lcd_circle(197,80,3,0);
delay_ms(20);
lcd_circle(197,80,2,0);
delay_ms(20);
lcd_circle(197,80,1,0);





lcd_print(5,15,"Calibrating touch screen");

//now, the real calibration pass
setCalibrationMatrix( &displaySample[0], &screenSample[0], &matrix ) ;

eeprom_write_byte(&ee_isCalibrated, TOUCHSCR_IS_CALIBRATED);
eeprom_write_block(&matrix, &ee_matrix, sizeof(MATRIX)); //MATRIX = 28 bytes
					
					delay_ms(250);
					delay_ms(250);
					delay_ms(250);
POINT clear_value;
touch_get_cursor(&clear_value);
lcd_clear_text();
press = no;

return 0;
}






/****************************************************/
/*                                                  */
/* Touch Get Cursor                                 */
/*                                                  */
/****************************************************/

//unsigned char x,y;
//if (touch_get_cursor(&x,&y)==1)
	//do something

// this relates a touch's max sample to a pixel location 

char touch_get_cursor(POINT* p)
{
	
if (press)
{

	POINT32 my_point, calibrated_point;
	my_point.x = (long)xx_loc;
	my_point.y = (long)yy_loc;

	//display.x, display.y is my result
	getDisplayPoint( &calibrated_point, &my_point, &matrix ) ;

	(p->x) = (unsigned char)calibrated_point.x;
	(p->y) = (unsigned char)calibrated_point.y;
			
	press = no;
	return 1;
}
else
	return 0;	

}//end touch_get_cursor

