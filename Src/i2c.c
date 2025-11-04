#include "i2c.h"
#include "stm32f4xx.h"

void I2C_Configure(I2C_TypeDef *i2c, I2C_Configuration config) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    
}

void I2C_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR2 = (I2C1->CR2 & ~I2C_CR2_FREQ_Msk) | (42 << I2C_CR2_FREQ_Pos);
    I2C1->CCR &= ~I2C_CCR_FS; // Standard mode
    I2C1->CCR &= ~I2C_CCR_DUTY; // DUTY = 0;
    I2C1->CCR |= (210 << I2C_CCR_CCR_Pos);
    I2C1->TRISE = 43;
    I2C1->CR1 |= I2C_CR1_PE; // Enable I2C peripheral
}

void I2C_Start(void) {
    I2C1->CR1 |= I2C_CR1_ACK;
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
