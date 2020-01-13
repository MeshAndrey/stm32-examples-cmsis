/**
  ******************************************************************************
  * @file    uart_tx/src/main.c 
  * @author  Meshcheryakov Andrey
  * @version V0.0.1
  * @date    29-October-2019
  * @brief   This code example shows how to use UART as transmitter.
  *
 ===============================================================================
                    #####       MCU Resources     #####
 ===============================================================================
   - RCC
   - SysTick
   - USART1
   - GPIO:
               PC13 - LED
            PA9  - MCU's UART Tx
            PA10 - MCU's UART Rx - Not used in this example

===============================================================================
                    #####       Another Resources     #####
===============================================================================
   - UART to USB adapter (CP2102 as example)
   - Use terminal program to see what is being received.
     Use picocom, PuTTY, etc.
   
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
   - UART is sending data every second. So you need to use terminal program.
     For picocom:
         $ sudo picocom /dev/ttyUSB0 -b9600
     Where /dev/ttyUSB0 - full path to device. It may be different from this.
           -b9600       - parameter of baudrate. This example uses 9600 baud.
     sudo permission is needed. 
   - Watch on received data.
  *    
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f103xb.h"
#include <stdio.h>
#include <string.h>

/* End of includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define F_CPU         8000000UL
#define TimerTick    F_CPU/1000-1
#define TEXT_BUF    16
/* End of private defines ----------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t counter = 0;
volatile uint32_t delay_time = 0;
              char text[TEXT_BUF];

/* End of private variables --------------------------------------------------*/

/* Function ptototypes -------------------------------------------------------*/
void SysTick_Handler(void);
/* End of function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

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
    SysTick->CTRL =    SysTick_CTRL_CLKSOURCE_Msk | 
                      SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  This function :
  *          - Configures the Green LED pin on GPIO PC13.
  * @param  None
  * @retval None
  */
static void 
init_gpio_led(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

/**
  * @brief  This function :
  *          - Configures PA9 && PA10 for UART.
  * @param  None
  * @retval None
  */
static void 
init_gpio_uart(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;     // GPIOA Clock ON. Alter function clock ON

    GPIOA->CRH     &= ~GPIO_CRH_CNF9;                             // Clear CNF bit 9
    GPIOA->CRH     |= GPIO_CRH_CNF9_1;                            // Set CNF bit 9 to 10 - AFIO Push-Pull
    GPIOA->CRH     |= GPIO_CRH_MODE9_0;                            // Set MODE bit 9 to Mode 01 = 10MHz

    GPIOA->CRH     &= ~GPIO_CRH_CNF10;                            // Clear CNF bit 9
    GPIOA->CRH     |= GPIO_CRH_CNF10_0;                            // Set CNF bit 9 to 01 = HiZ
    GPIOA->CRH     &= ~GPIO_CRH_MODE10;                            // Set MODE bit 9 to Mode 01 = 10MHz
}

/**
  * @brief  This function :
  *          - Configures UART.
  * @param  None
  * @retval None
  */
static void
init_uart(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                        // USART1 Clock ON
    USART1->BRR   = 0x341;                                        // Baudrate for 9600 on 8Mhz
    USART1->CR1  |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;    // USART1 ON, TX ON, RX ON                        
}

/**
  * @brief  This function :
  *          - Transmitts byte through UART.
  * @param  uint8_t byte to send
  * @retval None
  */
static void 
send_byte(uint8_t data) 
{
    while(!(USART1->SR & USART_SR_TC)); // waiting for bit TC in register SR is 1

    USART1->DR = data;                     // send byte throught uart
}

/**
  * @brief  This function :
  *          - Organizes transmitting of char array through UART.
  * @param  char[] array to tx (text)
  * @retval None
  */
static void 
send_text(char array[])
{
    for(uint8_t i = 0; i < strlen(array); i++)
        send_byte(array[i]);
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
    init_gpio_led();
    init_gpio_uart();

    init_SysTick();
    init_uart();

    while(1)
    {                
        if (delay_time == 0)
        {
            sprintf(text, "%d\r\n", (int)counter);            
            send_text(text);
            counter++;

            GPIOC->ODR ^= GPIO_ODR_ODR13;

            delay_time = 1000;
        }
    }
}

/**
  * @brief  SysTick interrupt handler.
  *         This function is being called every ms (millisecond).
  * @param  None
  * @retval None
  */
void 
SysTick_Handler(void)
{
    if (delay_time > 0)
        delay_time--;
}

/****************************** END OF FILE ***********************************/
