#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7,
    GPIO_PIN_8 = 8,
    GPIO_PIN_9 = 9,
    GPIO_PIN_10 = 10,
    GPIO_PIN_11 = 11,
    GPIO_PIN_12 = 12,
    GPIO_PIN_13 = 13,
    GPIO_PIN_14 = 14,
    GPIO_PIN_15 = 15
} GPIO_Pin;

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTERNATE = 2,
    GPIO_MODE_ANALOG = 3,
} GPIO_Mode;

typedef enum {
    GPIO_OTYPE_PUSHPULL = 0,
    GPIO_OTYPE_OPENDRAIN = 1
} GPIO_OutputType;

typedef enum {
    GPIO_OSPEED_LOW = 0,
    GPIO_OSPEED_MEDIUM = 1,
    GPIO_OSPEED_HIGH = 2,
    GPIO_OSPEED_VERYHIGH = 3
} GPIO_OutputSpeed;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2
} GPIO_Pull;

typedef enum {
    GPIO_ALTERNATE_0 = 0,
    GPIO_ALTERNATE_1 = 1,
    GPIO_ALTERNATE_2 = 2,
    GPIO_ALTERNATE_3 = 3,
    GPIO_ALTERNATE_4 = 4,
    GPIO_ALTERNATE_5 = 5,
    GPIO_ALTERNATE_6 = 6,
    GPIO_ALTERNATE_7 = 7,
    GPIO_ALTERNATE_8 = 8,
    GPIO_ALTERNATE_9 = 9,
    GPIO_ALTERNATE_10 = 10,
    GPIO_ALTERNATE_11 = 11,
    GPIO_ALTERNATE_12 = 12,
    GPIO_ALTERNATE_13 = 13,
    GPIO_ALTERNATE_14 = 14,
    GPIO_ALTERNATE_15 = 15
} GPIO_AlternateFunction;

typedef struct {
    GPIO_Pin pin;
    GPIO_Mode mode;
    GPIO_Pull pull;
    GPIO_OutputType outputType;
    GPIO_OutputSpeed outputSpeed;
    GPIO_AlternateFunction alternateFunction;
} GPIO_Configuration;

void GPIO_EnableClock(GPIO_TypeDef *port);
void GPIO_DisableClock(GPIO_TypeDef *port);
void GPIO_Configure(GPIO_TypeDef *port, GPIO_Configuration *config);
void GPIO_Toggle(GPIO_TypeDef *port, GPIO_Pin pin);
void GPIO_Set(GPIO_TypeDef *port, GPIO_Pin pin);
void GPIO_Reset(GPIO_TypeDef *port, GPIO_Pin pin);

#endif
