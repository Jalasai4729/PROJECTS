
#include "../inc/STM32F401RBT6.h"
#include "../inc/GPIO.h"
#include "../inc/LCD.h"
#include "../inc/systicktimer.h"
#include "../inc/ADC1.h"


int temp_sensor =0;

 void KM_ADC1_INIT(void)// – Initialize ADC1 and configure Channel 10 (PC0) and PC1
 {	 
	 
	 
	     KM_GPIO_PORT_INIT(GPIOC);             //CLOCK FOR C PORT
		   KM_GPIO_PORT_CONFIG(GPIOC,ANALOG,0);  //PC0 LM35
		   KM_GPIO_PORT_CONFIG(GPIOC,ANALOG,1);  //PC1 POTENTIOMETER
	 
	 RCC_APB2ENR |=(0x1<<8); //clock for adc
	  ADC1_CR1 |=(0x1<< SCAN);// 8 BIT SET 1 ON SCAN MODE
	    ADC1_SQR3 |= (0XA);   // Loading 01010 0-4 the  VALUE Channel Number 10 LM35
	       //  ADC1_CR2 |= (0X1<<10);    // EOCs

			 ADC1_CR1 |=(0x1<< 5);
	 NVIC_ISER0 |=(0x1<<18);
      ADC1_CR2 |= (0X1);    // ADON 
	 

 }
 
void KM_ADC1_READ(void) //– Read ADC output, convert to temperature in °C
	{
					    ADC1_CR2 |= (0X1 << 30);  // START SWSTART

	}
	