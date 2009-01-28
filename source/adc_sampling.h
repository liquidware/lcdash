
#define ADC_TOTAL_CHANNELS	3
#define ADC_DELAY_MAX		2 //every 3*128us = 376us


#define ADC_CHAN0			0
#define ADC_CHAN1			1
#define ADC_CHAN2			2


#define MILLISMAX 			8	//128uS* 8 = 1.024 milliseconds


unsigned long millis(void); //returns the number of milliseconds since startup
