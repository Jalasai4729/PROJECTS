/*
 * RTC.c
 *
 * Created on: Aug 14, 2026
 * Author: sai
 */

#include "main.h"
#include "RTC.h"
#include "LCD.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;
char time[17];
  char date[17];

/***********************************************************************************************************************************************************/
#define DS1307_ADDRESS    0xD0

uint8_t rtc_data[7];

/*----------------------------------------------------------
 * BCD to Decimal
 *
 * Example:
 * 0x25 -> 25
 *----------------------------------------------------------*/
int BCD_To_DEC(int bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/*----------------------------------------------------------
 * Decimal to BCD
 *
 * Example:
 * 25 -> 0x25
 *----------------------------------------------------------*/
int DEC_To_BCD(int dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}


/*----------------------------------------------------------
 * RTC READ
 *----------------------------------------------------------*/
void RTC_Read(void)
{

    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;

    uint8_t day;
    uint8_t date_value;
    uint8_t month;
    uint8_t year;

    HAL_StatusTypeDef status;

    /* Read 7 registers from DS1307 */

    status = HAL_I2C_Mem_Read(&hi2c1,
                              DS1307_ADDRESS,
                              0x00,
                              I2C_MEMADD_SIZE_8BIT,
                              rtc_data,
                              7,
                              HAL_MAX_DELAY);

              // HAL_KM_I2C1_Mem_Read(0x68, 0x00, 0x01, TX_RTC_DATA, 7);//REGESTER LEVEL CODE
             // HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, 0x00,I2C_MEMADD_SIZE_8BIT,RX_RTC_DATA, 7, 1000);

    /* Check I2C read */

    if(status != HAL_OK)
    {
        KM_LCD_WRITE_CMD(0x80);
        KM_LCD_WRITE_STR("I2C READ ERROR");

        return;
    }

    /* Convert BCD to Decimal */

    seconds = BCD_To_DEC(rtc_data[0] & 0x7F);

    minutes = BCD_To_DEC(rtc_data[1] & 0x7F);

    /* 24-hour mode */

    hours = BCD_To_DEC(rtc_data[2] & 0x3F);

    day = BCD_To_DEC(rtc_data[3] & 0x07);

    date_value = BCD_To_DEC(rtc_data[4] & 0x3F);

    month = BCD_To_DEC(rtc_data[5] & 0x1F);

    year = BCD_To_DEC(rtc_data[6]);


    /* Create time */

    sprintf(time,
            "%02d:%02d:%02d",
            hours,
            minutes,
            seconds);

    /* Create date */

    sprintf(date,
            "%02d-%02d-20%02d",
            date_value,
            month,
            year);


    /* Display time */

    KM_LCD_WRITE_CMD(0x80);

    KM_LCD_WRITE_STR(time);


    /* Display date */

    KM_LCD_WRITE_CMD(0xC0);

    KM_LCD_WRITE_STR(date);
}


/*----------------------------------------------------------
 * SET TIME AND DATE
 *----------------------------------------------------------*/
HAL_StatusTypeDef RTC_SetTimeDate(uint8_t hour,
                                  uint8_t minute,
                                  uint8_t second,
                                  uint8_t day,
                                  uint8_t date,
                                  uint8_t month,
                                  uint8_t year)
{
    uint8_t data[7];

    /* Convert Decimal -> BCD */

    data[0] = DEC_To_BCD(second);
    data[1] = DEC_To_BCD(minute);
    data[2] = DEC_To_BCD(hour);
    data[3] = DEC_To_BCD(day);
    data[4] = DEC_To_BCD(date);
    data[5] = DEC_To_BCD(month);
    data[6] = DEC_To_BCD(year);

    /*
     * Write:
     *
     * 0x00 = Seconds
     * 0x01 = Minutes
     * 0x02 = Hours
     * 0x03 = Day
     * 0x04 = Date
     * 0x05 = Month
     * 0x06 = Year
     */

    return HAL_I2C_Mem_Write(&hi2c1,
                             DS1307_ADDRESS,
                             0x00,
                             I2C_MEMADD_SIZE_8BIT,
                             data,
                             7,
                             HAL_MAX_DELAY);
}

 //HAL_I2C_Mem_Write(&hi2c1, 0x68 << 1, 0x00,I2C_MEMADD_SIZE_8BIT,TX_RTC_DATA, 7, 1000);
     //HAL_KM_I2C1_Mem_Write(0x68, 0x00, 0x01, RX_RTC_DATA, 7);//REGESTER LEVEL CODE
