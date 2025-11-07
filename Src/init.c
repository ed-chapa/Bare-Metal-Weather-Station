#include "init.h"

#include "globals.h"
#include "config.h"
#include "interrupt.h"
#include "irq.h"

void Interrupt_Init() {
    IRQ_RegisterCallback(DMA2_Stream1_IRQn, DMA_TransferComplete);
    IRQ_RegisterCallback(TIM6_DAC_IRQn, TIM6_UpdateHandler);
}

void GPIO_Init(void) {
    GPIO_EnableClock(GPIOA);
    GPIO_EnableClock(GPIOB);

    GPIO_Configure(GPIOA, &PA8_Output);
    GPIO_Set(GPIOA, GPIO_PIN_8); // Set initially HIGH

    GPIO_Configure(GPIOB, &PB8_AF4);
    GPIO_Configure(GPIOB, &PB9_AF4);
}

void DMA_Init(void) {
    DMA_EnableClock(DMA2);
    DMA_Configure(DMA2_Stream1, &DMA2_Stream1_Config); // DMA2_Stream1 for TIM1
}

void I2C_Init(void) {
    I2C_EnableClock(I2C1);
    I2C_Configure(I2C1, &I2C1_Config);
}

void TIM1_Init(void) {
    // Enable TIM1 clock
    TIM_EnableClock(TIM1);

    // Fill in configuration struct
    TIM_InputCaptureConfig tim1_cfg = {
        .prescaler   = 83,                       // 1 MHz timer clock
        .arr         = 0xFFFF,                   // Max ARR to avoid overflow
        .channel     = TIM_CHANNEL_1,            // Use channel 1
        .icPolarity  = TIM_ICPOLARITY_FALLING,   // Capture falling edges
        .icSelection = TIM_ICSELECTION_DIRECTTI, // Map CC1 to TI1
        .icFilter    = TIM_ICFILTER_FCKINT_N4    // Filter: 4 samples
    };

    // Configure TIM1 for input capture using abstraction
    TIM_IC_Configure(TIM1, &tim1_cfg);

    // Enable DMA
    TIM1->DIER |= TIM_DIER_CC1DE;
}

void TIM6_Init() {
    TIM_EnableClock(TIM6);
    TIM_BASE_Configure(TIM6, &TIM6_Config);
    TIM_EnableCounter(TIM6);
}
