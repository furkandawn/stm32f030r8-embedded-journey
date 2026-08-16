#include "stm32f030x8.h"
#include "mode_timer.h"
#include "effect_timer.h"

/* TIM16_IRQHandler 8 seconds periodic interrupt */

/* PCLK calculated by hand with RM, 48MHz */
#define MODE_TIMER_PSC 48000
#define MODE_TIMER_ARR 8000

volatile LedMode_t current_led_mode = MODE_TOGGLE;

void mode_timer_init(void) {
    /* enable clock gate through TIM16 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    
    TIM16->PSC = (MODE_TIMER_PSC - 1);
    TIM16->ARR = (MODE_TIMER_ARR - 1);

    TIM16->DIER &= ~TIM_DIER_UIE;

    TIM16->EGR |= TIM_EGR_UG;
    TIM16->SR &= ~TIM_SR_UIF;
    
    TIM16->DIER |= TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM16_IRQn);

    TIM16->CR1 |= TIM_CR1_CEN;
}

void TIM16_IRQHandler(void) {
    /* clear the interrupt flag, if not this ISR will be called forever */
    TIM16->SR &= ~TIM_SR_UIF;

    if (current_led_mode == MODE_BREATHING)
    {
        current_led_mode = MODE_TOGGLE;
        set_mode_toggle();
    }
    else
    {
        current_led_mode = MODE_BREATHING;
        set_mode_pwm();
    }
}