/**
  ******************************************************************************
  * @file    blink_led/src/main.c 
  * @author  Meshcheryakov Andrey
  * @version V0.0.1
  * @date    23-September-2019
  * @brief   This code example shows how to configure the GPIOs.
  *
 ===============================================================================
                    #####       MCU Resources     #####
 ===============================================================================
   - RCC
   - GPIO:
   		    PC13 - LED
   
 ===============================================================================
                    ##### How to use this example #####
 ===============================================================================
   - this file must be inserted in a project containing  the following files :
      o system_stm32f1xx.c, startup_stm32f103xb.s
      o stm32f1xx.h to get the register definitions
      o CMSIS files

 ===============================================================================
                    ##### How to test this example #####
 ===============================================================================
   - Compile this code.
   - Flash target device.
   - LED is blinking.

  *    
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f103xb.h"

/* End of includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  This function :
             - Configures the Green LED pin on GPIO PC13.
  * @param  None
  * @retval None
  */
void
configure_gpio(void)
{
	/* Enable power for GPIO PORT C */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	/* Clear PA1 and PA2 control register bits */
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);

	/* Configure PA1 and PA2 as Push Pull output at max 10Mhz */
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

/**
  * @brief  This function :
  *          - Makes simple delay.
  *         This function is only for educational purposes.
  *         Do not use it in real application. 
  * @param  uint32_t delay time
  * @retval None
  */
void 
delay(uint32_t time)
{
    while(time--)
	    __asm 		// inline asm - syntax for gcc with c99
	    (
	  	    "nop"				  
	    );
}

/**
  * @brief  Main program.
  *         User program starts from here.
  * @param  None
  * @retval None
  */
int
main(void)
{
	configure_gpio();

	/* Infinite loop */
	while(1)
	{
		uint32_t time = 1000000;	
		
		/* Set bit 13 of Port C bit set/reset register */
		GPIOC->BSRR = GPIO_BSRR_BS13;
        /* Some delay */
		delay(time);
		
	    /* Reset bit 13 of Port C bit set/reset register */
		GPIOC->BSRR = GPIO_BSRR_BR13;
		/* Some delay*/
		delay(time);
	}
}

/****************************** END OF FILE ***********************************/
