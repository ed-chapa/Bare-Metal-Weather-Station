#include "config.h"

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
