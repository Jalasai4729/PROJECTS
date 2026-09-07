/* QUESTION :
 * GPIO_MINIPROJECT 
 * 16*2 MONOCHROME LCD
 * Microcontroller : STM32F401RBT6
 * RaayanMini      : 
 * Author :JALA SAI
 * Date : 10-02-2026
 * STLink Debugger : 
 * Hardware        : 01)16*2 MONOCHROME LCD
 *
 *      PC0--->MOTOR 1
 *			PC1--->MOTOR 2
 *      PC2--->MOTOR 3
 *			PC3--->MOTOR 4
 *
 *
 *      PA6--->IR SENSOR
 *			PA7--->MQ3 SENSOR
 
 *      PA8--->PWM SIGNAL FOR SERVO MOTOR
 *      PA9  -> TIM1_CH2 -> ENA L288N MOTOR DRIVER
 *      PA10 -> TIM1_CH3 -> ENB L298N MOTOR DRIVER
 *
 *      PA2--->RX USART2
 *			PA3--->TX USART2
 *
 *
 *			PB0--->LCD_D4
 *			PB1--->LCD_D5
 *      PB2--->LCD_D6
 *      PB3--->LCD_D7
 *      PB4--->LCD_RS
 *      PB5--->LCD_RW
 *			PB8--->LCD_EN
 
 --------------------------------------------------------------
                STM32F401RBT6 FULL PROJECT
        Voice Controlled Car + LCD + MQ3 + IR
--------------------------------------------------------------
*/ 

#include "../inc/stm32f401rbt6.h"
#include "../inc/GPIO.h"
#include "../inc/lcd.h"
#include "../inc/systicktimer.h"
#include "../inc/SYSCFG.h"
#include "../inc/USART2.h"
#include "../inc/CONTROLLING.h"



volatile int object_flag = 0;
volatile int gas_flag = 0;

char voice[20]; // Array to store received voice command 
char data;      // Variable to store single UART character
int i = 0;

/* LCD home screen */
void lcd_car_running(void)
{
    KM_LCD_WRITE_CMD(0x01);
    KM_LCD_WRITE_STR("OBJECT CLEAR");
    KM_LCD_WRITE_CMD(0xC0);
    KM_LCD_WRITE_STR("CAR RUNNING...");
}
void lcd_car_stop(void)
{
	  KM_LCD_WRITE_CMD(0x01);
    KM_LCD_WRITE_STR("  CAR");
    KM_LCD_WRITE_CMD(0xC0);
    KM_LCD_WRITE_STR(" STOPPED...");
}

int main(void)
{
    /* GPIO clock enable */
    KM_GPIO_PORT_INIT(GPIOA);
    KM_GPIO_PORT_INIT(GPIOB);
    KM_GPIO_PORT_INIT(GPIOC);
    
	  GPIO_Init();
    /* Init peripherals */
    KM_STK_INIT();
    KM_PIN_LCD_INIT();
    KM_LCD_INIT();
    KM_SYSCFG_CONFIG();
	  PWM_Init();
	  KM_LCD_WRITE_CMD(0x01);
    KM_LCD_WRITE_STR("voice based Car");
    KM_LCD_WRITE_CMD(0xC0);
    KM_LCD_WRITE_STR("  controller");
    KM_USART2_INIT();
	  KM_USART2_CONFIG();
    
	  // Motor_Speed(15000);

    Servo_Close();


    while(1)
    {
			
			// IR obstacle detected 
       if(object_flag)
        {
            stop();

            KM_LCD_WRITE_CMD(0x01);
            KM_LCD_WRITE_STR("OBJECT DETECT");
            KM_LCD_WRITE_CMD(0xC0);
            KM_LCD_WRITE_STR("CAR STOPPED");

          //  KM_mdelay_IT(70);
					
					  if(GPIOA_IDR & (1 << 6))
             {
              object_flag = 0;
              lcd_car_running();
              }
		   	}

        // Gas detected 
        if(gas_flag)
        {
            stop();

            KM_LCD_WRITE_CMD(0x01);
            KM_LCD_WRITE_STR("GAS DETECTED");
            KM_LCD_WRITE_CMD(0xC0);
            KM_LCD_WRITE_STR("CAR STOPPED");

            KM_mdelay_IT(1500);

            gas_flag = 0;
            lcd_car_stop();
        }
			
			 // Receive one character from Bluetooth 
     data = KM_USART2_INCHAR_TIME_OUT();

       // Send received character back 
        KM_USART2_OUTCHAR(data);

				switch(data)
				{
            case 'F':
						case 'f':
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Forward");
						        front();
						        KM_mdelay_IT(3000);
                    break;
       
                case 'B':
                case 'b':									
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Backward");
								    back();
										KM_mdelay_IT(3000);

                    break;
               
                case 'L':
                case 'l':									
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Left");
								    left();
										KM_mdelay_IT(3000);

                    break;
  
                case 'R':
                case 'r':									
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Right");
								    right();
										KM_mdelay_IT(3000);

                    break;

                case 'S':
					      case 's':				
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Stop");
                    stop();
										KM_mdelay_IT(3000);

                    break;
								//key->lock
								case 'K':
				        case 'k':					
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("Lock");
										Servo_Close();
										KM_mdelay_IT(3000);

										break;
								case 'U':
		            case 'u':							
										KM_LCD_WRITE_CMD(0x01);
										KM_LCD_WRITE_STR("unLock");
								    Servo_Open();
										KM_mdelay_IT(3000);
										break;
            }
    }
	}
	

/* EXTI interrupt for PA6 and PA7 */
void EXTI9_5_IRQHandler(void)
{
    /* PA6 -> IR */
    if(EXTI_PR & (1 << 6))
    {
        object_flag = 1;
        EXTI_PR |= (1 << 6);
    }

    /* PA7 -> MQ3 */
    if(EXTI_PR & (1 << 7))
    {
        gas_flag = 1;
        EXTI_PR |= (1 << 7);
    }
}