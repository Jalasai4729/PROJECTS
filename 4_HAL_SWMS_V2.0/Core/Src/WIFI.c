/*
 * WIFI.c
 *
 *  Created on: Aug 14, 2026
 *      Author: sai
 */

#include "main.h"
#include "RTC.h"
#include "LCD.h"
#include "WIFI.h"
#include <stdio.h>


extern UART_HandleTypeDef huart6;
extern int ERROR_WIFI;
extern int TEMP;


unsigned char rx_buffer[100];
char data_buffer[100];
int length=0;
char length_buffer[100];
int i=0;
int tcp_connected=0;

/****************** IS WIFI FOUND ********************************************************/

int IS_WIFI_FOUND(void) //OUT CHAR
{
	    char ret1=0;
				HAL_UART_Transmit (&huart6,(unsigned char*)"AT\r\n",10,100);

	//	  KM_USART6_OUT_STR("AT\r\n");  //CALLING TO OUTCHAR
	    ret1=WIFI_OK_RESPONSE();      //RECEIVING RESPONS
	    return ret1;
}

/**************** OK RESPONSE ******************************************************************************************/

int WIFI_OK_RESPONSE(void) //RECEIVE CHAR
{
	int i=0;
	while(i<100)
	     {
				 	 HAL_UART_Receive (&huart6,&rx_buffer[i],50,100);

     //  rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
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
					HAL_UART_Transmit (&huart6,(unsigned char*)"AT+RST\r\n",10,100);

//    KM_USART6_OUT_STR("AT+RST\r\n");// CALLING OUR CHAR FUN TX COMMAND
		KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN

    KM_LCD_WRITE_STR("RESTART DEVICE");
	  HAL_Delay(8000);

     	HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CWJAP?\r\n",20,100);

		//KM_USART6_OUT_STR("AT+CWJAP?\r\n");// CALLING OUR CHAR FUN TX COMMAND
	  ret2=WIFI_JAPQ_RESPONSE();//CALLING TO QUERY FUN
	  if(ret2==NO_AP)
		{
			ret2=WIFI_INIT();   //WIFI_INIT
		  KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("WIFI GOT IP");
	    HAL_Delay(2000);

		HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CIPMUX=0\r\n",20,100);

//			KM_USART6_OUT_STR("AT+CIPMUX=0\r\n");// CALLING OUR CHAR FUN TX COMMAND
			KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("CIPMUX=0 SINGLE");
      KM_LCD_WRITE_CMD(0XC0);//SECOND LINE
      KM_LCD_WRITE_STR("CONNECTION");
	    HAL_Delay(1000);

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
				 			HAL_UART_Receive (&huart6,&rx_buffer[i],10,100);

     //  rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
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
	return NO_AP;// 0 NO ACCESS POINT
}

/************** int WIFI_INIT ***************************************/

int WIFI_INIT(void)
{
		char ret3=0;
	     	HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CWMODE=3\r\n",20,100);

    //	KM_USART6_OUT_STR("AT+CWMODE=3\r\n");// CALLING OUR CHAR FUN TX COMMAND
	    ret3=WIFI_OK_RESPONSE();//CALLING TO OK RESPONSE


		  KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
      KM_LCD_WRITE_STR("CWMODE=3 OK");
	    HAL_Delay(1000);

        KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
        KM_LCD_WRITE_STR("SEND ID");
        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
        KM_LCD_WRITE_STR("PASSWORD");
		   //	KM_USART6_OUT_STR("AT+CWJAP=\"KMLearnX2-2.4G\",\"9963111084\"\r\n");// CALLING OUR CHAR FUN TX COMMAND  PASSWORD
	  // 	KM_USART6_OUT_STR("AT+CWJAP=\"Real Me Sai Rocks\",\"sai 4729\"\r\n");// CALLING OUR CHAR FUN TX COMMAND  PASSWORD

	     	HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CWJAP=\"Real Me Sai Rocks\",\"sai 4729\"\r\n",80,100);

	       HAL_Delay(5000);
			   KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
         KM_LCD_WRITE_STR("CONNECTING..");
			   HAL_Delay(5000);

	      KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
        KM_LCD_WRITE_STR("CONNECTING....");
				HAL_Delay(5000);

	    ret3=WIFI_JAP_RESPONSE();//
	    switch(ret3)
			{

				case '1':  	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CONNECTING");
					        KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("TIME OUT");
           				HAL_Delay(1000);
									ERROR_WIFI=1;
				         // while(1);

				case '2':  	KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("WRONG PASSWORD");

          				HAL_Delay(1000);
          				ERROR_WIFI=1;
			         //  while(1);

				case '3': KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("CAN NOT FOUND");
				     	    KM_LCD_WRITE_CMD(0xC0);          //CLEAR LCD SCREEN
                  KM_LCD_WRITE_STR("TARGET AP");
				          HAL_Delay(1000);
									ERROR_WIFI=1;
        //   				while(1);

				case '4':   KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                    KM_LCD_WRITE_STR("CONNECTION FAIL");
				        	  HAL_Delay(1000);
										ERROR_WIFI=1;
                 //   while(1);

				case ERROR_JAP: KM_LCD_WRITE_CMD(0x01);          //CLEAR LCD SCREEN
                        KM_LCD_WRITE_STR("PLEASE ENTER");
                        KM_LCD_WRITE_CMD(0xC0);
                        KM_LCD_WRITE_STR("CORRECT PSWD");
				        	      HAL_Delay(1000);
												ERROR_WIFI=1;
                       // while(1);
			}
return ret3;
}


/**************** WIFI_JAP_RESPONSE ********************************************/

int WIFI_JAP_RESPONSE()
{
	int i=0;
	while(i<100)

	{
						 	 HAL_UART_Receive (&huart6,&rx_buffer[i],10,100);

      // rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
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
	int i=0;
	    char ret4=0;

		     	HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CIPSTATUS\r\n",20,100);

		    ret4=WIFI_STATUS_RESPONSE();
	  		 if(ret4 != '2')   // NOT CONNECTED
    {
        tcp_connected = 0;   // force reconnect next time
    }
}
/******* void KM_WIFI_PUSH_SERVER  ***************************************/
	void KM_WIFI_PUSH_SERVER()
	{
		int i=0;
	char ret4=0;

		HAL_UART_Transmit (&huart6,(unsigned char*)"AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n",80,100);
      HAL_Delay(1000);


		 sprintf(data_buffer,"GET /page.php?temp=20&hum=40&dev=42\r\n");//STORE DATA BUFFER

		 length=MY_STRLENGTH(data_buffer);//CALLING STRING LENGTH
	   sprintf(length_buffer,"AT+CIPSEND=%d\r\n",length);//STORE LENGTH BUFFE
		HAL_UART_Transmit (&huart6,&length_buffer[i],80,100);

		//TEMPERATURE
	   HAL_Delay(4000);
		sprintf(data_buffer,"GET /page.php?temp=%d&hum=40&dev=42\r\n",TEMP);

				HAL_UART_Transmit (&huart6,&data_buffer[i],80,100);
		//		HAL_UART_Transmit(&huart6,(uint8_t *)&length_buffer[i],strlen(&length_buffer[i]),100);

		ret4=RESULT_RESPONSE();
		if(ret4==ERROR)
		{
		    	KM_LCD_WRITE_CMD(0xCB);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("ERROR");
 			   	ERROR_WIFI=1;

			}
		else
		{
		    	KM_LCD_WRITE_CMD(0xC9);          //CLEAR LCD SCREEN
          KM_LCD_WRITE_STR("   OK    ");
		}

}

/******************* WIFI_STATUS_RESPONSE **************************************************/

int WIFI_STATUS_RESPONSE()
{

	int i=0;
	while(i<100)
	     {
				 				 	 HAL_UART_Receive (&huart6,&rx_buffer[i],50,100);

       //rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
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
				 				 	 HAL_UART_Receive (&huart6,&rx_buffer[i],50,100);

      // rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
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

int RESULT_RESPONSE()
{
		int i=0;
	while(i<100)
	     {
				 				 	 HAL_UART_Receive (&huart6,&rx_buffer[i],50,100);

      // rx_buffer[i]=KM_USART6_INCHAR_TIME_OUT();    //CALLING IN CHAR RECEIVING RESPONSE
           if((rx_buffer[i]=='e') && (rx_buffer[i-1]=='n') && (rx_buffer[i-2]=='o') )
		         {
			       return OK;//FOUND '0'
		         }
		  i++;
	     }
	return ERROR;// '1'
}


