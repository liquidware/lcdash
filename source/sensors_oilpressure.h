
#define VDO_SLOPE		11
#define VDO_OFFSET		-6



#define LCDASH_OIL_PRESSURE	202 //id for the gauge listing

#define OIL_PRESSURE		0b10000100
#define OIL_PRESSURE_PIN	PF4

void oil_pressure_calibrate(unsigned char oil_sensor);
void oil_pressure_setupADC(void);
void oil_pressure_processADC(unsigned int adc);
float oil_pressure_getPressure();
