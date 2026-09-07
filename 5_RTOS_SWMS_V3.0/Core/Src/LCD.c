/*
 * LCD.c
 *
 *  Created on: Aug 24, 2026
 *      Author: sai
 */

#include "main.h"
#include "LCD.h"

void KM_LCD_WRITE_CMD(unsigned char data)
{
	HAL_GPIO_WritePin(GPIOB,LCD_RS_Pin,GPIO_PIN_RESET);   //to clear 4th bit in GPIOB_ODR to configure RS to instruction register
	HAL_GPIO_WritePin(GPIOB,LCD_RW_Pin,GPIO_PIN_RESET);  //to clear 5th bit in GPIOB_ODR to configure RW BIT
	KM_WRITE_HIGHER_NIBBLE(data);  //extract high nibble
	KM_WRITE_LOWER_NIBBLE(data); //extract low nibble
}
void KM_LCD_WRITE_DATA(unsigned char data)
{
	HAL_GPIO_WritePin(GPIOB,LCD_RS_Pin,GPIO_PIN_SET);   //to SET 4th bit in GPIOB_ODR to configure RS to instruction register
	HAL_GPIO_WritePin(GPIOB,LCD_RW_Pin,GPIO_PIN_RESET); ////to clear 5th bit in GPIOB_ODR to configure RW BIT
	KM_WRITE_HIGHER_NIBBLE(data); //extract high nibble
	KM_WRITE_LOWER_NIBBLE(data); //extract low nibble
}


void KM_WRITE_HIGHER_NIBBLE(unsigned char data) //to extract the high nibble of data send to LCD
{
	HAL_GPIO_WritePin(GPIOB,LCD_D3_Pin,data&128);   // 3RD bit in GPIOB_ODR to configure DATA BIT-3
	HAL_GPIO_WritePin(GPIOB,LCD_D2_Pin,data&64);   // 2ND bit in GPIOB_ODR to configure DATA BIT-2
	HAL_GPIO_WritePin(GPIOB,LCD_D1_Pin,data&32);   // 1ST bit in GPIOB_ODR to configure DATA BIT-1
	HAL_GPIO_WritePin(GPIOB,LCD_D0_Pin,data&16);   // 0TH bit in GPIOB_ODR to configure DATA BIT-0
	HAL_GPIO_WritePin(GPIOB,LCD_EN_Pin,GPIO_PIN_SET);   //to SET 8th bit in GPIOB_ODR to configure EN BIT
    HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOB,LCD_EN_Pin,GPIO_PIN_RESET);    //to SET 8th bit in GPIOB_ODR to configure EN BIT
}

void KM_WRITE_LOWER_NIBBLE(unsigned char data) //to extract the low nibble of data send to LCD
{
  HAL_GPIO_WritePin(GPIOB,LCD_D3_Pin,data&8);   // 3RD bit in GPIOB_ODR to configure DATA BIT-3
	HAL_GPIO_WritePin(GPIOB,LCD_D2_Pin,data&4);   // 2ND bit in GPIOB_ODR to configure DATA BIT-2
	HAL_GPIO_WritePin(GPIOB,LCD_D1_Pin,data&2); // 1ST bit in GPIOB_ODR to configure DATA BIT-1
	HAL_GPIO_WritePin(GPIOB,LCD_D0_Pin,data&1);  // 0TH bit in GPIOB_ODR to configure DATA BIT-0
	HAL_GPIO_WritePin(GPIOB,LCD_EN_Pin,GPIO_PIN_SET);    //to SET 8th bit in GPIOB_ODR to configure EN BIT
    HAL_Delay(3);
	HAL_GPIO_WritePin(GPIOB,LCD_EN_Pin,GPIO_PIN_RESET);    //to SET 8th bit in GPIOB_ODR to configure EN BIT
}
void KM_LCD_INIT()
{
  HAL_Delay(50);
	KM_LCD_WRITE_CMD(0x33);
  HAL_Delay(5);
	KM_LCD_WRITE_CMD(0x32);
  KM_LCD_WRITE_CMD(0x0C);
	KM_LCD_WRITE_CMD(0x01);
}
void KM_LCD_WRITE_STR(char str[])
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        if(i == 16)
            KM_LCD_WRITE_CMD(0xC0); // move to second line

        KM_LCD_WRITE_DATA(str[i]);
    }
}


void KM_ITOA_STR(int num)
{
	char string[50];
	 	int reminder=0,temp=0;
	  int i=0;
	 int j=0;

    while(num)  //CONVERTED TO STRING
  	{
	    reminder  = (num%10);
	  	string[i] =(reminder + '0');
	   	 num= (num/10);
	   	i++;
   	}
		string[i]='\0';

		for (j=0;j<i/2;j++)
		{
			temp = string[j];
			string[j]=string[i-1-j];
			string[i-1-j]=temp;
		}
	   KM_LCD_WRITE_STR(string); //CALLING STRING
}
