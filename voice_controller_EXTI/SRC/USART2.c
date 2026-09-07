 #include "../inc/stm32f401rbt6.h"
 #include "../inc/GPIO.h"
 #include "../inc/systicktimer.h"
 #include "../inc/USART2.h"

 //char ch=0;

void KM_USART2_INIT(void)
{
	//INITIALIZATION PHASE A
	 
	 // RCC_AHB1ENR |=(0x1<<2);//clock C
	  KM_GPIO_PORT_CONFIG(GPIOA,ALT,2);// PA2 MODE REGESTER SET
    KM_GPIO_PORT_CONFIG(GPIOA,ALT,3);//PA3 MODE REGESTER SET
	

    GPIOA_AFRL &=~(0x00007700);///24-31
		GPIOA_AFRL |=(0x00007700);//PA2 PA3
}
void KM_USART2_CONFIG(void)
{
	
	//CONFIGARATION PHASE B
	
    RCC_APB1ENR |=(0x1<<17);//ENABLE CLOCK FOR UART
	
		USART2_BRR =0x0683;//LOAD BAUD RATE VALUE 9600
		USART2_CR1 |=(0x1<<3);//ENABLE TX	
		USART2_CR1 |=(0x1<<2);//ENABLE RX	
	
	//	USART2_CR1 |=(0x1<<7);//ENABLE TXEIE INTERRUPT	
//		USART2_CR1 |=(0x1<<5);//ENABLE RXNEIE INTERRUPT
	
//   	NVIC_ISER1 |=(0x1<<6);


	
     USART2_CR1 |=(0x1<<13);//ENABLE UART UE BIT
}
 
void KM_USART2_OUTCHAR(char ch)
{
        	while(!(USART2_SR & (0x1<<7)));//TXE
	        USART2_DR =ch;
}

char KM_USART2_INCHAR()
{
	          while(!(USART2_SR & (0x1<<5)));//RXNE
    	    return USART2_DR ;
}

 char KM_USART2_INCHAR_TIME_OUT(void)
{
		     int j=5000000;

	      while(j)
			   {
          if(USART2_SR & (0x1<<5))//RXNE
					   {
  	         return  (USART2_DR  & 0xff) ; 

					    }
					 j--;
			   }
				return 0;
}

void KM_USART2_OUT_STR(char str[])
{
	int i=0;
	while(str[i])
		
	{
		KM_USART2_OUTCHAR(str[i]);//calling
		i++;
	}
}
