#include "stm32f103xb.h"

void delay(uint32_t time)
{
    while(time--)
	    __asm 		// inline asm - syntax for gcc with c99
	    (
	  	    "nop"				  
	    );
}

int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

	// Clear PA1 and PA2 control register bits
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);

	// Configure PA1 and PA2 as Push Pull output at max 10Mhz
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

	while(1)
	{
		uint32_t time = 1000000;	
		
		GPIOC->BSRR = GPIO_BSRR_BS13;
		delay(time);
		
		GPIOC->BSRR = GPIO_BSRR_BR13;
		delay(time);
	}
}
