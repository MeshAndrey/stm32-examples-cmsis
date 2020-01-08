#include "stm32f103xb.h"

#define F_CPU         8000000UL
#define TimerTick     F_CPU/1000-1

void TIM2_IRQHandler(void);

void
init_gpio(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN; 

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13); 
    GPIOC->CRH |= GPIO_CRH_MODE13_0;  
}

void
init_tim2(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 8000 - 1;
    TIM2->ARR = 1000;
    TIM2->CNT = 0;

    TIM2->CR1 |= TIM_CR1_ARPE;  //Bit 7 ARPE: Auto-reload preload enable
    //TIM2->CR1 |= TIM_CR1_DIR;

    TIM2->DIER |= TIM_DIER_UIE; //Bit 0 UIE: Update interrupt enable
    TIM2->CR1 |= TIM_CR1_CEN; 

    __enable_irq(); 
    NVIC_EnableIRQ (TIM2_IRQn); // enable TIM interrupt 
    NVIC_SetPriority(TIM2_IRQn, 0);
}

int
main(void)
{
    init_gpio(); 
    init_tim2();

    GPIOC->ODR |= GPIO_ODR_ODR13;

    while(1)
    {
    }
}

void
TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;

    GPIOC->ODR ^= GPIO_ODR_ODR13;
}