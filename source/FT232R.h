/*! \file FT232R.h
	\brief	An Driver for FTDIchip.com's FT232R based devices
*/

#ifndef _FT232R_H_
#define _FT232R_H_

//FT232R Configurable Baud Rates	
#define B300		"102700\r" //!< A baud rate of 300
#define B600		"881300\r" //!< A baud rate of 600	
#define B1200		"C40900\r" //!< A baud rate of 1200
#define B2400		"E20400\r" //!< A baud rate of 2400
#define B4800		"710200\r" //!< A baud rate of 4800
#define B9600		"384100\r" //!< A baud rate of 9600
#define B19200		"938000\r" //!< A baud rate of 19200
#define B38400		"4EC000\r" //!< A baud rate of 38400
#define B57600		"34C000\r" //!< A baud rate of 56700
 
#define B115200		"1A0000\r" //!< A baud rate of 115200
#define B230400		"0D0000\r" //!< A baud rate of 230400
#define B460800		"064000\r" //!< A baud rate of 460800
#define B921600		"038000\r" //!< A baud rate of 921600

#define B1000000	"030000\r" //!< A baud rate of 1 Meg
#define B1500000	"020000\r" //!< A baud rate of 1.5 Meg
#define B2000000	"010000\r" //!< A baud rate of 2 Meg
#define B3000000	"000000\r" //!< A baud rate of 3 Meg



////////////////////////////////////////////////////////////////////
/*!
	Set the baud rate of the FT232R Device
	\param baud This is the string containing the baud rate. 
				The definitions for this parameter are in this file.
*/
////////////////////////////////////////////////////////////////////
char FT232R_setBaud(char * baud);



////////////////////////////////////////////////////////////////////
/*!
	Reads a specified amount of characters from a FT232R Device
	\param amount The amount of characters to be read
	\param timeout_cnt The timeout time in milliseconds
*/
////////////////////////////////////////////////////////////////////
char FT232R_read(unsigned char amount,unsigned int timeout_cnt);

////////////////////////////////////////////////////////////////////
/*!
	Send an array of Bytes through an FTDI based device
	\param msg 		The array of characters to be sent
	\param length	The length of the msg

*/
////////////////////////////////////////////////////////////////////
char FT232R_send(unsigned char * msg,unsigned char length);



////////////////////////////////////////////////////////////////////
/*!
	Send a null-terminated string through an FTDI based device
	\param msg 		The string of characters to be sent

*/
////////////////////////////////////////////////////////////////////
char FT232R_puts(char * msg);


#endif
