#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile uint32_t CALIB;
} SysTick_Struct;

#define SYSTICK_BASE 0xE000E010UL
#define SYSTICK ((SysTick_Struct*)SYSTICK_BASE)

#define CSR_ENABLE    (1U << 0)
#define CSR_CLKSOURCE (1U << 2)
#define CSR_COUNTFLAG (1U << 16)

#define MSEC_TICK 16000 /* 16,00,000 Hz (Internal Clock Frequency) / 1000 msec (1000 msec in 1 second) = 16,000 ticks; Clock ticks every 1 msec */

static inline void systickMsecDelay(uint32_t msec) {
    SYSTICK->RVR = MSEC_TICK - 1;
    SYSTICK->CVR = 0;
    SYSTICK->CSR = CSR_CLKSOURCE | CSR_ENABLE;

    for (uint32_t i = 0; i < msec; i++){
        while ((SYSTICK->CSR & CSR_COUNTFLAG) == 0);
    }

    SYSTICK->CSR = 0;
}

#endif
