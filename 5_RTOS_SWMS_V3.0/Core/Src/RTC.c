/*
 * RTC.c
 *
 * Created on: Aug 24, 2026
 * Author: sai
 */

#include "main.h"
#include "RTC.h"
#include "LCD.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

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
HAL_StatusTypeDef RTC_Read(RTC_Data_t *rtc)
{
    uint8_t rtc_data[7];

    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(&hi2c1,
                              DS1307_ADDRESS,
                              0x00,
                              I2C_MEMADD_SIZE_8BIT,
                              rtc_data,
                              7,
                              HAL_MAX_DELAY);

    if(status != HAL_OK)
    {
        return status;
    }

    /* Convert BCD to Decimal */

    rtc->seconds = BCD_To_DEC(rtc_data[0] & 0x7F);

    rtc->minutes = BCD_To_DEC(rtc_data[1] & 0x7F);

    rtc->hours = BCD_To_DEC(rtc_data[2] & 0x3F);

    rtc->day = BCD_To_DEC(rtc_data[3] & 0x07);

    rtc->date = BCD_To_DEC(rtc_data[4] & 0x3F);

    rtc->month = BCD_To_DEC(rtc_data[5] & 0x1F);

    rtc->year = BCD_To_DEC(rtc_data[6]);

    return HAL_OK;
}

/**********************************************************************************************************************/



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


/*******************************************************************************************************************/
