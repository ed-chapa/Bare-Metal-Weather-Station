#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx.h"

typedef enum {
    I2C_MODE_STANDARD = 0,
    I2C_MODE_FAST = 1
} I2C_Mode;

typedef struct {
    I2C_TypeDef *instance;
    I2C_Mode mode;
    // uint8_t ownAddress;
    bool enableAck;
} I2C_Configuration;

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAddress(uint8_t address, uint8_t read);
void I2C_WriteByte(uint8_t data);
void I2C_Write(uint8_t *data, uint32_t size);

#endif
