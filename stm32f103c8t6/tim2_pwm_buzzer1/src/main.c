/**
 * This example shows the basics of using a PWM.
 * Using - LED - PC13 - toggles in main loop
 *          - LED - PA1  - toggles by PWM. 
 *     
 * Period of pulse is 1 sec.
 * Duty of pulse is 0.5 sec = 500 ms = 500000 us
 * This values are set up by timer's settings.
 * 
 */

#include "stm32f103xb.h"

#define F_CPU         8000000UL        // CPU frequency
#define TimerTick    F_CPU/1000-1

volatile uint32_t time;
volatile uint8_t  flag;

void SysTick_Handler(void);

void init_gpio(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
    
    /*LED PC13*/
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    /*PWM PA1*/
    
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;  // 10: Alternate function output Push-pull 
    GPIOA->CRL |= GPIO_CRL_CNF1_1;

    GPIOA->CRL |= GPIO_CRL_MODE1_1;     // 10: Output mode, max speed 2 MHz.
    GPIOA->CRL &= ~GPIO_CRL_MODE1_0;
}

void init_tim2(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 8 - 1;
    TIM2->ARR = 100;
    TIM2->CNT = 0;

    TIM2->CCR2 = 50; // duty cycle of PWM
    
    TIM2->DIER |= TIM_DIER_UIE; //Bit 0 UIE: Update interrupt enable
    TIM2->CR1 |= TIM_CR1_CEN; 

    TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // PWM mode 2
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE; // enable the corresponding preload register
    TIM2->CR1 |= TIM_CR1_ARPE;        // enable the auto-reload preload register

    TIM2->EGR |= TIM_EGR_UG;     // initialize all the registers
    TIM2->CCER |= TIM_CCER_CC2P; // set polarity as 1
    TIM2->CCER |= TIM_CCER_CC2E; // enable output

    __enable_irq(); 
    NVIC_EnableIRQ (TIM2_IRQn); // enable TIM interrupt 
    NVIC_SetPriority(TIM2_IRQn, 0);
}

void SysTick_init(void)
{
    SysTick->LOAD = TimerTick;
    SysTick->VAL  = TimerTick;
    SysTick->CTRL =    SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

int main(void)
{
    init_gpio();
    SysTick_init();
    init_tim2();
    
    time = 0;
    flag = 0;

    while(1)
    {
        if (time == 0 && flag == 0)
        {
            GPIOC->BSRR = GPIO_BSRR_BS13;
            time = 1000;
            flag = 1;
        }

        if (time == 0 && flag == 1)    
        {
            GPIOC->BSRR = GPIO_BSRR_BR13;
            time = 1000;
            flag = 0;
        }    
    }
}

void SysTick_Handler(void)
{
    time--;
}

void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
}