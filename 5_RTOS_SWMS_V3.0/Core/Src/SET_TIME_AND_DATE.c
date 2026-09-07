/*
 * SET_TIME_AND_DATE.c
 *
 *  Created on: Aug 24, 2026
 *      Author: sai
 */

#include "main.h"
#include "cmsis_os.h"
#include "SET_TIME_AND_DATE.h"
#include "LCD.h"
#include "RTC.h"
#include <stdio.h>

volatile int ent_switch=0;

extern osThreadId_t Task_5Handle;
extern osEventFlagsId_t myEvent01Handle;

RTC_Data_t rtc;



/*****************************************************************************/
void RTC_CONFIG()
{
    uint32_t flags;

			   // Display header
          KM_LCD_WRITE_CMD(0x01);
          KM_LCD_WRITE_CMD(0x81);
          KM_LCD_WRITE_STR("HH:MM:SS    YUP");
          KM_LCD_WRITE_CMD(0xC1);
          KM_LCD_WRITE_STR("DD/MM/YY    NDN");

          flags = osThreadFlagsWait(
        		                 UP_BUTTON_FLAG |
        		                 DOWN_BUTTON_FLAG |
        		                 ENTER_BUTTON_FLAG,
        		                 osFlagsWaitAny,
        		                 osWaitForever);

     if(flags & UP_BUTTON_FLAG)//8TH  UP SWITCH
	  {
		  KM_LCD_WRITE_CMD(0x01);
		   ent_switch=0;

	   while(ent_switch <= 7)
	    {
           STOP_RTC_WRITE();	//DISPLAY DATE & TIME

		   flags = osThreadFlagsWait(
		                 UP_BUTTON_FLAG |
		                 DOWN_BUTTON_FLAG |
		                 ENTER_BUTTON_FLAG,
		                 osFlagsWaitAny,
		                 osWaitForever);
		   if (flags & osFlagsError)
		         {
		             continue;
		         }

		     if(flags & UP_BUTTON_FLAG)
		     {
		         SWITCH1_FLAG();
		     }
		    if(flags & DOWN_BUTTON_FLAG)
		     {
		         SWITCH2_FLAG();
		     }
		    if (flags & ENTER_BUTTON_FLAG)
		          {
		              ent_switch++;
		          }


		   	   if(ent_switch==7)
				  {
					  KM_LCD_WRITE_CMD(0x8b);
                      KM_LCD_WRITE_STR("set  ");
                      KM_LCD_WRITE_CMD(0xCa);
                       KM_LCD_WRITE_STR("update");

                   RTC_SetTimeDate(  rtc.hours,
		                             rtc.minutes,
		                             rtc.seconds,
									 0,
									 rtc.date,
									 rtc.month,
									 rtc.year);//UPDATE TIME AND DATE
				   }
            }
      }

     ent_switch=0;
		KM_LCD_WRITE_CMD(0x01);//CLEAR LCD SCREEN

}

/****** UP SWITCH PC-8 INCREMENTS *******************************************/

void SWITCH1_FLAG()
{
    if(ent_switch==1)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("sec  ");
       rtc.seconds++;
        if( rtc.seconds>59)
        	 rtc.seconds=0;
    }
    else if(ent_switch==2)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("min  ");
       rtc.minutes++;
        if(rtc.minutes>59)
        	rtc.minutes=0;
    }
    else if(ent_switch==3)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("hours  ");
       rtc.hours++;
        if(rtc.hours>12)
        	rtc.hours=1;
    }
    else if(ent_switch==4)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("day  ");
       rtc.date++;
        if(rtc.date>30)
        	rtc.date=1;
    }
    else if(ent_switch==5)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("month  ");
       rtc.month++;
        if(rtc.month>12)
        	rtc.month=1;
    }
    else if(ent_switch==6)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("year  ");
       rtc.year++;
        if(rtc.year>99)
        	rtc.year=1;
    }

}

/********* DN SWITCH PC-9 DECREMENTS*******************************************************/

void SWITCH2_FLAG()
{
    if(ent_switch==1)
    {
				KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("sec  ");
       rtc.seconds--;
        if( rtc.seconds<0)
        	 rtc.seconds=59;
    }
    else if(ent_switch==2)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("min  ");
       rtc.minutes--;
        if(rtc.minutes<0)
        	rtc.minutes=59;
    }
    else if(ent_switch==3)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("hours  ");
       rtc.hours--;
        if(rtc.hours<1)
        	rtc.hours=12;
    }
    else if(ent_switch==4)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("day  ");
       rtc.date--;
        if(rtc.date<1)
        	rtc.date=30;
    }
    else if(ent_switch==5)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("month  ");
       rtc.month--;
        if(rtc.month<1)
        	rtc.month=12;
    }
    else if(ent_switch==6)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("year  ");
       rtc.year--;
        if(rtc.year<1)
        	rtc.year=99;
    }

}

/****************************************************************************************************/

void STOP_RTC_WRITE()//READ FROM RTC
{

		             char time[17];
		             char date[17];

		             sprintf(time,
		                     "%02d:%02d:%02d",
		                     rtc.hours,
		                     rtc.minutes,
		                     rtc.seconds);

		             sprintf(date,
		                     "%02d-%02d-20%02d",
		                     rtc.date,
		                     rtc.month,
		                     rtc.year);

		             KM_LCD_WRITE_CMD(0x80);
		             KM_LCD_WRITE_STR(time);

		             KM_LCD_WRITE_CMD(0xC0);
		             KM_LCD_WRITE_STR(date);


}
/********************************************************************************************************/




