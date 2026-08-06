#include "stm32f030x8.h"

int main(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    while(1)
    {
        GPIOA->BSRR = GPIO_BSRR_BS_5;

        for (uint32_t i = 0; i < 1000000; i++);

        GPIOA->BSRR = GPIO_BSRR_BR_5;

        for (uint32_t i = 0; i < 1000000; i++);
    }

}
