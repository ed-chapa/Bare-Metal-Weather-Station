#include "i2c.h"
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

void I2C_Configure(I2C_TypeDef *i2c, I2C_Configuration *config) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Set the frequency
    i2c->CR2 &= ~I2C_CR2_FREQ_Msk;
    i2c->CR2 |= ((config->clockFrequency / 1000000) << I2C_CR2_FREQ_Pos);

    // Set the mode
    i2c->CCR &= ~I2C_CCR_FS;
    i2c->CCR |= (config->mode << I2C_CCR_FS_Pos);

    // Set the duty cycle 
    i2c->CCR &= ~I2C_CCR_DUTY;
    i2c->CCR |= (config->duty << I2C_CCR_DUTY_Pos);

    // Calculate CCR and TRISE
    int ccr = 0;
    int trise = 0;

    uint32_t real_frequency = config->clockFrequency * 1000000;
    if (config->mode == I2C_MODE_STANDARD) { // Standard mode (100 kHz)
        ccr = real_frequency / (2 * 100000);
        trise = (real_frequency / 1000000) + 1;
    } else if (config->mode == I2C_MODE_FAST) {
        if (config->duty == I2C_DUTY_2) { // Fast mode with duty cycle = 2
            ccr = real_frequency / (3 * 400000);
        }
        else if (config->duty == I2C_DUTY_16_9) { // Fast mode with duty cycle = 16/9
            ccr = real_frequency / (25 * 400000);
        }
        trise = ((real_frequency * 300) / 1000000000) + 1;
    }
    // Set CCR
    i2c->CCR &= ~I2C_CCR_CCR;
    i2c->CCR |= (ccr << I2C_CCR_CCR_Pos);
    // Set TRISE
    i2c->TRISE = trise;

    // Set Acknowledge
    i2c->CR1 |= (config->enableAck << I2C_CR1_ACK_Pos);

    // Enable I2C
    i2c->CR1 |= I2C_CR1_PE;
}

void I2C_EnableClock(I2C_TypeDef *i2c) {
    if (i2c == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    } else if (i2c == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    }
}

void I2C_DisableClock(I2C_TypeDef *i2c) {
    if (i2c == I2C1) {
        RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
    } else if (i2c == I2C2) {
        RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;
    }
}

void I2C_Start(void) {
    // I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_START;
}

void I2C_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_SendAddress(uint8_t address, uint8_t read) {
    while(!(I2C1->SR1 & I2C_SR1_SB));
    I2C1->DR = (address << 1) + read;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    volatile uint32_t temp;
    temp = I2C1->SR1;
    temp = I2C1->SR2;
}

void I2C_WriteByte(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void I2C_Write(uint8_t *data, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        I2C_WriteByte(data[i]);
    }
}

void I2C_Scan(void) {
    for (uint8_t addr = 0; addr < 128; addr++) {
        // Send START condition
        I2C1->CR1 |= I2C_CR1_START;
        while (!(I2C1->SR1 & I2C_SR1_SB));

        // Send address with write bit (0)
        I2C1->DR = (addr << 1);
        while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));

        if (I2C1->SR1 & I2C_SR1_ADDR) {
            // ACK received
            volatile uint32_t temp = I2C1->SR1;
            temp = I2C1->SR2;
            break;
        }
        else {
            // NACK received
            I2C1->SR1 &= ~I2C_SR1_AF; // Clear AF flag
        }

        // Send STOP condition
        I2C1->CR1 |= I2C_CR1_STOP;

        // Small delay between attempts
        for (volatile int i = 0; i < 1000; i++);
    }
}
