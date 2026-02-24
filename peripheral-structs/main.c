#include <stdint.h>

#define PERIPH_BASE 0x40000000UL

#define GPIOA_OFFSET (0x20000UL)
#define GPIOA_BASE   (PERIPH_BASE + GPIOA_OFFSET)
#define GPIOA_PA5    (1U << 5)
#define LED_PIN      GPIOA_PA5 

#define MODER_PA5_BIT0 (1U << 10)
#define MODER_PA5_BIT1 (1U << 11)

#define RCC_OFFSET  (0x23800UL)
#define RCC_BASE    (PERIPH_BASE + RCC_OFFSET)
#define RCC_GPIOAEN (1U << 0)

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

typedef struct
{
    volatile uint32_t PADDING[12];
    volatile uint32_t AHB1ENR;
} RCC_Struct;

#define RCC   ((RCC_Struct*)RCC_BASE)
#define GPIOA ((GPIO_Struct*)GPIOA_BASE)

int main(void)
{
    // Enable RCC for Port A
    RCC->AHB1ENR |= RCC_GPIOAEN;
    // Set MODER of GPIOA to 01 for general-purpose output
    GPIOA->MODER |= MODER_PA5_BIT0;
    GPIOA->MODER &= ~MODER_PA5_BIT1;

    // Loop Forever
	for(;;) {
        GPIOA->ODR ^= LED_PIN;
        for (volatile uint32_t i = 0; i < 0x4FFFF; i++) { }
    }
}
