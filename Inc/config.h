#ifndef CONFIG_H
#define CONFIG_H

#include "gpio.h"
#include "i2c.h"
#include "dma.h"
#include "tim.h"
#include "globals.h"

extern GPIO_Configuration PA8_Output;
extern GPIO_Configuration PA8_AF1;
extern GPIO_Configuration PB8_AF4;
extern GPIO_Configuration PB9_AF4;
extern I2C_Configuration I2C1_Config;
extern DMA_Configuration DMA2_Stream1_Config;
extern TIM_BaseConfig TIM6_Config;

#endif
