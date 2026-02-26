#ifndef PERIPH_H
#define PERIPH_H

#include <stdint.h>

#define PERIPH_BASE 0x40000000UL

#define GPIOA_OFFSET 0x20000UL
#define GPIOA_BASE   (PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET  0x23800UL
#define RCC_BASE    (PERIPH_BASE + RCC_OFFSET)

#define TIM2_OFFSET 0x00000UL
#define TIM2_BASE   (PERIPH_BASE + TIM2_OFFSET)

#define UART4_OFFSET 0x04C00
#define UART4_BASE   (PERIPH_BASE + UART4_OFFSET)

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_Struct;

/* How padding is calculated: [New Register Offset - (4 + Old Register Offset)] / 4 */
typedef struct
{
    volatile uint32_t PADDING1[12];
    volatile uint32_t AHB1ENR;
    volatile uint32_t PADDING2[3];
    volatile uint32_t APB1ENR;
} RCC_Struct;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t PADDING1[3];
    volatile uint32_t SR;
    volatile uint32_t PADDING2[4];
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_Struct;

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
} USART_Struct;

#define RCC   ((RCC_Struct*)RCC_BASE)
#define GPIOA ((GPIO_Struct*)GPIOA_BASE)
#define TIM2  ((TIM_Struct*)TIM2_BASE)
#define  UART4 ((USART_Struct*)UART4_BASE)

#endif