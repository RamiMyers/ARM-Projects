// #ifndef BUTTON_H
// #define BUTTON_H

// #define PC13 (1U << 13)
// #define BUTTON_PIN PC13

// #define MODER_PC13_BIT0 (1U << 26)
// #define MODER_PC13_BIT1 (1U << 27)

// #define RCC_GPIOCEN (1U << 2)

// static inline void buttonInit(void) {
//     // Enable RCC for Port C
//     RCC->AHB1ENR |= RCC_GPIOCEN;

//     // Set PC13 mode to input (00)
//     GPIOC->MODER &= ~MODER_PC13_BIT0;
//     GPIOC->MODER &= ~MODER_PC13_BIT1;
// }

// static inline uint8_t buttonGet(void) {
//     // Return button state (button is active-low)
//     return !(GPIOC->IDR & BUTTON_PIN);
// }

// #endif