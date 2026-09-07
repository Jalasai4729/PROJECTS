/*
 * RTC.h
 *
 *  Created on: Aug 24, 2026
 *      Author: sai
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#define RTC_CONFIG_FLAG     (1U << 1)

#define UP_BUTTON_FLAG       (1U << 0)
#define DOWN_BUTTON_FLAG     (1U << 1)
#define ENTER_BUTTON_FLAG    (1U << 2)

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;

    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;

} RTC_Data_t;

HAL_StatusTypeDef RTC_Read(RTC_Data_t *rtc);


int BCD_To_DEC(int bcd);
int DEC_To_BCD(int dec);

HAL_StatusTypeDef RTC_SetTimeDate(uint8_t hour,
                                  uint8_t minute,
                                  uint8_t second,
                                  uint8_t day,
                                  uint8_t date,
                                  uint8_t month,
                                  uint8_t year);




#endif /* INC_RTC_H_ */
