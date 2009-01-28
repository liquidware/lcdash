#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>

#include "FT232R.h"
#include "usb.h"



char FT232R_setBaud(char * baud)
{
	if (USB_getMode() != COMMANDMODE)
		if ( !USB_commandMode() )
			return 0;

	char out[13] = "fbd $";


	strncat(out,baud,7);

	out[12]=0x00;

	USB_puts(out);
	USB_read(5,200);

	USB_dataMode(); //go back into dataMode

	return 1;

}



char FT232R_read(unsigned char amount,unsigned int timeout_cnt)
{
	if (USB_getMode() != DATAMODE)
		if ( !USB_setMode(DATAMODE) ) //switch to data mode
			return 0;  //dataMode failed

	return USB_read(amount,timeout_cnt);
}


char FT232R_puts(char * msg)
{
	if (USB_getMode() != DATAMODE)
		if ( !USB_setMode(DATAMODE) ) //switch to data mode
			return 0;  //dataMode failed
	
	USB_puts(msg); //send the string

return 1; //success
}


char FT232R_send(unsigned char * msg,unsigned char length)
{
	if (USB_getMode() != DATAMODE)
		if ( !USB_setMode(DATAMODE) ) //switch to data mode
			return 0;  //dataMode failed
	
	USB_send(msg,length); //send the msg

return 1; //success
}
