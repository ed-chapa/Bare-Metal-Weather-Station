#ifndef CONFIG_H
#define CONFIG_H

#include "gpio.h"
#include "i2c.h"
#include "dma.h"
#include "tim.h"

extern GPIO_Configuration PA8_Output;
extern GPIO_Configuration PA8_AF1;
extern GPIO_Configuration PB8_AF4;
extern GPIO_Configuration PB9_AF4;
extern I2C_Configuration I2C1_Config;
extern DMA_Configuration DMA2_Stream1_Config;
extern TIM_BaseConfig TIM6_Config;

void Interrupt_Init(void);
void GPIO_Init(void);
void DMA_Init(void);
void I2C_Init(void);
void TIM1_Init(void);
void TIM6_Init(void);

#endif
