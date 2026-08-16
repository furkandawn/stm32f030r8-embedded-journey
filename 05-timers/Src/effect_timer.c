#include "stm32f030x8.h"

/* advanced timer TIM1's 4 channel used as both PWM provider and event (toggle Vcc output) creater */

/* PCLK calculated by hand with RM, 48MHz */
#define TOGGLE_PSC 48000
#define PWM_PSC 48
#define DEFAULT_PSC (TOGGLE_PSC)

#define TOGGLE_ARR 0xFFFF /* 16 bit ARR */
#define PWM_ARR 20000 /* (1 MHz / 20000) = 50 Hz, 20ms event trigger*/

#define TOGGLE_CCR1 500 /* Fastest toggle */
#define TOGGLE_CCR2 1000
#define TOGGLE_CCR3 2000
#define TOGGLE_CCR4 4000 /* Slowest toggle */

#define PWM_STEP1 2048 /* Fastest breath */
#define PWM_STEP2 1024
#define PWM_STEP3 512
#define PWM_STEP4 256 /* Slowest breath */

void set_mode_toggle(void);

static uint8_t dir1 = 0, dir2 = 0, dir3 = 0, dir4 = 0;

void effect_timer_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    TIM1->PSC = (DEFAULT_PSC - 1);

    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

    TIM1->CR1 &= ~TIM_CR1_ARPE;

    TIM1->DIER |= TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM1_CC_IRQn);    
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

    set_mode_toggle();
}

void set_mode_toggle(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;

    TIM1->DIER &= ~TIM_DIER_UIE;

    TIM1->CR1 &= ~TIM_CR1_ARPE;

    /* Force outputs low before switching to toggle mode */
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2);
    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M);
    TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2);

    TIM1->EGR |= TIM_EGR_UG;   /* update state */
    TIM1->SR &= ~TIM_SR_UIF;

    TIM1->PSC = (TOGGLE_PSC - 1);
    TIM1->ARR = 0xFFFF;
    TIM1->CNT = 0;

    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);
    
    TIM1->CCR1 = TOGGLE_CCR1;
    TIM1->CCR2 = TOGGLE_CCR2;
    TIM1->CCR3 = TOGGLE_CCR3;
    TIM1->CCR4 = TOGGLE_CCR4;

    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0);
    
    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M);
    TIM1->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0);

    TIM1->SR &= ~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF);
    TIM1->DIER |= (TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE);
    
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;
    TIM1->DIER |= TIM_DIER_UIE;

    TIM1->CR1 |= TIM_CR1_CEN;
}

void set_mode_pwm(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;

    TIM1->DIER &= ~TIM_DIER_UIE;

    TIM1->CR1 |= TIM_CR1_ARPE;

    TIM1->PSC = (PWM_PSC - 1);
    TIM1->ARR = (PWM_ARR - 1);
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;
    TIM1->CCR4 = 0;
    dir1 = 0;
    dir2 = 0;
    dir3 = 0;
    dir4 = 0;

    TIM1->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    TIM1->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);
    
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);

    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M);
    TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
    
    TIM1->DIER &= ~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE);
    
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;
    
    TIM1->DIER |= TIM_DIER_UIE;

    TIM1->CR1 |= TIM_CR1_CEN;
}

static void pwm_update_helper(__IO uint32_t *ccr, uint8_t *direction, uint16_t step)
{
    if (*direction == 0)
    {
        if (*ccr + step >= (PWM_ARR - 1))
        {
            *ccr = (PWM_ARR - 1);
            *direction = 1;
        }
        else
        {
            *ccr += step;
        }
    }
    else
    {
        if (*ccr <= step)
        {
            *ccr = 0;
            *direction = 0;
        }
        else
        {
            *ccr -= step;
        }
    }
}

static void pwm_update(void)
{
    pwm_update_helper(&TIM1->CCR1, &dir1, PWM_STEP1);
    pwm_update_helper(&TIM1->CCR2, &dir2, PWM_STEP2);
    pwm_update_helper(&TIM1->CCR3, &dir3, PWM_STEP3);
    pwm_update_helper(&TIM1->CCR4, &dir4, PWM_STEP4);
}

/* IRS used for updating PWM duty cycle */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM1->SR & TIM_SR_UIF)
    {
        TIM1->SR &= ~TIM_SR_UIF;
        pwm_update();
    }
}

/* IRS used for toggling each channel's output Vcc and updating OC values */
void TIM1_CC_IRQHandler(void)
{
    if (TIM1->SR & TIM_SR_CC1IF)
    {
        TIM1->SR &= ~TIM_SR_CC1IF;
        TIM1->CCR1 += TOGGLE_CCR1;
    }

    if (TIM1->SR & TIM_SR_CC2IF)
    {
        TIM1->CCR2 += TOGGLE_CCR2;
        TIM1->SR &= ~TIM_SR_CC2IF;
    }

    if (TIM1->SR & TIM_SR_CC3IF)
    {
        TIM1->CCR3 += TOGGLE_CCR3;
        TIM1->SR &= ~TIM_SR_CC3IF;
    }

    if (TIM1->SR & TIM_SR_CC4IF)
    {
        TIM1->CCR4 += TOGGLE_CCR4;
        TIM1->SR &= ~TIM_SR_CC4IF;
    }
}
