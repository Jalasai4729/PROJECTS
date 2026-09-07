/*
 * EEPROM.c
 *
 *  Created on: Aug 14, 2026
 *      Author: sai
 */

/****************************************************************************************************************/
#include "main.h"
#include "EEPROM.h"
#include "RTC.h"
#include <stdio.h>


extern I2C_HandleTypeDef hi2c1;

/* Temperature variable from your temperature task */
extern volatile int TEMP;
extern uint8_t TX_RTC_DATA[7];


/* 24C08 EEPROM address */

#define EEPROM_ADDRESS    (0x50 << 1)


/*
 * Write RTC time, date and temperature
 * into EEPROM.
 */

void EEPROM_Write_RTC_Temperature(void)
{
	RTC_Read();
	 extern char time[17];
	 extern char date[17];

    HAL_StatusTypeDef status;

    /*
     * ------------------------------------------------
     * PAGE 1
     * EEPROM 0x00
     *
     * Store:time
     * Hour
     * Minute
     * Second
     * ------------------------------------------------
     */


    status = HAL_I2C_Mem_Write(&hi2c1,
                               EEPROM_ADDRESS,
                               0x00,
                               I2C_MEMADD_SIZE_8BIT,
                               time,
                               3,
                               1000);

    if(status != HAL_OK)
    {
        /* EEPROM write error */
    }

    HAL_Delay(5);


    /*
     * ------------------------------------------------
     * PAGE 2
     * EEPROM 0x10
     *
     * Store:date
     * Date
     * Month
     * Year
     * ------------------------------------------------
     */

    status = HAL_I2C_Mem_Write(&hi2c1,
                               EEPROM_ADDRESS,
                               0x10,
                               I2C_MEMADD_SIZE_8BIT,
                               date,
                               3,
                               1000);

    if(status != HAL_OK)
    {
        /* EEPROM write error */
    }

    HAL_Delay(5);


    /*
     * ------------------------------------------------
     * PAGE 3
     * EEPROM 0x20
     *
     * Store temperature
     * ------------------------------------------------
     */

    status = HAL_I2C_Mem_Write(&hi2c1,
                               EEPROM_ADDRESS,
                               0x20,
                               I2C_MEMADD_SIZE_8BIT,
							   TEMP,
                               1,
                               1000);

    if(status != HAL_OK)
    {
        /* EEPROM write error */
    }

    HAL_Delay(5);
}
