/**
  ******************************************************************************
  * @file    systick/src/main.c 
  * @author  Meshcheryakov Andrey
  * @version V0.0.1
  * @date    22-October-2019
  * @brief   This code example shows how to use system timer (SysTick) with IT.
  *
 ===============================================================================
                    #####       MCU Resources     #####
 ===============================================================================
   - RCC
   - SysTick
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
   - Green LED is inverting every second.

  *    
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f103xb.h"

/* End of includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define F_CPU 		8000000UL		// CPU frequency
#define TimerTick	F_CPU/1000-1
/* End of private defines ----------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t time;
volatile uint8_t  flag;

/* End of private variables --------------------------------------------------*/

/* Function ptototypes -------------------------------------------------------*/
void SysTick_Handler(void);
/* End of function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function :
  *          - Configures the Green LED pin on GPIO PC13.
  * @param  None
  * @retval None
  */
static void 
init_gpio(void)
{
    /* Enable power for GPIO PORT C */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
    /* Clear PC13 control register bits */
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);

    /* Configure PC13 as Push Pull output at max 10Mhz */
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

/**
  * @brief  This function :
  *          - Configures SysTick timer.
  * @param  None
  * @retval None
  */
static void 
init_SysTick(void)
{
	SysTick->LOAD = TimerTick;
	SysTick->VAL  = TimerTick;
	SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk
                                               | SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Main function.
  *         User program starts from here.
  * @param  None
  * @retval None
  */
int 
main(void)
{
	init_gpio();
    init_SysTick();
    
	time = 0;
	flag = 0;

    /* Infinite loop */
	while(1)
	{
		if (time == 0)
		{
			GPIOC->ODR ^= GPIO_BSRR_BS13; // invert bit 13 of Port C
			time = 1000;
		}
	}
}


/**
  * @brief  SysTick interrupt handler.
  *         This function is being called every ms (millisecond).
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	time--;
}

/****************************** END OF FILE ***********************************/
