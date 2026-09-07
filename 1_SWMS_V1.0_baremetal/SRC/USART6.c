 #include "../inc/stm32f401rbt6.h"
 #include "../inc/GPIO.h"
 #include "../inc/systicktimer.h"
 #include "../inc/USART6.h"
  #include "../inc/WIFI.h"

 //char ch=0;

void KM_USART6_INIT(void)
{
	//INITIALIZATION PHASE A
	 
	  RCC_AHB1ENR |=(0x1<<2);//clock C
	  KM_GPIO_PORT_CONFIG(GPIOC,ALT,6);// PC6 MODE REGESTER SET
   KM_GPIO_PORT_CONFIG(GPIOC,ALT,7);//PC7 MODE REGESTER SET
	
	//GPIOC_MODER|=(0x2<<12);
	//	GPIOC_MODER|=(0x2<<14);

 GPIOC_AFRL &=~(0xff000000);///24-31
		GPIOC_AFRL |=(0x88000000);//PC6 ,PC7///
}
void KM_USART6_CONFIG(void)
{
	
	//CONFIGARATION PHASE B
	
	 

    RCC_APB2ENR |=(0x1<<5);//ENABLE CLOCK FOR UART
	
		USART6_BRR =0x8A;//LOAD BAUD RATE VALUE 115500
		USART6_CR1 |=(0x1<<3);//ENABLE TX	
		USART6_CR1 |=(0x1<<2);//ENABLE RX	
	
     USART6_CR1 |=(0x1<<13);//ENABLE UART UE BIT
}
 
void KM_USART6_OUT_CHAR(char ch)
{
        	while(!(USART6_SR & (0x1<<7)));//TX
	        USART6_DR =ch;
}

char KM_USART6_INCHAR()
{
	          while(!(USART6_SR & (0x1<<5)));//TX
    	    return USART6_DR ;
}

 char KM_USART6_INCHAR_TIME_OUT(void)
{
		     int j=5000000;

	      while(j)
			   {
          if(USART6_SR & (0x1<<5))//TX
					   {
  	         return  (USART6_DR  & 0xff) ; 

					    }
					 j--;
			   }
				return 0;
}

void KM_USART6_OUT_STR(char str[])
{
	int i=0;
	while(str[i])
		
	{
		KM_USART6_OUT_CHAR(str[i]);//calling
		i++;
	}
}
