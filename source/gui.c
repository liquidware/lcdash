
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <stdlib.h>

#include "t6963.h"
#include "gui.h"
#include "Font.h"
#include "bitops.h"


void gui_drawBackground()
{
//lets repaint all of the background images

//	gui_puts("Hello LCDash world!",0,0,0); //paint regular
//	gui_puts("Hello LCDash world!",0,9,1); //paint inverted
	
//	gui_largeNumS("0123456789 -",0,60,0); //paint normal
//	gui_largeNumS("0123456789 -",0,60,1); //paint inverted

}



///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_XlargeNumS(char *string,unsigned int x_pos,unsigned int y_pos,unsigned char invert)
{

	while(*string)
		{
		gui_XlargeNumC(*string++,x_pos,y_pos,invert);
		x_pos+=48;
		}
}



///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_XlargeNumC(char ch,unsigned char x_pos,unsigned char y_pos,unsigned char invert)
{

unsigned int num_index;

if (ch==46) 
	num_index = 10*492; //period
else if (ch==32) 
	num_index = 11*492; //space
else if (ch=='-') 
	num_index = 12*492; //negative sign
else 
	num_index = (ch-48)*492;

gui_drawBMP(xlarge_nums+num_index,x_pos,y_pos,48,82,invert);

}





///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_largeNumS(char *string,unsigned int x_pos,unsigned int y_pos,unsigned char invert)
{

	while(*string)
		{
		gui_largeNumC(*string++,x_pos,y_pos,invert);
		x_pos+=16;
		}
}





///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_largeNumC(char ch,unsigned char x_pos,unsigned char y_pos,unsigned char invert)
{

unsigned int num_index;

if (ch==46) 
	num_index = 10*46; //period
else if (ch==32) 
	num_index = 11*46; //space
else if (ch=='-') 
	num_index = 12*46; //negative sign
else 
	num_index = (ch-48)*46;

gui_drawBMP(large_nums+num_index,x_pos,y_pos,16,23,invert);

}


///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_drawBMP(const unsigned char *img, unsigned char x_loc,unsigned char y_loc, 
								unsigned char width, unsigned char height,unsigned char invert)
{


unsigned char x_pos = x_loc;
unsigned char y_pos = y_loc;
unsigned char i;

/*! If the bitmap has an even number of bytes
	and is placed on a beginning address on the lcd screen
	Then draw one byte at a time insted of one bit at a time
	This process draws a bitmap about 4 times faster than drawing one bit at a time
*/
if ((width%8 == 0) && (x_pos%8 == 0))// x_pos = left
	{
	unsigned char lcd_curr_byte;
	unsigned char pattern, y;
	unsigned char width_bytes = width/8;
	unsigned char line[width_bytes];

		for (y = y_pos; y<(y_pos+height); y++)
		{
		
		lcd_setGraphByteAddr(x_pos,y);
		

		//loading line------------------
			for(i=0;i<width_bytes;i++)

				line[i] = pgm_read_byte(img++);//img = bmp
		//----end

		//middle bytes---------------------------------
			for(lcd_curr_byte=0; lcd_curr_byte < width_bytes; lcd_curr_byte++)   
			{
				pattern = line[lcd_curr_byte];
				
				if (invert == 1)//if invert is one then invert the colors of the bmp
					pattern = pattern^0xff;
									

				//flipbits of pattern
	    		//reverse bits in a byte
	    		pattern = (pattern << 4) | (pattern >> 4);
	    		pattern = ((pattern & 0b00110011) << 2) | ((pattern & 0b11001100) >> 2);
	    		pattern = ((pattern & 0b01010101) << 1) | ((pattern & 0b10101010) >> 1);
				//the above process flipped the nibbles				

				lcd_writeByte(pattern);
			}
		}
	}


else
	{
	/*!
	draw the bitmap one pixel at a time
	*/
	unsigned char on_off;
	unsigned int row;
	unsigned char byte;
	unsigned int bytecnt;

	if (width<8) bytecnt = (8*height)/8;
	else bytecnt = (width*height)/8;

	for (row=0;row<bytecnt;row++)
	{

		byte = pgm_read_byte(img+row);
		if (!invert)
			byte ^= 0xFF; //flip the bits, why?
		unsigned char shifter = 0b00000001;

		for (i=0;i<8;i++)
		{
			on_off = byte & shifter;
			if (!invert)
				on_off ^=1;

			if(x_pos <= x_loc+width) 
			 	lcd_pixel(x_pos,y_pos,on_off^invert);

			shifter = shifter << 1;
			x_pos=x_pos+1;
		}
	
		if (x_pos > (x_loc+ (width-1)) )
		{
			y_pos=y_pos+1;
			x_pos=x_loc;
		}//end if
	
	} //end for(row)

}
}



///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_puts(char *string,unsigned char x_pos,unsigned char y_pos,unsigned char invert)
{

while(*string)
	{
	gui_putc(*string++,x_pos,y_pos,invert);
	x_pos=x_pos+6;
	}
}




///////////////////////////////////////////////////////////////////////
// print one char in grapic mode
//
/////////////////////////////////////////////////////////////////////
void gui_putc(unsigned char ch, unsigned char x_pos,unsigned char y_pos,unsigned char invert)
{
unsigned int x,shifted,on_off,letter;
unsigned int row,byte;

	letter=ch-(32);
	letter=(letter*7);

	for (row=0;row<7;row++)
	{
		byte = pgm_read_byte(font_5x7+letter+row);

		shifted = byte;

		for (x=0;x<5;x++)
		{
			on_off = shifted & 0b10000000;
			on_off = on_off>>7; 
			shifted = shifted << 1;
			x_pos=x_pos+1;
			lcd_pixel(x_pos,y_pos,(on_off^invert));
		}
		y_pos=y_pos+1;
		x_pos=x_pos-5;
	}

}



///////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////
void gui_puts_P(char *string,unsigned char x_pos,unsigned char y_pos,unsigned char invert)
{

unsigned char len = strlen_P(string);
char 	out[len];
strcpy_P(out,string);

gui_puts(out,x_pos,y_pos,invert);


}
