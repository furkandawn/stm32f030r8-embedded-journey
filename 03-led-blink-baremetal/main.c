#include <stdint.h>

#define RCC_BASE 0x40021000UL
#define AHBENR_OFFSET 0x14UL
#define RCC_AHBENR (*(volatile uint32_t *)(RCC_BASE + AHBENR_OFFSET)) /* 0x40021014 */
#define RCC_AHBENR_GPIOAEN (1U << 17)

#define GPIOA_BASE 0x48000000UL
#define MODER_OFFSET 0x00UL
#define BSRR_OFFSET 0x18UL
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + MODER_OFFSET))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + BSRR_OFFSET))

/* Goes to .data segment (initialized global) in both RAM and FLASH */
uint32_t initialized_variable = 10;

/* Goes to .rodata segment in FLASH */
const uint32_t initialized_constant_variable = 16;

/* Goes to .bss segment (uninitialized global) in RAM */
uint32_t uninitialized_variable[64];

int main(void)
{
	initialized_variable++;
	uninitialized_variable[0] = 1;
	// Enable GPIOA clock gate by setting the IOPAEN bit high (17th bit of RCC_AHBENR register)
	RCC_AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure GPIOA PA5 in "general purpose output mode"
	GPIOA_MODER &= ~((1U << 10) | (1U << 11));
	GPIOA_MODER |= (1U << 10);

    /* Loop forever */
	for(;;){
		// Set PA5 pin high
		GPIOA_BSRR = (1U << 5);

		// crude busy-wait delay, duration depends on clock speed
		for(volatile uint32_t i = 0; i < 400000; i++);

		// Set PA5 pin low
		GPIOA_BSRR = (1U << (5 + 16));

		for(volatile uint32_t i = 0; i < 400000; i++);
	}
}