 #include "../inc/stm32f401rbt6.h"
 #include "../inc/GPIO.h"
 #include "../inc/CONTROLLING.h"

/*==============================================================
                        FRONT FUNCTION
											  Motor rotates in forward direction
==============================================================*/
void front(void)
{
	  GPIOC_ODR &= ~(0x0F);
    GPIOC_ODR |= (1 << 1);  // Make PC1 HIGH													
		 GPIOC_ODR |= (1 << 3);   //PC3 HIGH	
	
	 TIM1_CCR2 = 15000;   // ENA
    TIM1_CCR3 = 15000;   // ENB
	
}
/*==============================================================
                        BACK FUNCTION
 Motor rotates in reverse direction
==============================================================*/
void back(void)
{
	 GPIOC_ODR &= ~(0x0F);
    GPIOC_ODR |= (1 << 0);   //Make PC0 HIGH
		GPIOC_ODR |=(1 << 2);   // Make PC2 HIGH
	
	 TIM1_CCR2 = 15000;   // ENA
    TIM1_CCR3 = 15000;   // ENB
	 
}
/*==============================================================
                        LEFT FUNCTION
// Vehicle turns left
================= SOFT LEFT ================= */
void left(void)
{
    /* both motors forward */

    GPIOC_ODR &= ~(0x0F);

    GPIOC_ODR |= (1 << 1);   // Left motor forward
    GPIOC_ODR |= (1 << 3);   // Right motor forward

    /* Different speeds */

    TIM1_CCR2 = 2000;        // Left motor 40%
    TIM1_CCR3 = 16000;       // Right motor 80%
}

/* ================= SOFT RIGHT ================= */

void right(void)
{
    /* both motors forward */

    GPIOC_ODR &= ~(0x0F);

    GPIOC_ODR |= (1 << 1);   // Left motor forward
    GPIOC_ODR |= (1 << 3);   // Right motor forward

    /* Different speeds */

    TIM1_CCR2 = 16000;       // Left motor 80%
    TIM1_CCR3 = 2000;        // Right motor 40%
}
/*==============================================================
                        STOP FUNCTION
					 // Stop all motors
==============================================================*/
void stop(void)
{
    GPIOC_ODR &= ~0x0F;                // Clear lower 4 bits PC0 PC1 PC2 PC3                                      
}
/*==============================================================
                    SERVO OPEN FUNCTION
						  // Servo rotates to open position
==============================================================*/

/*CCR1 = 1000 + (Angle × 1000) / 180
 
    180 degree  1000+(180×1000)/180

                =2000
*/
void Servo_Open(void)
{
    TIM1_CCR1 = 2000;   //  Set PWM compare value to 2500                                    
}
/*==============================================================
                    SERVO CLOSE FUNCTION
					    // Servo rotates to close position
==============================================================*/
void Servo_Close(void)
{
	/*
	 0 degree  1000+(0×1000)/180
              =1000
	*/
    TIM1_CCR1 = 1000;                  // Set PWM compare value to 1000
                                  
}

/*==============================================================
                    MOTOR SPEED FUNCTION
==============================================================*/
void Motor_Speed(int speed)
{
    TIM1_CCR2 = speed;   // ENA
    TIM1_CCR3 = speed;   // ENB
}


/*==============================================================
                    PWM INIT
==============================================================*/
void PWM_Init(void)
{
    RCC_AHB1ENR |= (1 << 0);      // Bit0 (GPIOAEN)
                                  // Enable clock for GPIO Port A

    RCC_APB2ENR |= (1 << 0);      // Bit0 (TIM1EN)
                                  // Enable clock for TIM1


/*=========================================================
      Configure PA8, PA9, PA10 as Alternate Function Mode
=========================================================*/

    GPIOA_MODER &= ~(0x3F << 16); // Clear MODER bits for PA8 PA9 PA10
                                  // PA8 -> MODER8[1:0]
                                  // PA9 -> MODER9[1:0]
                                  // PA10-> MODER10[1:0]

    GPIOA_MODER |= (0x2A << 16);  // Set Alternate Function Mode (10)
                                  // PA8  = AF
                                  // PA9  = AF
                                  // PA10 = AF
/*=========================================================
      Select Alternate Function AF1 (TIM1)
=========================================================*/

    GPIOA_AFRH &= ~(0xFFF);       // Clear AF bits
                                  // AFRH[3:0]   -> PA8
                                  // AFRH[7:4]   -> PA9
                                  // AFRH[11:8]  -> PA10

    GPIOA_AFRH |= (0x111);        // AF1 = TIM1
                                  // PA8  -> TIM1_CH1
                                  // PA9  -> TIM1_CH2
                                  // PA10 -> TIM1_CH3


/* ----------------------------------
       Timer Clock = 16MHz

       PWM frequency:
       16MHz / 16 = 1MHz
	   
	     prescalar input 16mhz output is 1MHZ
-----------------------------------*/

    TIM1_PSC = 16 - 1;
		
	/*------------------------------------
	Servo Motor Working

     A servo motor requires a PWM signal.

     PWM Frequency = 50 Hz (20 ms period)
     0° = 1 ms pulse
     90° = 1.5 ms pulse
     180° = 2 ms pulse	

  ---------------------------------------*/
    TIM1_ARR = 20000 - 1;
		
/*=========================================================
      Channel 1 Configuration (Servo)
=========================================================*/

    TIM1_CCMR1 |= (6 << 4);       // OC1M[2:0] = 110
                                  // Select PWM Mode 1
                                  // Output HIGH while CNT < CCR1

    TIM1_CCMR1 |= (1 << 3);       // OC1PE = 1
                                  // Enable CCR1 preload
                                  // New CCR1 value updates
                                  // at next update event
/*=========================================================
      Channel 2 Configuration (Motor A)
=========================================================*/

    TIM1_CCMR1 |= (6 << 12);      // OC2M[2:0] = 110
                                  // PWM Mode 1 for Channel 2

    TIM1_CCMR1 |= (1 << 11);      // OC2PE = 1
                                  // Enable CCR2 preload



/*=========================================================
      Channel 3 Configuration (Motor B)
=========================================================*/

    TIM1_CCMR2 |= (6 << 4);       // OC3M[2:0] = 110
                                  // PWM Mode 1 for Channel 3

    TIM1_CCMR2 |= (1 << 3);       // OC3PE = 1
                                  // Enable CCR3 preload


/*=========================================================
      Enable PWM Outputs
=========================================================*/

    TIM1_CCER |= (1 << 0);        // CC1E = 1
                                  // Enable Channel 1 output
                                  // Servo PWM appears on PA8

    TIM1_CCER |= (1 << 4);        // CC2E = 1
                                  // Enable Channel 2 output
                                  // Motor PWM appears on PA9

    TIM1_CCER |= (1 << 8);        // CC3E = 1
                                  // Enable Channel 3 output
                                  // Motor PWM appears on PA10



/*=========================================================
      Advanced Timer Main Output Enable
=========================================================*/

    TIM1_BDTR |= (1 << 15);       // MOE = Main Output Enable
                                  // Mandatory for TIM1/TIM8
                                  // Without MOE PWM will NOT
                                  // appear on output pins


/*=========================================================
      Generate Update Event
=========================================================*/

    TIM1_EGR |= (1 << 0);         // UG = Update Generation
                                  // Immediately load PSC
                                  // ARR
                                  // CCR preload registers
/*=========================================================
      Enable Auto Reload Preload
=========================================================*/

    TIM1_CR1 |= (1 << 7);         // ARPE = Auto Reload Preload Enable
                                  // ARR value is buffered
                                  // Updated safely on next overflow


/*=========================================================
      Start Timer
=========================================================*/

    TIM1_CR1 |= (1 << 0);         // CEN = Counter Enable
                                  // Timer starts counting
                                  // PWM generation begins
}

