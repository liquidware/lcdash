#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <string.h>

#include "messageBar.h"
#include "t6963.h"
#include "touchscreen.h"
#include "bitops.h"
#include "gui.h"


LCD_RECT	msg_bar; 	//!< The message area rectangle
char msg[MAX_MSG_SIZE+1]; //!< The current Mesage



void messageBar_addMsg(char * the_msg)
{

	strncpy(msg,the_msg,MAX_MSG_SIZE+1);
	msg[MAX_MSG_SIZE] = 0x00; //put a terminating char, just in case
	messageBar_refresh();
	
	
}



void messageBar_refresh()
{
	
	unsigned char center;
	
	center = (strnlen(msg,MAX_MSG_SIZE)*6)/2;
	
	lcd_fill_rect(msg_bar,0xFF); //clear the background
	gui_puts(msg,119-center,120,1); //display the centered message

}


void messageBar_clear()
{
	lcd_fill_rect(msg_bar,0x00); //clear the background
}


void messageBar_init()
{

	//the message bar area
	msg_bar.left =0;
	msg_bar.right = 239;
	msg_bar.top= 119;
	msg_bar.bottom = 127;

}
