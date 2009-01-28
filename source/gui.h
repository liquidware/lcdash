/*! \file gui.h 
	\brief	Graphical User Interface drawing routines 
*/
#ifndef _GUI_H_
#define _GUI_H_


//////////////////////////////////////////////////////
/*! This functions draws the entire gauge layout background */
//////////////////////////////////////////////////////
void gui_drawBackground();



/////////////////////////////////////////////////////
/*! This function draws a large number chacter on the screen
	\param ch the chacter to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_XlargeNumC(char ch,unsigned char x_pos,unsigned char y_pos,unsigned char invert);



/////////////////////////////////////////////////////
/*! This function draws a large number string on the screen
	\param string The string to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_XlargeNumS(char *string,unsigned int x_pos,unsigned int y_pos,unsigned char invert);




/////////////////////////////////////////////////////
/*! This function draws a large number chacter on the screen
	\param ch the chacter to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_largeNumC(char ch,unsigned char x_pos,unsigned char y_pos,unsigned char invert);



/////////////////////////////////////////////////////
/*! This function draws a large number string on the screen
	\param string The string to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_largeNumS(char *string,unsigned int x_pos,unsigned int y_pos,unsigned char invert);




/////////////////////////////////////////////////////
/*! This function draws a null-terminated string of chacters on the screen
	\param string The string to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_puts(char *string,unsigned char x_pos,unsigned char y_pos,unsigned char invert);



/////////////////////////////////////////////////////
/*! This function draws a character on the screen
	\param ch the chacter to draw
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
//////////////////////////////////////////////////////
void gui_putc(unsigned char ch, unsigned char x_pos,unsigned char y_pos,unsigned char invert);



/////////////////////////////////////////////////////////////
/*! This function draw a bitmap from a Linux X-Bitmap format

 	\param img The is the image stored in program memory
	\param x_loc The x coordinate of the bitmap
	\param y_loc The y coordinate of the bitmap
	\param width The width of the bitmap
	\param height The heigh of the bitmap
	\param invert O = Don't invert, 1 = Invert colors
*/	
///////////////////////////////////////////////////////////////
void gui_drawBMP(const unsigned char *img, unsigned char x_loc,unsigned char y_loc, 
										unsigned char width, unsigned char height,unsigned char invert);

//////////////////////////////////////////////////////////////
/*!
	This puts a string from program memory out to the screen
	\param string The string to draw (from program memory)
	\param x_pos The left coordinate of the character
	\param y_pos The top coordinate of the character
	\param invert O = Don't invert, 1 = Invert colors 
*/
///////////////////////////////////////////////////////////////////
void gui_puts_P(char *string,unsigned char x_pos,unsigned char y_pos,unsigned char invert);


#endif
