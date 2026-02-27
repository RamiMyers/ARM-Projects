#ifndef TIMER_H
#define TIMER_H

#include "peripherals.h"

#define RCC_TIM2EN (1U << 0)
#define CR1_CEN    (1U << 0)
#define SR_UIF     (1U << 0)

#define PRESCALE 16000
#define AUTO_RELOAD 1000

static inline void tim2_1Hz_Init(void) {
    RCC->APB1ENR |= RCC_TIM2EN;

    TIM2->PSC = PRESCALE - 1;
    TIM2->ARR = AUTO_RELOAD - 1;
    TIM2->CNT = 0;
    TIM2->CR1 |= CR1_CEN;
}

#endif