#include "stm32f030x8.h"

#define HSI_VALUE ((uint32_t)8000000) /* Default value of the Internal oscillator in Hz. */

void SystemCoreClockUpdate(void);
void ClockSwitchToPLL48MHz(void);

uint32_t SystemCoreClock = HSI_VALUE; /* System Clock Frequency (Core Clock) */

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};   /*!< AHB prescalers table values */
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};    /*!< APB prescalers table values */

void SystemInit(void)
{
	ClockSwitchToPLL48MHz();
}


/* Updates the SystemCoreClock global variable by reading the current
 * clock configuration from RCC registers. This function should be called
 * whenever the system clock configuration changes.
*/
void SystemCoreClockUpdate(void)
{
    uint32_t sysclk_source = 0, pllmul = 0;

    /* read the current system clock source (HSI, HSE, PLL) */
    sysclk_source = (RCC->CFGR & RCC_CFGR_SWS);

    switch(sysclk_source)
    {
    	/* system clock is driven by Internal HSI oscillator */
        case RCC_CFGR_SWS_HSI:
            SystemCoreClock = HSI_VALUE;
            break;

		/* system clock is driven by the PLL */
        case RCC_CFGR_SWS_PLL:
        	/*
        	 * decode the PLLMUL bits
        	 * start bit is 18, shift right 18 times
        	 * x12 is stored as 10 (1010), add 2 to the decimal number
        	 */
            pllmul = ((RCC->CFGR & RCC_CFGR_PLLMUL) >> 18) + 2;

            /* PLL input is HSI divided by 2, then multiplied by PLLMUL */
            SystemCoreClock = (HSI_VALUE >> 1) * pllmul;
            break;

        case RCC_CFGR_SWS_HSE:
            SystemCoreClock = HSI_VALUE; // TODO : implement HSE
            break;

        default:
            SystemCoreClock = HSI_VALUE;
            break;
    }

    /* decode and apply AHB Prescaler SYSCLK -> HCLK */
    SystemCoreClock >>= AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
}

void ClockSwitchToPLL48MHz(void)
{
	/* Ensure HSI is enabled and ready */
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

    /* Disable PLL to configure it */
    RCC->CR &= ~RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY));

    /* Configure PLL multiplication factor (x12) */
    RCC->CFGR &= ~RCC_CFGR_PLLMUL;
    RCC->CFGR |= RCC_CFGR_PLLMUL12;

    /* Set PLL source to HSI/2 */
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;

    /* Set FLASH Latency to one wait state (24 MHz < SYSCLK < 48MHz) */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY;
    
    /* switch on prefetching */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Enable PLL back */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Set PLL as system clock*/
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    SystemCoreClockUpdate();
}

void ClockSwitchToHSI(void)
{
    /* Wait until stable HSI source */
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    /* switch the system clock to HSI */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    /* set FLASH latency to zero wait state */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;

    /* disable prefetching */
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;

    /* disable PLL */
    RCC->CR &= ~RCC_CR_PLLON;
    while (RCC->CR & RCC_CR_PLLRDY);

    SystemCoreClockUpdate();
}

uint32_t ClockGetHCLK(void)
{
    return SystemCoreClock;
}

uint32_t ClockGetPCLK(void)
{
    return SystemCoreClock >> APBPrescTable[((RCC->CFGR & RCC_CFGR_PPRE) >> 8)];
}