#include <stdint.h>

// External definitions found in the linker script
extern uint32_t _estack;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Function Prototypes - Reset_Handler, main, and ISR handlers
void Reset_Handler(void);
int main(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Vector Table
uint32_t vectorTable[] __attribute__((section(".isr_vector_tbl"))) = { 
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    (uint32_t)&SVC_Handler,
};

// Default Handler - fallback for undefined ISR handlers; loops infinitely to prevent undefined behavior
void Default_Handler(void) {
    while (1) { }
}

// Reset Handler Definition
void Reset_Handler(void) {
    uint32_t dataMemSize = &_edata - &_sdata;
    uint32_t bssMemSize = &_ebss - &_sbss;

    // Copies FLASH memory to SRAM
    uint32_t* p_mem_src = &_etext;
    uint32_t* p_mem_dest = &_sdata;
    for (uint32_t i = 0; i < dataMemSize; i++) 
        *p_mem_dest++ = *p_mem_src++;

    // Zeros BSS Section
    p_mem_dest = &_sbss;
    for (uint32_t i = 0; i < bssMemSize; i++)
        *p_mem_dest++ = 0;

    // Enter main
    main();
}