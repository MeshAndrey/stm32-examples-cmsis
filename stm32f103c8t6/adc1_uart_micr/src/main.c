#include "stm32f103xb.h"
#include <stdio.h>
#include <string.h>

#define F_CPU        8000000UL
#define TimerTick    F_CPU/1000-1
#define TEXT_BUF     16

volatile uint32_t counter = 0;
volatile uint32_t delay_time = 0;
             char text[TEXT_BUF];


void SysTick_Handler(void);

void SysTick_init(void)
{
    SysTick->LOAD = TimerTick;
    SysTick->VAL  = TimerTick;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void init_gpio_led(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

void init_gpio_uart(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;    // GPIOA Clock ON. Alter function clock ON

    GPIOA->CRH   &= ~GPIO_CRH_CNF9;                             // Clear CNF bit 9
    GPIOA->CRH   |= GPIO_CRH_CNF9_1;                            // Set CNF bit 9 to 10 - AFIO Push-Pull
    GPIOA->CRH   |= GPIO_CRH_MODE9_0;                           // Set MODE bit 9 to Mode 01 = 10MHz

    GPIOA->CRH   &= ~GPIO_CRH_CNF10;                            // Clear CNF bit 10
    GPIOA->CRH   |= GPIO_CRH_CNF10_0;                           // Set CNF bit 10 to 01 = HiZ
    GPIOA->CRH   &= ~GPIO_CRH_MODE10;                           // Set MODE bit 10 to Mode 01 = 10MHz
}

void init_gpio_adc(void)
{
    GPIOA->CRL &= ~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);	        // set PA1 to analog input for ADC
    GPIOA->CRL &= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);
}


void init_uart(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                       // USART1 Clock ON

    USART1->BRR   = 0x341;                                      // Baudrate for 9600 on 8Mhz
    USART1->CR1  |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;	// USART1 ON, TX ON, RX ON
}

void init_adc(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                         // ADC1 Clock ON

    ADC1->CR2  &= ~ADC_CR2_ALIGN;                               // Right Align
    ADC1->SQR1 &= ~(ADC_SQR1_L_0 | ADC_SQR1_L_1 
                                 | ADC_SQR1_L_2 | ADC_SQR1_L_3);// Number of regular conversions - 1
    ADC1->SQR3 |= ADC_SQR3_SQ1_0;                               // First conversion - channel 1

    ADC1->CR2 |= ADC_CR2_SWSTART;                               // Start regular conversion
}

uint16_t get_adc_result()
{
    uint16_t result = 0;

    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_ADON;

    while ((ADC1->SR & ADC_SR_EOC) == 0);                       // wait for end of conversion bit is set by hardware

    result = ADC1->DR;                                          // read result

    ADC1->CR2 &= ~ADC_CR2_ADON;	                                // Disable ADC1 for lower power //consuption

    return result;
}

// void putchar(char c) //send char to uart
// {
//     while(!(USART1->SR & USART_SR_TC)); // wait for data register of uart1 is empty
//     USART1->DR = c;
// }

void send_byte_to_uart(uint8_t data) 
{
    while(!(USART1->SR & USART_SR_TC)); // waiting for bit TC in register SR is 1

    USART1->DR = data;                  // send byte throught uart
}

void send_text_to_uart(char array[])
{
    for(uint8_t i = 0; i < strlen(array); i++)
        send_byte_to_uart(array[i]);
}

int main(void)
{
    uint16_t adc_result = 0;

    init_gpio_led();
    init_gpio_uart();
    init_gpio_adc();

    SysTick_init();

    init_adc();
    init_uart();


    while(1)
    {
        if (delay_time == 0)
        {
            adc_result = get_adc_result();
            sprintf(text, "%d\r\n", (int)adc_result);
            send_text_to_uart(text);

            //counter++;

            GPIOC->ODR ^= GPIO_ODR_ODR13;

            delay_time = 1;
        }
    }
}

void SysTick_Handler(void)
{
    if (delay_time > 0)
        delay_time--;
}