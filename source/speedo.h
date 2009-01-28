/*! 
	\file speedo.h
	Speedometer Input capture calculations
*/

#define LCDASH_SPEEDO				200 //!<An LCDash mesaurable parameter
#define LCDASH_VSS_PPM				201 //!<An LCDash mesaurable parameter
#define AVERAGE_SAMPLES				4 	//!<The amount of samples to average

unsigned char speedo_getMPH();
void speedo_init();
unsigned int speedo_getPulsesPerMile();
void speedo_calibrate();
