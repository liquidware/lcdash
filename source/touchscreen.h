#ifndef _TOUCHSCREEN_H_
#define _TOUCHSCREEN_H_

typedef struct Point
{
	unsigned char x; /*!< X coordinate, 8-bit */
	unsigned char y; /*!< X coordinate, 8-bit */
}POINT; 


void touch_init();

void touchscreen_setup_x();
void touchscreen_setup_y();
void touchscreen_process_y(unsigned char adc_value);
void touchscreen_process_x(unsigned char adc_value);


void delay_ms(unsigned char x);

char touch_calibrate(void);
char touch_get_cursor(POINT* p);


#define TOUCHSCR_IS_CALIBRATED 14 //!< Variable, that if changed will force a calibrate 


#define xDir	0
#define yDir	1
#define yes	1
#define no	0

//pin defines
#define XPLUS_PIN	PF3
#define XMINUS_PIN	PF1
#define YPLUS_PIN	PF2
#define YMINUS_PIN	PF0

#define YPLUS	0b01110010 //left adjusted result in ADCH (8-bit precision)
#define XPLUS	0b01100011 
#define XMINUS	0b01100001
#define YMINUS	0b01100000



#endif
