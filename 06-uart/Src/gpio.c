#include "stm32f030x8.h"

void GPIO_init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
	GPIOA->MODER |=  (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

    GPIOA->AFR[0] &= ~((0xF << GPIO_AFRL_AFSEL2_Pos) | (0xF << GPIO_AFRL_AFSEL3_Pos));
    GPIOA->AFR[0] |=  ((1 << GPIO_AFRL_AFSEL2_Pos)  | (1 << GPIO_AFRL_AFSEL3_Pos));
}

void LED_toggle(void)
{
    GPIOA->ODR ^= GPIO_ODR_5;
}