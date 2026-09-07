/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "LCD.h"
#include "WIFI.h"
#include "RTC.h"
#include "EEPROM.h"
#include "SET_TIME_AND_DATE.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ADC_DONE_FLAG       (1U << 0)

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart6;

/* Definitions for Task_1 */
osThreadId_t Task_1Handle;
const osThreadAttr_t Task_1_attributes = {
  .name = "Task_1",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_2 */
osThreadId_t Task_2Handle;
const osThreadAttr_t Task_2_attributes = {
  .name = "Task_2",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_3 */
osThreadId_t Task_3Handle;
const osThreadAttr_t Task_3_attributes = {
  .name = "Task_3",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_4 */
osThreadId_t Task_4Handle;
const osThreadAttr_t Task_4_attributes = {
  .name = "Task_4",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_5 */
osThreadId_t Task_5Handle;
const osThreadAttr_t Task_5_attributes = {
  .name = "Task_5",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};
/* USER CODE BEGIN PV */

volatile int TEMP_value=0;
volatile int TEMP=0;

volatile int ERROR_WIFI;
volatile int ret=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART6_UART_Init(void);
void StartTask_RTC_1(void *argument);
void StartTask_ADC_2(void *argument);
void StartTask_WIFI_3(void *argument);
void StartTask_EEPROM_4(void *argument);
void StartTask_DATE_TIME_CONFIGURE5(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

  KM_LCD_INIT();
   KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
         KM_LCD_WRITE_STR("WELCOME TO");
  			 KM_LCD_WRITE_CMD(0xC0);               //PRINTF SECOND LINE
         KM_LCD_WRITE_STR(" S W M W V3.O ");
  	   	 HAL_Delay(1000);


 /*  RTC_SetTimeDate(4, 1, 00,
                   5,
                   14, 8, 26);*/

/********************** DEVICE CHECKING *********************************************************/
  	   		   //IS WIFI FOUND

  	   							 ret=IS_WIFI_FOUND();
  	   		           if(ret==WIFI_NOT_FOUND)//-1 == -1  //CONDITON TRUE
  	   					  {
  	   						 KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
  	   		                 KM_LCD_WRITE_STR("WIFI NOT FOUND");
  	   						 KM_LCD_WRITE_CMD(0xC0);              //PRINT SECOND LINE
  	   		                 KM_LCD_WRITE_STR("RESET BOARD");
  	   						 ERROR_WIFI=1;
  	   									 // while(1);
  	   						  }
  	   					 KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
  	   					 KM_LCD_WRITE_STR("AT OK");
  	   					 KM_LCD_WRITE_CMD(0xC0);               //PRINTF SECOND LINE
  	   		            KM_LCD_WRITE_STR("WIFI FOUND");
  	   			   	    HAL_Delay(1000);

/************************* IS WIFI CONNECTED ************************************************************/

  	   					     ret=IS_WIFI_CONNECTED();
  	   						   if(ret==WIFI_CONNECTED)//0 == 0  //CONDITON TRUE
  	   				       KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN
  	   				     	 KM_LCD_WRITE_STR("WIFI CONNECTED");
  	   							 	HAL_Delay(2000);
  	   								 KM_LCD_WRITE_CMD(0x01);               //CLEAR LCD SCREEN

  	   			 KM_LCD_WRITE_CMD(0x8E);
  	   			 KM_LCD_WRITE_DATA(0xdf);       // Print custom degree symbol
  	   			KM_LCD_WRITE_DATA('C');     // Print C

/**********************************************************************************************************/


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 0, &myBinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_1 */
  Task_1Handle = osThreadNew(StartTask_RTC_1, NULL, &Task_1_attributes);

  /* creation of Task_2 */
  Task_2Handle = osThreadNew(StartTask_ADC_2, NULL, &Task_2_attributes);

  /* creation of Task_3 */
  Task_3Handle = osThreadNew(StartTask_WIFI_3, NULL, &Task_3_attributes);

  /* creation of Task_4 */
  Task_4Handle = osThreadNew(StartTask_EEPROM_4, NULL, &Task_4_attributes);

  /* creation of Task_5 */
  Task_5Handle = osThreadNew(StartTask_DATE_TIME_CONFIGURE5, NULL, &Task_5_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D0_Pin LCD_D1_Pin LCD_D2_Pin LCD_D3_Pin
                           LCD_RS_Pin LCD_RW_Pin LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : UP_SWITCH_PC8_Pin DN_SWITCH_PC9_Pin ENT_SWITCH_PC10_Pin */
  GPIO_InitStruct.Pin = UP_SWITCH_PC8_Pin|DN_SWITCH_PC9_Pin|ENT_SWITCH_PC10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc)
{
	  if (hadc->Instance == ADC1)
	    {
	        osSemaphoreRelease(myBinarySem01Handle);
	    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin==UP_SWITCH_PC8_Pin)
	{
		osThreadFlagsSet(Task_5Handle,UP_BUTTON_FLAG);
	}

	else if(GPIO_Pin==DN_SWITCH_PC9_Pin)
	{
		osThreadFlagsSet(Task_5Handle, DOWN_BUTTON_FLAG);
	}

	else if(GPIO_Pin==ENT_SWITCH_PC10_Pin)
	{
		osThreadFlagsSet(Task_5Handle,ENTER_BUTTON_FLAG);
	}

}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTask_RTC_1 */
/**
  * @brief  Function implementing the Task_1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_RTC_1 */
void StartTask_RTC_1(void *argument)
{
  /* USER CODE BEGIN 5 */
    RTC_Data_t rtc;
  /* Infinite loop */
  for(;;)
  {
	  if(RTC_Read(&rtc) == HAL_OK)
	         {
	             osMutexAcquire(myMutex01Handle, osWaitForever);

	             char time[17];
	             char date[17];

	             sprintf(time,
	                     "%02d:%02d:%02d",
	                     rtc.hours,
	                     rtc.minutes,
	                     rtc.seconds);

	             sprintf(date,
	                     "%02d-%02d-20%02d",
	                     rtc.date,
	                     rtc.month,
	                     rtc.year);

	             KM_LCD_WRITE_CMD(0x80);
	             KM_LCD_WRITE_STR(time);

	             KM_LCD_WRITE_CMD(0xC0);
	             KM_LCD_WRITE_STR(date);

	             osMutexRelease(myMutex01Handle);
	         }
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask_ADC_2 */
/**
* @brief Function implementing the Task_2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_ADC_2 */
void StartTask_ADC_2(void *argument)
{
  /* USER CODE BEGIN StartTask_ADC_2 */
	volatile int temp=0;
  /* Infinite loop */
  for(;;)
  {
	  HAL_ADC_Start_IT(&hadc1); //SWATART BIT ENABLE

	//  osThreadFlagsWait(ADC_DONE_FLAG, osFlagsWaitAny,osWaitForever);
	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);

		temp=HAL_ADC_GetValue(&hadc1);
         TEMP_value=(temp*0.08);

	  			 osMutexAcquire(myMutex01Handle, osWaitForever);//LOCK using MUTEX
	  			 KM_LCD_WRITE_CMD(0x8C);
	  			 KM_ITOA_STR(TEMP_value);
	  			 osMutexRelease(myMutex01Handle);      //UNCLOCK using MUTEX

	  			 osMessageQueuePut(myQueue01Handle, &TEMP_value, 0, 200);

    osDelay(5000);//every 5 seconds read temp value from ADC
  }
  /* USER CODE END StartTask_ADC_2 */
}

/* USER CODE BEGIN Header_StartTask_WIFI_3 */
/**
* @brief Function implementing the Task_3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_WIFI_3 */
void StartTask_WIFI_3(void *argument)
{
  /* USER CODE BEGIN StartTask_WIFI_3 */
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(myQueue01Handle, &TEMP, NULL, osWaitForever);

	//  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
	      KM_WIFI_OPERATION();
	     ret=KM_WIFI_PUSH_SERVER();
	//  osSemaphoreRelease(myBinarySem01Handle);


		if(ret==ERROR)
		{
		    osMutexAcquire(myMutex01Handle, osWaitForever);//LOCK using MUTEX
			  KM_LCD_WRITE_CMD(0xCB);          //CLEAR LCD SCREEN
	          KM_LCD_WRITE_STR("ERROR");
	   	   osMutexRelease(myMutex01Handle);      //UNCLOCK using MUTEX

	 		 ERROR_WIFI=1;
		}

		else
			{
		      osMutexAcquire(myMutex01Handle, osWaitForever);//LOCK using MUTEX
			      KM_LCD_WRITE_CMD(0xC9);          //CLEAR LCD SCREEN
	              KM_LCD_WRITE_STR("   OK    ");
	    	  osMutexRelease(myMutex01Handle);      //UNCLOCK using MUTEX

			}


    osDelay(1);
  }
  /* USER CODE END StartTask_WIFI_3 */
}

/* USER CODE BEGIN Header_StartTask_EEPROM_4 */
/**
* @brief Function implementing the Task_4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_EEPROM_4 */
void StartTask_EEPROM_4(void *argument)
{
  /* USER CODE BEGIN StartTask_EEPROM_4 */
  /* Infinite loop */
  for(;;)
  {
	if(ERROR_WIFI==1) //ANY ERROR CASE WIFI
	   {
		 osMutexAcquire(myMutex01Handle, osWaitForever);//LOCK using MUTEX
	         KM_LCD_WRITE_CMD(0xCA);               //CLEAR LCD SCREEN
             KM_LCD_WRITE_STR("EP");
		 osMutexRelease(myMutex01Handle);      //UNCLOCK using MUTEX

    	  EEPROM_Write_RTC_Temperature();
	      ERROR_WIFI=0;
	  }
    osDelay(1);
  }
  /* USER CODE END StartTask_EEPROM_4 */
}

/* USER CODE BEGIN Header_StartTask_DATE_TIME_CONFIGURE5 */
/**
* @brief Function implementing the Task_5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_DATE_TIME_CONFIGURE5 */
void StartTask_DATE_TIME_CONFIGURE5(void *argument)
{
  /* USER CODE BEGIN StartTask_DATE_TIME_CONFIGURE5 */
    RTC_Data_t rtc;
    uint32_t flags;

  /* Infinite loop */
  for(;;)
  {

	  flags = osThreadFlagsWait(
	                    UP_BUTTON_FLAG |
	                    DOWN_BUTTON_FLAG |
	                    ENTER_BUTTON_FLAG,
	                    osFlagsWaitAny,
	                    osWaitForever);

            if (flags & ENTER_BUTTON_FLAG)
            {
            	 osThreadSuspend(Task_1Handle);
            			       osThreadSuspend(Task_2Handle);
            			       osThreadSuspend(Task_3Handle);
            			       osThreadSuspend(Task_4Handle);

            				 osMutexAcquire(myMutex01Handle, osWaitForever);//LOCK using MUTEX
            			       RTC_CONFIG();
            				osMutexRelease(myMutex01Handle);      //UNCLOCK using MUTEX

            				            osThreadResume(Task_1Handle);
            				            osThreadResume(Task_2Handle);
            				            osThreadResume(Task_3Handle);
            				            osThreadResume(Task_4Handle);
            }
            osDelay(1);

  }
  /* USER CODE END StartTask_DATE_TIME_CONFIGURE5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
