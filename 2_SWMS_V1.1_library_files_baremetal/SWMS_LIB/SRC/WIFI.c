 #include "../inc/stm32f401rbt6.h"
 #include "../inc/GPIO.h"
 #include "../inc/systicktimer.h"
 #include "../inc/USART6.h"
 #include "../inc/WIFI.h"
 #include "../inc/lcd.h " 
 #include <stdio.h>



char rx_buffer[100];
//char ret1=0;
char data_buffer[100];
int length=0;
char length_buffer[100];
int extern temp;

/****************** IS WIFI FOUND ********************************************************/

int IS_WIFI_FOUND(void) //OUT CHAR 
{
	    char ret1=0;
		  KM_USART6_OUT_STR("AT\r\n");  //CALLING TO OUTCHAR
	    ret1=WIFI_OK_RESPONSE();      //RECEIVING RESPONS
	    return ret1;
}

/**************** OK RESPONSE ******************************************************************************************/

int WIFI_OK_RESPONSE(void) //RECEIVE CHAR
{
	int i=0;
	while(i<100)
	     {
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
           if((rx_buffer[i]=='K') && (rx_buffer[i-1]=='O'))
		         {
			       return WIFI_FOUND;//FOUND 0
		         }
		  i++;
	     }
	return WIFI_NOT_FOUND;// -1
}  

/*************** IS_WIFI_CONNECTED **********************************************************************************/

int IS_WIFI_CONNECTED()
{   
   	char ret2=0;
    KM_USART6_OUT_STR("AT+RST\r\n");// CALLING OUR CHAR FUN TX COMMAND 
		KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
    KM_LCD_WRITE_STR("RESTART DEVICE"); 
	  KM_mdelay_IT(8000);
	
     
		KM_USART6_OUT_STR("AT+CWJAP?\r\n");// CALLING OUR CHAR FUN TX COMMAND 
	  ret2=WIFI_JAPQ_RESPONSE();//CALLING TO QUERY FUN
	  if(ret2==NO_AP)
		{
			ret2=WIFI_INIT();   //WIFI_INIT
		  KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("WIFI GOT IP"); 
	    KM_mdelay_IT(2000);
			
			KM_USART6_OUT_STR("AT+CIPMUX=0\r\n");// CALLING OUR CHAR FUN TX COMMAND 
			KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("CIPMUX=0 SINGLE");
      KM_LCD_WRITE_CMD(0XC0);//SECOND LINE
      KM_LCD_WRITE_STR("CONNECTION"); 			
	    KM_mdelay_IT(1000);
			
			return WIFI_CONNECTED;

		}
    return WIFI_CONNECTED;
}

/***************** WIFI_JAPQ_RESPONSE ***********************************************************/

int WIFI_JAPQ_RESPONSE(void)
{
		int i=0;
	while(i<100)
	     {
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
           if((rx_buffer[i]=='o') && (rx_buffer[i-1]=='N'))//NO AP
		         {
			       return NO_AP;//FOUND -1
		         }
					 if((rx_buffer[i]=='C') && (rx_buffer[i-1]=='+'))//NO AP
		         {
			       return NO_AP;//FOUND -1
		         }
						 
		  i++;
	     }
	return AP;// 0 NO ACCESS POINT
}

/************** int WIFI_INIT ***************************************/

int WIFI_INIT(void)
{
		char ret3=0;
    	KM_USART6_OUT_STR("AT+CWMODE=3\r\n");// CALLING OUR CHAR FUN TX COMMAND  
	    ret3=WIFI_OK_RESPONSE();//CALLING TO OK RESPONSE
	   
	
		  KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("CWMODE=3 OK"); 
	    KM_mdelay_IT(1000);

        KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
        KM_LCD_WRITE_STR("SEND ID");
        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
        KM_LCD_WRITE_STR("PASSWORD"); 		
		   //	KM_USART6_OUT_STR("AT+CWJAP=\"KMLearnX2-2.4G\",\"9963111084\"\r\n");// CALLING OUR CHAR FUN TX COMMAND  PASSWORD
	  // 	KM_USART6_OUT_STR("AT+CWJAP=\"Real Me Sai Rocks\",\"sai 4729\"\r\n");// CALLING OUR CHAR FUN TX COMMAND  PASSWORD
		KM_USART6_OUT_STR(ID);
     //	KM_USART6_OUT_STR("AT_CWJAP=\"Chinnu\",\"8309446752\"\r\n");// CALLING OUR CHAR FUN TX COMMAND  PASSWORD
	  // KM_mdelay_IT(2000);
	   
	    ret3=WIFI_JAP_RESPONSE();//
	    switch(ret3)
			{                                                                                                                                                                                                                                                                                                                                                                                                                      

				case '1':  	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CONNECTING"); 
					        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("TIME OUT"); 
           				KM_mdelay_IT(1000);
				          while(1);
				
				case '2':  	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("WRONG PASSWORD"); 
				
          				KM_mdelay_IT(1000);
			           while(1);
				
				case '3': KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CAN NOT FOUND"); 
				     	    KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("TARGET AP");
				          KM_mdelay_IT(1000);
           				while(1);
				
				case '4':   KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                    KM_LCD_WRITE_STR("CONNECTION FAIL"); 
				        	  KM_mdelay_IT(1000);
                    while(1);
										
				case ERROR_JAP: KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                        KM_LCD_WRITE_STR("PLEASE ENTER");
                        KM_LCD_WRITE_CMD(0xC0);
                        KM_LCD_WRITE_STR("CORRECT PSWD"); 
				        	      KM_mdelay_IT(1000);
                        while(1);										
			} 
return ret3;
}


/**************** WIFI_JAP_RESPONSE ********************************************/
int WIFI_JAP_RESPONSE()
{
	int i=0;
	while(i<100)
	     
	{
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
				 if((rx_buffer[i]>'0') && (rx_buffer[i]<'9') && (rx_buffer[i-1]==':'))
		         {
			       return rx_buffer[i];
		         }
				 if((rx_buffer[i]>'R') && (rx_buffer[i-1]<'O') && (rx_buffer[i-2]=='R'))
		         {
			       return ERROR_JAP;//'8'
		         }
		  i++;
	     }
	return WIFI_CONNECTED;//'0'
}

/************* KM_WIFI_OPERATION *************************************************/

void KM_WIFI_OPERATION(void)
{ 
	
	    char ret4=0;
			KM_USART6_OUT_STR("AT+CIPSTATUS\r\n");// STATUS CALLING OUR CHAR FUN TX COMMAND 
    ret4=WIFI_STATUS_RESPONSE();
	  switch(ret4)
		{
			case '0': 	KM_LCD_WRITE_CMD(0x01); 			//CLEAR LCD SCREEN
                 	KM_LCD_WRITE_STR("CIPSTATUS");
		             	KM_LCD_WRITE_CMD(0xC0); 			//CLEAR LCD SCREEN

            			KM_LCD_WRITE_STR("CONNECTED");
                  KM_USART6_OUT_STR("AT+CIPCLOSE\r\n");// STATUS CALLING OUR CHAR FUN TX COMMAND 
 			            KM_mdelay_IT(1000);
			            break;
			
			case '2':  	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CIP STATUS"); 
					        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("WIFI GOT IP"); 
           				KM_mdelay_IT(1000);
			           break;
			
			case '3':	  KM_LCD_WRITE_CMD(0x01); 			//CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CIPSTATUS");
			            KM_LCD_WRITE_CMD(0xC0); 			//CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CONNECTED");
			            KM_USART6_OUT_STR("AT+CIPCLOSE\r\n");// STATUS CALLING OUR CHAR FUN TX COMMAND 
 			            KM_mdelay_IT(1000);
			            break;
			
			case '5': 	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CONNECTION FAIL"); 
								  KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("PLEASE RESET ");  
			            ret4=IS_WIFI_CONNECTED();
	
            			KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("WIFI CONNECTED"); 
					        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("GOT IP");  
           				KM_mdelay_IT(1000);
				          break;
			
		}
		KM_USART6_OUT_STR("AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n");// STATUS CALLING OUR CHAR FUN TX COMMAND 
    ret4=WIFI_START_OK_RESPONSE();
		if(ret4==ERROR)
		{
					KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("ALREADY"); 
 			    KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("CONNECTED");  
	   		  KM_mdelay_IT(1000);
		}
			KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("CONNECTED DATA"); 
		  KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("PUSH SERVER"); 
		
      KM_mdelay_IT(1000);
		
		
		 sprintf(data_buffer,"GET /page.php?temp=20&hum=40&dev=42\r\n");//STORE DATA BUFFER
		
		 length=MY_STRLENGTH(data_buffer);//CALLING STRING LENGTH
	   sprintf(length_buffer,"AT+CIPSEND=%d\r\n",length);//STORE LENGTH BUFFER
     KM_USART6_OUT_STR(length_buffer);// STATUS CALLING OUR CHAR FUN TX COMMAND 
		
		//TEMPERATURE
	   KM_mdelay_IT(3000);
	//	sprintf(data_buffer,"GET /page.php?temp=%d&hum=40&dev=42\r\n",temp);
		sprintf(data_buffer,GET_PAGE,temp);
		KM_USART6_OUT_STR(data_buffer);// STATUS CALLING OUR CHAR FUN TX COMMAND 
		
		ret4=RESULT_RESPONSE();
		if(ret4==ERROR)
		{
		    	KM_LCD_WRITE_CMD(0xCB);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("ERROR"); 
 			   // KM_LCD_WRITE_CMD(0xC0); 			//CLEAR LCD SCREEN
			  while(1);
		}
		    	KM_LCD_WRITE_CMD(0xCc);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("OK"); 


}

/******************* WIFI_STATUS_RESPONSE **************************************************/

int WIFI_STATUS_RESPONSE()
{
	
	int i=0;
	while(i<100)
	     {
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
				 if((rx_buffer[i]>'0') && (rx_buffer[i]<'9') && (rx_buffer[i-1]==':'))
		         {
			       return rx_buffer[i];
		         }
		  i++;
	     }
	//return WIFI_CONNECTED;//'0'
	
}

/********************* WIFI_STATUS_OK_RESPONSE *******************************************************************/

int WIFI_START_OK_RESPONSE()
{
	int i=0;
	while(i<100)
	     {
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
           if((rx_buffer[i]=='K') && (rx_buffer[i-1]=='O'))
		         {
			       return OK;//FOUND '0'
		         }
		  i++;
	     }
	return ERROR;// '1'
}

/************* MY_STRLENGTH ***********************************************************************************************/

int MY_STRLENGTH(char str[])
{
	int i=0;
	while(str[i])
	{
		i++;
	}
	return i;
}

/****************************************************************************************************************************/


/****************************************************************************************************************************/

int RESULT_RESPONSE()
{
		int i=0;
	while(i<100)
	     {
       rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
           if((rx_buffer[i]=='e') && (rx_buffer[i-1]=='n') && (rx_buffer[i-2]=='o') )
		         {
			       return OK;//FOUND '0'
		         }
		  i++;
	     }
	return ERROR;// '1'
}

/***********************************************************************************************************************************/

