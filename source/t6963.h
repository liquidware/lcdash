/*! \file t6963.h 
	\brief	The t6963 graphics driver.
*/
#ifndef _T6963_H_
#define _T6963_H_


#define LCD_DATA_PORT	PORTC 	/*!< lcd data out port */
#define LCD_DATA_DDR	DDRC 	/*!< LCD data direction reg */
#define LCD_DATA_PINS	PINC 	/*!< The pins used for input */

#define LCD_CTRL_PORT 	PORTA 	/*!< LCD control port */
#define LCD_CTRL_DDR	DDRA 	/*!< lcd control direction reg */

#define LCD_C_OR_D PA1 /*!< 	C/D  */    
#define LCD_RESET  PA0 /*!< 	/Reset  */ 
#define LCD_CE	   PA2 /*!<   /Chip enable */
#define LCD_RD     PA3 /*!<  /read */
#define LCD_WR	   PA4 /*!<  /write */

#define LCD_FS		PA5 /*!< font select */
#define LCD_RV		PA6 /*!< Negative image? */


#define XMAX 240        /*!< x max limit of LCD graphics drawing */
#define XMIN 0			/*!< x min limit of LCD graphics drawing */
#define YMAX 128		/*!< y max limit of LCD graphics drawing */		
#define YMIN 0			/*!< y min limit of LCD graphics drawing */
#define FONT_WIDTH 8    /*!< Set to match your font hardware choice */


/* ----- Definitions concerning LCD internal memory  ------ */

#define G_BASE 0x0280            /*!< base address of graphics memory */
#define T_BASE 0x0000            /*!< base address of text memory */
#define BYTES_PER_ROW XMAX/FONT_WIDTH         /*!< how many bytes per row on screen */

/* ----------------------------------------------------------- */


//----Structure Definitions


typedef struct Lcd_rect	
{
	unsigned char left; 	/*!< The left side of the rectangle */
	unsigned char top; 		/*!< The top position of the rectangle */
	unsigned char right; 	/*!< The right side of the rectangle */
	unsigned char bottom; 	/*!< The bottom position of the rectangle */
}LCD_RECT;	



//----Function Definitions

void lcd_init(void); 						/*!< Init the LCD screen */

void lcd_set_address(unsigned int addr); 	/*!< Set the LCD screen address */

void lcd_command(unsigned char data); 		/*!< The LCD command function */

void lcd_data(unsigned char data);			/*!< The LCD data function */

void lcd_clear_text();  		 			/*!< Clears the text memory of LCD */

void lcd_xy(unsigned int x,unsigned int y); 		 /*!< Sets memory pointer to (x,y) position (text mode) */

void lcd_print(unsigned int x,unsigned int y,char *string);  /*!< Sends string of characters to LCD (text mode) */

void lcd_setGraphByteAddr(unsigned char x_pixel, unsigned char y_pixel); /*!< Sets the LCD address based on pixels */

void lcd_writeByte(unsigned char pattern); /*!< Writes a byte to the LCD screen, autoincrements the LCD address */

void lcd_clear_graph(); /*!< This will clear the graphics on the screen */



////////////////////////////////////////////////////////////////////
/*!Draw a rectangle at a larger or smaller size from the existing
	\param r THe rectangle we're basing our size from
 	\param size_difference  The change in size: +1, +2, -2, -3,... etc 
	\param show 0 = Erase, 1 = Draw
*/
////////////////////////////////////////////////////////////////////
void lcd_rect_diffSize(LCD_RECT r, char size_difference, unsigned char show);



////////////////////////////////////////////////////////////////////
/*! This will draw a pixel at location
	\param x_pos X coordinate
	\param y_pos Y coordinate
	\param color 0 = off, 1 = On
*/
////////////////////////////////////////////////////////////////////
void lcd_pixel(unsigned char x_pos, unsigned char y_pos,unsigned char color);




////////////////////////////////////////////////////////////////////
/*! Draws a line 
	\param x1 Starting x
	\param y1 Starting y
	\param x2 Ending x
	\param y2 Ending y
	\param show 0 = Erase, 1 = Draw
*/
////////////////////////////////////////////////////////////////////
void lcd_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char show);




////////////////////////////////////////////////////////////////////
/*! Draws a Circle
	\param x X coordinate of center 
	\param y Y coordinate of center
	\param radius the radius of the circle
	\param show 0 = Erase, 1 = Draw
*/
////////////////////////////////////////////////////////////////////
void lcd_circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char show);



////////////////////////////////////////////////////////////////////
/*! Draws only half of a Circle
	\param x X coordinate of center 
	\param y Y coordinate of center
	\param radius the radius of the circle
	\param show 0 = Erase, 1 = Draw
*/
////////////////////////////////////////////////////////////////////
void lcd_circle_half(unsigned char x, unsigned char y, unsigned char radius, unsigned char show);



////////////////////////////////////////////////////////////////////
/*! Draws the outline of a rectangle
	\param r The rectangle outline to draw
	\param show 0 = Erase, 1 = Draw
*/
///////////////////////////////////////////////////////////////////
void lcd_rect(LCD_RECT r,unsigned char show);



///////////////////////////////////////////////////////////////////
/*! This will efficiently fill a rectangle with a pattern
	\param r This is the rectangle
	\param pattern This is the 8-bit pattern to apply to the rectangle
*/
///////////////////////////////////////////////////////////////////
void lcd_fill_rect(LCD_RECT r,unsigned char pattern);



///////////////////////////////////////////////////////////////////
/*! Draws and ellipse
	\param xc X coordinate of the center
	\param yc Y coordinate of the center
	\param a0 The width
	\param b0 The height
	\param show 0 = Erase, 1 = Draw
*/
///////////////////////////////////////////////////////////////////
void lcd_ellipse(unsigned char xc,unsigned char yc,unsigned char a0,unsigned char b0,unsigned char show);




///////////////////////////////////////////////////////////////////
/*! A graphics support function */
///////////////////////////////////////////////////////////////////
void lcd_4pixels(unsigned char  x,unsigned char  y,unsigned char  xc,unsigned char  yc,unsigned char  show);



void lcd_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);


void lcd_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
#endif
