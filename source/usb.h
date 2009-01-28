/*! 	\file usb.h
 		\brief The main USB driver for the LCDash system.
*/

#ifndef _USB_H_
#define _USB_H_

#define USB_DATA_PORT	PORTE	//!< Data port
#define USB_DATA_DDR	DDRE	//!< Data port DDR
#define USB_DATA_PINS	PINE	//!< Data port PIN 

#define USB_CTRL_PORT	PORTD	//!< The ctrl port
#define USB_CTRL_DDR	DDRD	//!< ddr
#define USB_CTRL_PIN	PIND	//!< pin

#define USB_RXF			PD0		//!< 0
#define USB_TXE			PD1		//!< 1
#define USB_WR			PD5		//!< 5
#define USB_RD			PD4		//!< 4
#define USB_DATAREQ		PD6		//!< 6
#define USB_DATAACK		PD7		//!< 7

#define USB_RESET		PB4		//!< 4

//#define USB_TIMEOUT		-1

#define USB_BUFF_SIZE	1024 /*!< This is the size of the USB buffer */

#define ALARM_SOUND	 	0	//!< An Alarm state
#define ALARM_SILENT 	1	//!< An Alarm state

#define DATAMODE		0	//!< a curr_mode option
#define COMMANDMODE		1	//!< a curr_mode option



//---Structures

////////////////////////////////////
/*!
	A structure for a USB device. 
	Contains information about the specific device. 
*/
////////////////////////////////////
typedef struct USB_dev{
				char dev_number; /*!< The device number 0-7 */
				unsigned char dev_type; /*!< The USB device class */
				unsigned char info[32]; /*!< Detailed info on the device */
				} USB_DEVICE;


//-----end of structures
//
//
//
//
//
//
//
//---Function Definitions


//////////////////////////////////////////////////////////////////////
//! A Read function with a timeout feature.
/*! 
	This function reads from the USB chip. If the function times out,
	the return value will be the number of characters correctly read.

	\param amount The desired amount of characters to be read
	\param timeout_cnt The max time time before 
				 this function returns
*/
//////////////////////////////////////////////////////////////////////
unsigned char USB_read(unsigned int amount,unsigned int timeout_cnt);



////////////////////////////////////////////////
//! Sends one character to the USB chip
/*! 
	\param c the chacter to transmit
*/
///////////////////////////////////////////////
void USB_putc(unsigned char c);



////////////////////////////////////////////////
//! Sends a 0x00 terminated string out to the USB chip
/*! 
	\param s the terminated string to transmit
*/
///////////////////////////////////////////////
void USB_puts(char* s);



////////////////////////////////////////////////
//! Initialize the USB chip.
/*! 
	Performs a USB chip reset and flushes the chip, putting the 
	chip into a 'known' communications state
*/
///////////////////////////////////////////////
void USB_init();



////////////////////////////////////////////////
//! Sends a string of characters to the USB chip
/*! 
	\param s The string.
	\param len The length of the string to send
*/
///////////////////////////////////////////////
void USB_send(unsigned char * s, unsigned int len);



////////////////////////////////////////////////////////////////
/*! Flushes the messages stored on the VNC1L
	Such as messages for when devices are connected/removed
*/
///////////////////////////////////////////////////////////////
void USB_flushChip();



////////////////////////////////////////////////////////////////
/*! This function returns a response describing the 
	type of device attached to the USB port.
	
	\param the_port 	This is the physical port on the system. 
						Its defined as either PORT1 or PORT2
	
	\param response 	Expect the response to be 13 characters 
						Make sure you pass me enough space

*/
///////////////////////////////////////////////////////////////
char USB_queryPort(char the_port,char* response);



////////////////////////////////////////////
/*!
	This function queries the USB device number and requests information about it.
	If successful, it returns 1 else 0.

	\param dev_number 	This is an ascii character number '0' through '7'. 
	\param response		Character array for storage of the reponse. The response is usually 128 bytes.
*/
////////////////////////////////////////////
char USB_explore(char dev_number, char * response);



///////////////////////////////////////////////////////////////
/*! Sets the current USB device, so we can directly talk to it in data mode.
	Usually call this before changing into data mode.

	\param dev_number  The USB device number (not the port number!) on the system
					The device number should be between 0-7 
*/
///////////////////////////////////////////////////////////////
char USB_selectDevice(char dev_number);



///////////////////////////////////////////////////////////////
/*! Sets the mode of talking to the VNC1L
	\param the_mode This is the mode we're changing to.
					Valid options are either: COMMANDMODE or DATAMODE
*/
///////////////////////////////////////////////////////////////
char USB_setMode(char the_mode);



unsigned char USB_getBuffSize();
unsigned char* USB_getBuff();

char USB_commandMode(); /*!< Put the VNC1L into COMMANDMODE */
char USB_dataMode();	/*!< Put the VNC1L into DATAMODE */
char USB_getMode();		/*!< Get the current mode of the VNC1L */

void USB_setAlarm(unsigned int timeout_amt);
char USB_checkAlarm();












#endif
