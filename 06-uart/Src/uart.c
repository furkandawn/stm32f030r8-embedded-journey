#include "stdint.h"
#include "stdbool.h"
#include "stm32f030x8.h"

#define RING_BUFFER_SIZE 128
#define RING_BUFFER_MASK (RING_BUFFER_SIZE - 1)

volatile uint8_t rx_buffer[RING_BUFFER_SIZE];
volatile uint16_t rx_head = 0;
volatile uint16_t rx_tail = 0;
volatile bool buffer_flag = false;

void USART2_SendChar(char c) {
    while (!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = (uint16_t)c;
}

char USART2_ReceiveChar(void) {
    while (!(USART2->ISR & USART_ISR_RXNE));
    return (char)(USART2->RDR & 0xFF);
}

void USART2_SendString(const char *str) {
    while (*str) {
        USART2_SendChar(*str++);
    }
}

void USART2_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    USART2->BRR = 48000000UL / 115200UL;

    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

    NVIC_EnableIRQ(USART2_IRQn);
}

static inline void RingBuffer_Put(uint8_t data)
{
	uint16_t next_head = (rx_head + 1) & RING_BUFFER_MASK;

	if (next_head != rx_tail)
	{
		rx_buffer[rx_head] = data;
		rx_head = next_head;
	}
}

bool RingBuffer_Get(uint8_t *data)
{
	if (rx_head == rx_tail) return false;

	*data = rx_buffer[rx_tail];

	rx_tail = (rx_tail + 1) & RING_BUFFER_MASK;

	return true;
}


void USART2_IRQHandler(void) {
	uint32_t isr_status = USART2->ISR;
	if (isr_status & USART_ISR_RXNE)
	{
		char rx_data = (char)(USART2->RDR & 0xFF);

		if (rx_data == '0')
		{
			buffer_flag = true;
		}
		else
		{
			RingBuffer_Put(rx_data);
		}
	}
}
