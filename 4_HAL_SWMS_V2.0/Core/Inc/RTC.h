/*
 * RTC.h
 *
 *  Created on: Aug 14, 2026
 *      Author: sai
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

void RTC_Read(void);

unsigned char dec_to_bcd(unsigned char val);
unsigned char bcd_to_dec(unsigned char val);


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
