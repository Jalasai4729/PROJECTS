/*
 * SET_TIME_AND_DATE.c
 *
 *  Created on: Aug 14, 2026
 *      Author: sai
 */

#include "main.h"
#include "SET_TIME_AND_DATE.h"
#include "LCD.h"
#include "RTC.h"

extern int flag1;
extern int flag2;
extern int flag3;
extern int ent_switch;


int SEC1=0;
int MIN1=0;
int HOURS1=0;


int DAY1=0;
int MONTH1=0;
int YEAR1=0;
volatile int TIME=0;
volatile int DATE=0;

void RTC_CONFIG()
{
			   // Display header
          KM_LCD_WRITE_CMD(0x01);
          KM_LCD_WRITE_CMD(0x81);
          KM_LCD_WRITE_STR("HH:MM:SS    YUP");
          KM_LCD_WRITE_CMD(0xC1);
          KM_LCD_WRITE_STR("DD/MM/YY    NDN");

         while(!(flag1) && !(flag2 ));// BLOCKING

     if(flag1)//8TH  UP SWITCH
	  {
				 KM_LCD_WRITE_CMD(0x01);

				 flag1=0;
				 ent_switch=0;
				 while(ent_switch <= 7)
				  {
          // Handle switch updates
           if(flag1==1)
           SWITCH1_FLAG();
            else if(flag2==1)
            SWITCH2_FLAG();

            STOP_RTC_WRITE();	//DISPLAY DATE & TIME

		   		  if(ent_switch==7)
				     {
					   KM_LCD_WRITE_CMD(0x8b);
                      KM_LCD_WRITE_STR("set  ");
                      KM_LCD_WRITE_CMD(0xCa);
                       KM_LCD_WRITE_STR("update");

                   RTC_SetTimeDate(HOURS1,MIN1,SEC1,0,DAY1,MONTH1, YEAR1);////UPDATED DATE AND TIME
				      }
            }
      }
		flag2=0;//DN 9 SWIITCH CLEAR
		flag3=0;//ENT 10 SWITCH CLEAR
		KM_LCD_WRITE_CMD(0x01);//CLEAR LCD SCREEN

}

/****** UP SWITCH PC-8 INCREMENTS *******************************************/
void SWITCH1_FLAG()
{
    if(ent_switch==1)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("sec  ");
        SEC1++;
        if(SEC1>59)
					SEC1=0;
    }
    else if(ent_switch==2)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("min  ");
        MIN1++;
        if(MIN1>59)
					MIN1=0;
    }
    else if(ent_switch==3)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("hours  ");
        HOURS1++;
        if(HOURS1>12)
					HOURS1=1;
    }
    else if(ent_switch==4)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("day  ");
        DAY1++;
        if(DAY1>30) DAY1=1;
    }
    else if(ent_switch==5)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("month  ");
        MONTH1++;
        if(MONTH1>12)
					MONTH1=1;
    }
    else if(ent_switch==6)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("year  ");
        YEAR1++;
        if(YEAR1>99)
					YEAR1=1;
    }

    flag1=0;
}

/********* DN SWITCH PC-9 DECREMENTS*******************************************************/

void SWITCH2_FLAG()
{
    if(ent_switch==1)
    {
				KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("sec  ");
        SEC1--;
        if(SEC1<0)
					SEC1=59;
    }
    else if(ent_switch==2)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("min  ");
        MIN1--;
        if(MIN1<0)
					MIN1=59;
    }
    else if(ent_switch==3)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("hours  ");
        HOURS1--;
        if(HOURS1<1)
					HOURS1=12;
    }
    else if(ent_switch==4)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("day  ");
        DAY1--;
        if(DAY1<1)
					DAY1=30;
    }
    else if(ent_switch==5)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("month  ");
        MONTH1--;
        if(MONTH1<1)
					MONTH1=12;
    }
    else if(ent_switch==6)
    {
			 KM_LCD_WRITE_CMD(0x8B);
       KM_LCD_WRITE_STR("year  ");
        YEAR1--;
        if(YEAR1<1)
					YEAR1=99;
    }

    flag2=0;
}

/****************************************************************************************************/

void STOP_RTC_WRITE()//READ FROM RTC
{
	  // Fill TIME buffer properly
    sprintf(TIME,"%02d:%02d:%02d", HOURS1, MIN1, SEC1);

    KM_LCD_WRITE_CMD(0x81);
    KM_LCD_WRITE_STR(TIME);

    // Use decimal format
    sprintf(DATE,"%02d/%02d/%02d", DAY1, MONTH1, YEAR1);

    KM_LCD_WRITE_CMD(0xC1);
    KM_LCD_WRITE_STR(DATE);

}
