

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

#define ID  "AT+CWJAP=\"Real Me Sai Rocks\",\"sai 4729\"\r\n"
#define GET_PAGE  "GET /page.php?temp=%d&hum=40&dev=42\r\n" //(%d = temp value replace)

/************** WI-FI initialization **************************************************************************/


int IS_WIFI_FOUND(void); 
int WIFI_OK_RESPONSE(void);
int IS_WIFI_CONNECTED(void);
int WIFI_JAPQ_RESPONSE(void);
int WIFI_INIT(void);
int WIFI_JAP_RESPONSE(void);


/************** WI-FI opeartion ***********************************************************************************/

int WIFI_STATUS_RESPONSE();
int WIFI_START_OK_RESPONSE();
int MY_STRLENGTH(char str[]);
int RESULT_RESPONSE();






