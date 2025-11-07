#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx.h"

typedef enum {
    I2C_MODE_STANDARD = 0,
    I2C_MODE_FAST = 1
} I2C_Mode;

typedef enum {
    I2C_DUTY_2 = 0,
    I2C_DUTY_16_9 = 1
} I2C_Duty;

typedef enum {
    I2C_ACK_DISABLE = 0,
    I2C_ACK_ENABLE = 1
} I2C_Ack;

typedef enum {
    I2C_DIRECTION_WRITE = 0,
    I2C_DIRECTION_READ = 1
} I2C_Direction;

typedef struct {
    I2C_TypeDef *instance;
    uint8_t clockFrequency;
    I2C_Mode mode;
    I2C_Duty duty;
    bool enableAck;
} I2C_Configuration;

void I2C_EnableClock(I2C_TypeDef *i2c);
void I2C_DisableClock(I2C_TypeDef *i2c);
void I2C_Configure(I2C_TypeDef *i2c, I2C_Configuration *config);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAddress(uint8_t address, I2C_Direction dir);
void I2C_WriteByte(uint8_t data);
void I2C_Write(uint8_t *data, uint32_t size);

#endif
