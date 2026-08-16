#include "stm32f030x8.h"

void gpio_led_init(void) {
    /* Enable GPIOA Clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    /* Set Alternate Function mode (10) */
    GPIOA->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9 | GPIO_MODER_MODER10 | GPIO_MODER_MODER11);
    GPIOA->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1);

    /* Map PA8-PA11 to AF2 (TIM1_CH1 - TIM1_CH4) in AFRH (AFR[1]) */
    GPIOA->AFR[1] &= ~((0xFU << 0) | (0xFU << 4) | (0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[1] |= ((0x2U << 0) | (0x2U << 4) | (0x2U << 8) | (0x2U << 12));
}