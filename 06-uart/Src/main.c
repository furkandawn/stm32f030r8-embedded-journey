#include "stm32f030x8.h"
#include "uart.h"
#include "gpio.h"

int main(void)
{

	GPIO_init();
	USART2_init();

	while(1)
	{
		if (buffer_flag)
		{
			buffer_flag = false;

			uint8_t byte;
			USART2_SendString("\r\n[");
			while(RingBuffer_Get(&byte))
			{
				USART2_SendChar((char)byte);
			}
			USART2_SendString("]\r\n");
		}

		__WFI();
	}
}