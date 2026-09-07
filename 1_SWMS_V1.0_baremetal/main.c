/* QUESTION :
 * USART2
 * Microcontroller : STM32F401RBT6
 * RaayanMini      : 
 * Author : JALA SAI
 * Date : 13-02-2026
 * STLink Debugger : 
 * Hardware        : USART2 WIFI MODULE ESP2844
 
 */
 
 
 
 
 #include "../inc/stm32f401rbt6.h"
 #include "../inc/GPIO.h"
 #include "../inc/systicktimer.h"
 #include "../inc/USART6.h"
 #include "../inc/WIFI.h"
 #include "../inc/lcd.h " 
 #include "../inc/ADC1.h"
int ret=0; 
int temp=0;
int main()
{
 /********* Phase 1: Device Initialization *******************/
	 
   KM_GPIO_PORT_INIT(GPIOB);
	 KM_GPIO_PORT_INIT(GPIOC);


/********* Phase 2: Device configuration *********************/
	
  //SYSTICK  FUNCTION
	
               KM_STK_INIT();
	
         KM_PIN_LCD_INIT();  //pins initilization
         KM_LCD_INIT();      //LCD INITILIZATION

        KM_ADC1_INIT();
 //USART2 CONFIGURE
	     	         KM_USART6_INIT();
								KM_USART6_CONFIG();

	             KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
               KM_LCD_WRITE_STR("WELCOME TO");
							 KM_LCD_WRITE_CMD(0xC0);              //PRINT SECOND LINE
               KM_LCD_WRITE_STR("   S W M S   ");
							 KM_mdelay_IT(1000);

	/********************** DEVICE CHECKING *********************************************************/
   //IS WIFI FOUND 

					 ret=IS_WIFI_FOUND();
           if(ret==WIFI_NOT_FOUND)//-1 == -1  //CONDITON TRUE
				     {  
							 KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
               KM_LCD_WRITE_STR("WIFI NOT FOUND");
							 KM_LCD_WRITE_CMD(0xC0);              //PRINT SECOND LINE
               KM_LCD_WRITE_STR("RESET BOARD");
							 while(1);
				     }	
			 KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN 
			 KM_LCD_WRITE_STR("AT OK");
			 KM_LCD_WRITE_CMD(0xC0);               //PRINTF SECOND LINE
       KM_LCD_WRITE_STR("WIFI FOUND");
	   	 KM_mdelay_IT(1000);

/************************* IS WIFI CONNECTED ************************************************************/
						 
			     ret=IS_WIFI_CONNECTED();
				   if(ret==WIFI_CONNECTED)//0 == 0  //CONDITON TRUE
		       KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN 
		     	 KM_LCD_WRITE_STR("WIFI CONNECTED");
					 	KM_mdelay_IT(2000);

/**********************************************************************************************************/					 

		//  KM_WIFI_OPERATION();


/************* OPERATION PHASE ************************************/
	

  while(1)
  {
		
					KM_mdelay_IT(1000);
         KM_ADC1_READ();                                                              
         KM_WIFI_OPERATION();
	     	KM_mdelay_IT(3000);
	}
}

	void ADC_IRQHandler()
	{
			temp = (ADC1_DR); // Store the Dout value in pot
       temp=(temp*0.08);
		
	}
	
//SysTick Interrupt Handler 
void SysTick_Handler(void)
{
    jiffie++;                          // Increment every 1 ms
}
