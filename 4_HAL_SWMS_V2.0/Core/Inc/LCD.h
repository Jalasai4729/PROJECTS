/*
 * LCD.h
 *
 *  Created on: Aug 14, 2026
 *      Author: sai
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

//Example: KM_Lcd_Pin_Init(LCD_PORT, LCD_D4, LCD_D5, LCD_D6, LCD_D7, LCD_RS, LCD_RW, LCD_EN);
void KM_LCD_INIT(void);  //Flow chart
void KM_LCD_WRITE_DATA(unsigned char );
void KM_LCD_WRITE_CMD(unsigned char );
void KM_WRITE_LOWER_NIBBLE(unsigned char );
void KM_WRITE_HIGHER_NIBBLE(unsigned char  );
void KM_LCD_WRITE_STR( char str[]);

void KM_ITOA_STR(int);
void KM_PIN_LCD_INIT(void);  //Flow chart

#endif /* INC_LCD_H_ */
