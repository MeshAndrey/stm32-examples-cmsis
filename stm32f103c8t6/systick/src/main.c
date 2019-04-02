#include "stm32f103xb.h"

#define F_CPU 		8000000UL		// CPU frequency
#define TimerTick	F_CPU/1000-1

volatile uint32_t time;
volatile uint8_t  flag;

void SysTick_Handler(void);

void init_gpio(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	// Clear PA1 and PA2 control register bits
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	// Configure PA1 and PA2 as Push Pull output at max 10Mhz
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

void SysTick_init(void)
{
	SysTick->LOAD = TimerTick;
	SysTick->VAL  = TimerTick;
	SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

int main(void)
{
	init_gpio();
    SysTick_init();
    
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