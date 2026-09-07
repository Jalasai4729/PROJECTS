/****************************************************
*   FILE : km_ADC1.h
* DESCRIPTION :This file contain function declaration related to ADC1 initialization
								and configuration
****************************************************/

 
 void KM_ADC1_INIT(void);// – Initialize ADC1 and configure Channel 10 (PC0)

void KM_ADC1_READ(void); //– Read ADC output, convert to temperature in °C


//Add the below MACROS for portability.

#define ADC1_IN10  0    //PC0   LM35 SENSOR
#define ADC1_IN11  1    //PC1    POTENTIOMETER
#define ADON       0
#define SCAN       8
#define EOCIE       5
#define EOCS        10

#define SWSTART     30 
/*#define
#define
#define
#define
#define
#define
#define*/