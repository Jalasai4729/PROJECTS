/*
 * WIFI.h
 *
 *  Created on: Aug 24, 2026
 *      Author: sai
 */

#ifndef INC_WIFI_H_
#define INC_WIFI_H_

#define WIFI_NOT_FOUND     '1'
#define WIFI_FOUND          '0'


#define WIFI_DISCONNECTED  '1'
#define WIFI_CONNECTED      '0'

#define NO_AP              '1'
#define AP                  '0'

#define ERROR_JAP '8'

//STATUS
#define ERROR    '1'
#define OK        '0'





void KM_WIFI_INIT(void);


int IS_WIFI_FOUND(void);
int WIFI_OK_RESPONSE(void);
int IS_WIFI_CONNECTED(void);
int WIFI_JAPQ_RESPONSE(void);
int WIFI_INIT(void);
int WIFI_JAP_RESPONSE(void);


/************** WI-FI opeartion ***********************************************************************************/
void KM_WIFI_OPERATION(void);
int KM_WIFI_PUSH_SERVER(void);


int WIFI_STATUS_RESPONSE(void);
int WIFI_START_OK_RESPONSE(void);
int MY_STRLENGTH(char str[]);
int RESULT_RESPONSE(void);

int TCP_RESPONSE(void);

int wait_for_char(char expected);


#endif /* INC_WIFI_H_ */
