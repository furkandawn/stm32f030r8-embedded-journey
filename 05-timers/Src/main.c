#include "stm32f030x8.h"
#include "mode_timer.h"
#include "effect_timer.h"
#include "gpio.h"

int main(void)
{
	gpio_led_init();
	mode_timer_init();
	effect_timer_init();
	
	while(1)
	{
		__WFI();
	}
}