#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bitops.h"
#include "sensors_oilpressure.h"


unsigned int 	oil_pressure = 0;
unsigned char 	oil_slope	= VDO_SLOPE;
signed char 	oil_offset = VDO_OFFSET;



void oil_pressure_setupADC()
{

CLRBIT(DDRF,OIL_PRESSURE_PIN);
ADMUX = OIL_PRESSURE;


}





void oil_pressure_processADC(unsigned int adc_value)
{

oil_pressure  = adc_value;

}


float oil_pressure_getPressure()
{

unsigned int pressure;

pressure = oil_pressure/oil_slope + oil_offset;

return pressure;
}
