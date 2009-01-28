/*! \file LCDash.h 
	\brief	The "main" header file for the LCDash system
*/

#ifndef _LCDASH_H_
#define _LCDASH_H_

#include "touchscreen.h"
#include "t6963.h"

#define LCDASH_VERSION		PSTR("LCDash-v2.1")
#define VDAF_VERSION		PSTR("VDAPFUL-V3.50")


/*------------------------------------*/
//LCDash supported protocols
#define PROTOCOL_NONE			0
#define PROTOCOL_MEGASQUIRT		1
#define PROTOCOL_OBD			2
#define PROTOCOL_MUT			3
/*-------------------------------*/




#define NOT_PRESSED 0 	/*!< The button was not pressed */
#define RELEASED	1	/*!< The button was released */
#define PRESSED		2	/*!< The button was pressed */

#define	BRIGHTNESS_SAVED	11	/*!< Brightness set in eeprom */

//extern const unsigned char plus_sign[] PROGMEM;
//extern const unsigned char minus_sign[] PROGMEM;

int main(); /*!< Our main loop. On embedded systems, we should never leave this. */



/////////////////////////////////////////
/*! 
	A Delays loop. 
	\param x The number of milliseconds, up to 255 milliseconds. 
*/
/////////////////////////////////////////
extern void delay_ms(unsigned char x); 



/////////////////////////////////////////
/*! 
	Global variable initialization here. 
	Also it restores previous settings, drawing of the background, gauges.. etc. 
*/
/////////////////////////////////////////
void LCDash_init();




/////////////////////////////////////////
/*! Goes to the main menu */
/////////////////////////////////////////
void do_main_menu();



////////////////////////////// //////////////////////////
/*! Checks to see if an 8-bit point was on or inside of a rectangle
    \param p An 8-bit point
    \param r The rectangle to check p against
*/
////////////////////////////////////////////////////////
char pointInRect(POINT p, LCD_RECT r);

void dualDisplay_send(unsigned char * buff,unsigned char len);
void dualDisplay_init();
void dualDisplay_off();
unsigned char dualDisplay_check();

void USB_selectZero();
void USB_selectOne();
unsigned char LCDash_descrProtocol();
void LCDash_setProtocol(unsigned char protocol);

void backlightRestore();
void backlightStore();
float backlightChange(char amount); //returns the percentage
#endif
