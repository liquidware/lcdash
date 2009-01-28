
#define MS1_SIG_SIZE	30

void ms1_init();
int ms1_getTemperature(unsigned char adc);
int ms1_getPressure(unsigned char adc);
int ms1_getBaro(unsigned char adc);
float ms1_getAFR(unsigned char adc);
void ms1_convertSensors(GAUGE * g_list[],unsigned char gauge_count);
void ms1_restoreSignature(char* signature);
void ms1_saveSignature(char* signature);
void ms1_restoreDefaultGauges();
void ms1_tpsGen(int low,int high);
unsigned char  ms1_getTPS(unsigned char adc);

