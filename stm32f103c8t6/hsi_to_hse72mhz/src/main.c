#include "stm32f103xb.h"

#define HSEStartUp_TimeOut 1000
#define RESET              0
#define SET                1

void delay(uint32_t time)
{
    while(time--)
    {
        __asm
        (
            "nop"
        );
    }

}

void init_gpio(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN; // Disable JTAG
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Clear PC13 control register bits
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    // Configure PC13 as Push Pull output at max 10Mhz
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
}

void init_rcc(void)
{
    volatile uint32_t StartUpCounter = 0, HSEStatus = 0; // timeout counter for HSE

    RCC->CR |= RCC_CR_HSEON;

    do 
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;  
    }
    while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));


    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    /* if HSE starting normally */
    if (HSEStatus == (uint32_t)0x01)
    {
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);
 
        //RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
 
        
        RCC->CR |= RCC_CR_PLLON;
 
        
        while((RCC->CR & RCC_CR_PLLRDY) == 0) 
        {

        }

        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) 
        {

        }
    }
    else 
    {
        /* Error handler */
    }
}


int main(void)
{
    init_rcc();
    init_gpio();

    while(1)
    {
        volatile uint32_t time = 10000000;
        
        GPIOC->BSRR = GPIO_BSRR_BS13;  

        delay(time);

        GPIOC->BSRR = GPIO_BSRR_BR13;

        delay(time);
    }
}
