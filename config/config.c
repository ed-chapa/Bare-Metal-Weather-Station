#include "config.h"

#include "globals.h"
#include "config.h"
#include "interrupt.h"
#include "irq.h"

GPIO_Configuration PA8_Output = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_OUTPUT,
    .outputType = GPIO_OTYPE_PUSHPULL,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
};

GPIO_Configuration PA8_AF1 = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_ALTERNATE,
    .alternateFunction = GPIO_ALTERNATE_1
};

GPIO_Configuration PB8_AF4 = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_ALTERNATE,
    .pull = GPIO_PULL_NONE,
    .outputType = GPIO_OTYPE_OPENDRAIN,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
    .alternateFunction = GPIO_ALTERNATE_4
};

GPIO_Configuration PB9_AF4 = {
    .pin = GPIO_PIN_9,
    .mode = GPIO_MODE_ALTERNATE,
    .pull = GPIO_PULL_NONE,
    .outputType = GPIO_OTYPE_OPENDRAIN,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
    .alternateFunction = GPIO_ALTERNATE_4
};

I2C_Configuration I2C1_Config = {
    .instance = I2C1,
    .clockFrequency = 42,
    .mode = I2C_MODE_STANDARD,
    .enableAck = false
};

DMA_Configuration DMA2_Stream1_Config = {
    .stream = DMA2_Stream1,
    .channel = DMA_CHANNEL_6,
    .memorySize = DMA_MSIZE_HALFWORD,
    .peripheralSize = DMA_PSIZE_HALFWORD,
    .peripheralIncrease = DMA_PINC_DISABLE,
    .memoryIncrease = DMA_MINC_ENABLE,
    .transferDirection = DMA_DIRECTION_PERIPHERAL_TO_MEMORY,
    .interrupts = DMA_INTERRUPT_TC,
    .memoryAddress = (uint32_t)timestamps,
    .peripheralAddress = (uint32_t) &(TIM1->CCR1),
    .numberOfDataTransfers = 43
};

TIM_BaseConfig TIM6_Config = {
    .prescaler = 8399,
    .arr = 9,
    .interrupts = TIM_INTERRUPT_UPDATE
};

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
