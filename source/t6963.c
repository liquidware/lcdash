#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "t6963.h"
#include "bitops.h"




void lcd_rect_diffSize(LCD_RECT r, char size_difference, unsigned char show)
{
	LCD_RECT	draw_r;
	
	//make the different size
	draw_r.left = r.left - size_difference;
	draw_r.right = r.right + size_difference;
	draw_r.top = r.top - size_difference;
	draw_r.bottom = r.bottom + size_difference;
	
	lcd_rect(draw_r,show); //draw the rectangle 
}


void lcd_setGraphByteAddr(unsigned char x_pixel, unsigned char y_pixel)
{
	unsigned int addr;


	addr =  G_BASE + (y_pixel*BYTES_PER_ROW)  + (x_pixel/FONT_WIDTH);
	lcd_set_address(addr);   // set LCD addr. pointer

}


void lcd_writeByte(unsigned char pattern)
{

	lcd_data(pattern);
	lcd_command(0xC0); //autoincrements the address

}


// ---------------------------------------------------------------------------------------------------------------------------------------------
//	Draw rectangle
// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_rect(LCD_RECT r,unsigned char show)
{
lcd_line(r.left, r.top, r.right, r.top, show);
lcd_line(r.right, r.top, r.right, r.bottom, show);
lcd_line(r.right, r.bottom, r.left, r.bottom, show);
lcd_line(r.left, r.bottom, r.left, r.top, show);
}


// ---------------------------------------------------------------------------------------------------------------------------------------------
//	Fill Inside Rectangle
// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_fill_rect(LCD_RECT r,unsigned char pattern)
{
unsigned char byte_width 	= (r.right-r.left) / 8;

unsigned char curr_xPos;
unsigned char curr_yPos;

unsigned char addr_isSet	= FALSE;


//doing y
for (curr_yPos=r.top; curr_yPos<=r.bottom; curr_yPos++)
	{

	curr_xPos  = r.left; //restart x to the left side
	addr_isSet = FALSE; //need to set the address again for next line

	//doing x
	while (curr_xPos <= r.right)
		{
		

		if ( (byte_width > 0) && ((curr_xPos % 8) ==0 ) && ( (r.right-curr_xPos) >=8 ) ) 
			{

			//We're here if we're on a byte (ex. 8,16,32)
			// and have 8 or more pixels still to write
			if (addr_isSet==FALSE)
				{
				//only set the graphics address once on the current line
				//	because the lcd controller auto increments the address
				lcd_setGraphByteAddr(curr_xPos,curr_yPos);
				addr_isSet = TRUE;
				}
			lcd_writeByte(pattern); //write the pattern
			curr_xPos+=8;
			}
		else
			{

			//We're here because we can only write pixels
			
			
			//--------------rotate our pattern
			  if (pattern & 0x80)
  				{
    			pattern = pattern << 1;
    			pattern = pattern | 0x01;
				lcd_pixel(curr_xPos,curr_yPos, 1); //set the pixel
  				}
  			else
  				{
    			pattern = pattern << 1;
				lcd_pixel(curr_xPos,curr_yPos, 0); //clear the pixel
  				} 
			//-------------end rotate

			curr_xPos++; //increment to the next pixel

			} 

		} //end of the line, doing next line

	}



} //end lcd_fillRect




/***********************************************************************
Draws a line from x1,y1 go x2,y2. Line can be drawn in any direction.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_line(unsigned char  x1,unsigned char  y1, unsigned char  x2, unsigned char  y2, unsigned char show) 
{
  int dy ;
  int dx ;
  int stepx, stepy, fraction;
  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0) 
  {
    dy = -dy;
    stepy = -1;
  }
  else 
  {
    stepy = 1;
  }
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }
  dy <<= 1;
  dx <<= 1;
  lcd_pixel(x1,y1,show);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1); 
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;  
      lcd_pixel(x1,y1,show);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      lcd_pixel(x1,y1,show);
    }
  }
}


/***********************************************************************
Draws a circle with center at x,y with given radius.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_circle(unsigned char  x, unsigned char  y, unsigned char  radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
    lcd_pixel(x + xc, y + yc, show);
    lcd_pixel(x + xc, y - yc, show);
    lcd_pixel(x - xc, y + yc, show);
    lcd_pixel(x - xc, y - yc, show);
    lcd_pixel(x + yc, y + xc, show);
    lcd_pixel(x + yc, y - xc, show);
    lcd_pixel(x - yc, y + xc, show);
    lcd_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}



// draw an ellipse
// Bresenham's ellipse algorithm
void lcd_ellipse(unsigned char xc,unsigned char yc,unsigned char a0,unsigned char b0,unsigned char show)
{
	int x,y;
	float a,b;
	float asqr,twoasqr;
	float bsqr,twobsqr;
	float d,dx,dy;

	x=0;
	y=b0;
	a=a0;
	b=b0;
	asqr=a*a;
	twoasqr=2*asqr;
	bsqr=b*b;
	twobsqr=2*bsqr;

	d=bsqr-asqr*b+asqr/4;
	dx=0;
	dy=twoasqr*b;
	while(dx<dy){
		lcd_4pixels(x,y,xc,yc,show);
		if(d>0){
			y--;
			dy-=twoasqr;
			d-=dy;
		}
		x++;
		dx+=twobsqr;
		d=d+bsqr+dx;
	}
	d=d+(3*(asqr-bsqr)/2-(dx+dy))/2;
	while(y>=0){
		lcd_4pixels(x,y,xc,yc,show);
		if(d<0){
			x++;
			dx+=twobsqr;
			d+=dx;
		}
		y--;
		dy-=twoasqr;
		d=d+asqr-dy;
	}
}


// graphics support function
void lcd_4pixels(unsigned char  x,unsigned char  y,unsigned char  xc,unsigned char  yc,unsigned char  show)
{
	lcd_pixel(xc+x,yc+y,show);
	lcd_pixel(xc-x,yc+y,show);
	lcd_pixel(xc+x,yc-y,show);
	lcd_pixel(xc-x,yc-y,show);
}

 
void lcd_circle_half(unsigned char  x, unsigned char  y, unsigned char radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
//    lcd_pixel(x + xc, y + yc, show);
    lcd_pixel(x + xc, y - yc, show);
//    lcd_pixel(x - xc, y + yc, show);
    lcd_pixel(x - xc, y - yc, show);
//    lcd_pixel(x + yc, y + xc, show);
    lcd_pixel(x + yc, y - xc, show);
//    lcd_pixel(x - yc, y + xc, show);
    lcd_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_pixel(unsigned char x_pos, unsigned char y_pos, unsigned char color)  
{



int addr;       // memory address of byte containing pixel to write
  if( (x_pos>=XMAX) || (y_pos>=YMAX) )return;
  addr =  G_BASE + (y_pos*BYTES_PER_ROW)  + (x_pos/FONT_WIDTH);
  lcd_set_address(addr);  // set LCD addr. pointer

if (color==1)
  lcd_command(0xf8 | (( (FONT_WIDTH-1)-x_pos%FONT_WIDTH)) );  // set bit-within-byte command
else
  lcd_command(0xf0 | (( (FONT_WIDTH-1)-x_pos%FONT_WIDTH)) );  // clear bit-within-byte command
}






//----------------------------------------------------------------
void lcd_clear_text()
{
 unsigned int i;   
 lcd_set_address(T_BASE);     // addrptr at address T_BASE

 for (i=0;i<( (XMAX/FONT_WIDTH)*(YMAX/8) );i++) {  //for every posible character  
      lcd_data(0); lcd_command(0xc0);               // write data, inc ptr.
 } // end for(i)

} // lcd_clear_text()


// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_clear_graph()    // clear graphics memory of LCD
{
unsigned int i;
lcd_set_address(G_BASE);     // addrptr at address G_BASE
for (i=0;i<( (XMAX*YMAX)/FONT_WIDTH );i++) {   
	lcd_data(0x00); lcd_command(0xc0);               // write data, inc ptr.
	}	 // end for(i)
} // end lcd_clear_graph()

// ---------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void lcd_print(unsigned int x,unsigned int y,char *string)  // send string of characters to LCD
{
unsigned int i;
unsigned int c;
	lcd_xy(x,y);
  	for (i=0;string[i]!=0;i++) 
  	{
		c = string[i] - 0x20;     // convert ASCII to LCD char address
		if (c<0) c=0;
			lcd_data(c);
		lcd_command(0xc0);               // write character, increment memory ptr.
  	} // end for

} // end lcd_string


// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_xy(unsigned int x,unsigned int y)  // set memory pointer to (x,y) position (text)
{
unsigned int addr;

  addr = T_BASE + (y * BYTES_PER_ROW) + x;
  lcd_set_address(addr);   // set LCD addr. pointer

} // lcd_xy()


// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_set_address(unsigned int addr)
{

	lcd_data(addr%256);
	lcd_data(addr>>8);
	lcd_command(0x24);
} //end lcd_set_address


// ---------------------------------------------------------------------------------------------------------------------------------------------
unsigned char lcd_status(){

unsigned char s;

///check the status of the LCD Screen
SETBIT(LCD_CTRL_PORT,LCD_CE);
LCD_DATA_PORT = 0x00;
LCD_DATA_DDR = 0x00; //set as data input

SETBIT(LCD_CTRL_PORT,LCD_C_OR_D);
SETBIT(LCD_CTRL_PORT,LCD_WR);
CLRBIT(LCD_CTRL_PORT,LCD_CE);
CLRBIT(LCD_CTRL_PORT,LCD_RD);


//need to wait at least for 150nS
asm("nop"); //~63nS @ 16Mhz
asm("nop"); //~63nS @ 16Mhz
asm("nop"); //~63nS @ 16Mhz

s = (LCD_DATA_PINS & 0x03); //get the status

SETBIT(LCD_CTRL_PORT,LCD_RD);
SETBIT(LCD_CTRL_PORT,LCD_CE);

LCD_DATA_DDR = 0xFF; //set back to data output

return s;
}


// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_command(unsigned char data)
{

while( lcd_status() !=0x03) { }; //wait until lcd is ready

LCD_DATA_PORT = data; //put the data out there
				
SETBIT(LCD_CTRL_PORT, LCD_C_OR_D); 				//command mode
//these next two instructions could be combined
CLRBIT(LCD_CTRL_PORT, LCD_CE); 				// CE on
CLRBIT(LCD_CTRL_PORT, LCD_WR); 				//assert Write


//!----need to hold data for min of 80nS -time to exec instructions takes care of this

SETBIT(LCD_CTRL_PORT, LCD_WR); 				// write mode inactive
SETBIT(LCD_CTRL_PORT, LCD_CE); 				// CE off


//data hold time (40ns) -just time make sure another function doesn't overrun this

} //end lcd_command



// ---------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_data(unsigned char data)
{


while( lcd_status() !=0x03) { }; //wait until lcd is ready
				
CLRBIT(LCD_CTRL_PORT, LCD_C_OR_D); 				//data mode
//these next two instructions could be combined
CLRBIT(LCD_CTRL_PORT, LCD_CE); 				// CE on
CLRBIT(LCD_CTRL_PORT, LCD_WR); 				//assert Write

LCD_DATA_PORT = data; //put the data out there
//!----need to hold data for min of 80nS -time to exec instructions takes care of this

SETBIT(LCD_CTRL_PORT, LCD_WR); 				// write mode inactive
SETBIT(LCD_CTRL_PORT, LCD_CE); 				// CE off


} //end lcd_data

// ---------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{
//	Init LCD display port	

	lcd_clear_graph();
	lcd_clear_text();
	
//----Set the initial data direction of all the used pins
	LCD_DATA_DDR = 0xff; // set datadir. as output
	
	SETBIT(LCD_CTRL_DDR,LCD_CE); //set the data direction of these control signals to output
	SETBIT(LCD_CTRL_DDR,LCD_RD); //set the data direction of these control signals to output
	SETBIT(LCD_CTRL_DDR,LCD_WR); //set the data direction of these control signals to output
	SETBIT(LCD_CTRL_DDR,LCD_C_OR_D); //set the data direction of these control signals to output
	SETBIT(LCD_CTRL_DDR,LCD_RESET); //set the data direction of these control signals to output
	SETBIT(LCD_CTRL_DDR,LCD_FS);
	SETBIT(LCD_CTRL_DDR,LCD_RV);	
//-------end set DDR


//Initial State of all pins
	SETBIT(LCD_CTRL_PORT,LCD_CE); //diable chip
	SETBIT(LCD_CTRL_PORT,LCD_RD); //disable read mode
	SETBIT(LCD_CTRL_PORT,LCD_WR); //disable write mode
	SETBIT(LCD_CTRL_PORT,LCD_C_OR_D); //command/status mode
	SETBIT(LCD_CTRL_PORT,LCD_RESET); //disable reset
	CLRBIT(LCD_CTRL_PORT,LCD_RV);
	CLRBIT(LCD_CTRL_PORT,LCD_FS);
//----End set initial state

//--------------------Reset the LCD screen
	CLRBIT(LCD_CTRL_PORT, LCD_RESET); 				//Reset on
	asm("nop");
	asm("nop");
	asm("nop");
	SETBIT(LCD_CTRL_PORT, LCD_RESET); 				//Reset off
//--------------------End reset
	
	
	lcd_command(0x80); 		// mode set: Graphics OR Text, ROM CGen

	lcd_data(G_BASE%256);				//low byte 
	lcd_data(G_BASE>>8);				//high byte
	lcd_command(0x42);			//set graphics home address
	

	lcd_data(XMAX/FONT_WIDTH);	//low byte = x_pixels/font_width
	lcd_data(0x00);				//high byte
	lcd_command(0x43);			//set graphics characters per line
	

	lcd_data(T_BASE%256);				//low byte
	lcd_data(T_BASE>>8);				//high byte
	lcd_command(0x40);			//set TEXT HOME address
	
	lcd_data(XMAX/FONT_WIDTH);	//low byte = x_pixels/font_width
	lcd_data(0x00);				//high byte
	lcd_command(0x41);			//set text line length BYTES_PER_ROW number of characters
	
	lcd_command(0b10011100); //turn g&t on
	
	


//--------end LCD init------


	lcd_clear_graph();
	lcd_clear_text();


} //end lcd_init





/***********************************************************************
Draws a line from x,y at given degree from inner_radius to outer_radius.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  lcd_line(fx,fy,tx,ty,show);
}


void lcd_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  lcd_line(fx,fy,tx,ty,show);
  lcd_line(fx+1,fy+1,tx+1,ty+1,show);
  lcd_line(fx-1,fy-1,tx-1,ty-1,show);
}






