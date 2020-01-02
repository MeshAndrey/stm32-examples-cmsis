/**
  ******************************************************************************
  * @file    gpio_exti/src/main.c 
  * @author  Meshcheryakov Andrey
  * @version V0.0.3
  * @date    02-January-2020
  * @brief   This code example shows how to use EXTI.
  *
 ===============================================================================
                    #####       MCU Resources     #####
 ===============================================================================
   - RCC
   - GPIO:
          PA0  - EXTI
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
   TODO : Write description
          Write comments
          Fix codestyle

  *    
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
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


void
configure_externalIT(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    EXTI->IMR = 0x0001;
    EXTI->RTSR = 0x0001;
    EXTI->FTSR = 0x0001;

    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn, 0);
}

void EXTI0_IRQHandler(void)
{
    if ((EXTI->PR & 0x0001) != 0)  /* Check line 0 has triggered the IT */
    {
        EXTI->PR |= 0x0001;        /* Clear the pending bit */
        GPIOC->ODR ^= (1 << 13);   /* Toggle green led on PC13 */
    }
}

/**
  * @brief  This function :
             - Makes simple delay.
            This function is only for educational purposes.
            Do not use it another time. 
  * @param  uint32_t delay time
  * @retval None
  */
void 
delay(uint32_t time)
{
    while(time--)
        __asm         // inline asm - syntax for gcc with c99
        (
              "nop"                  
        );
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int 
main(void)
{
    configure_gpio();
    configure_externalIT();

    GPIOC->ODR |= (1 << 13); 

    /* Infinite loop */
    while(1)
    {
    }
}

/****************************** END OF FILE ***********************************/
