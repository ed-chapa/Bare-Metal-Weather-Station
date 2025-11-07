#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include "gpio.h"
#include "irq.h"
#include "dma.h"
#include "i2c.h"
#include "ssd1306.h"
#include "tim.h"

volatile uint8_t transmission_started = 0;
volatile uint8_t ready_to_receive = 0;
volatile uint8_t data_received = 0;
volatile uint32_t tick_count = 0;
volatile uint32_t start_pulse = 0;
volatile uint8_t time_index = 0;

char i2c_buffer[128] = {0};
uint8_t decoded_data[5] = {0};
uint16_t timestamps[43] = {0};

//////////////// Init structs ////////////////

static GPIO_Configuration PA8_Output = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_OUTPUT,
    .outputType = GPIO_OTYPE_PUSHPULL,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
};

static GPIO_Configuration PA8_AF1 = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_ALTERNATE,
    .alternateFunction = GPIO_ALTERNATE_1
};

static GPIO_Configuration PB8_AF4 = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_ALTERNATE,
    .pull = GPIO_PULL_NONE,
    .outputType = GPIO_OTYPE_OPENDRAIN,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
    .alternateFunction = GPIO_ALTERNATE_4
};

static GPIO_Configuration PB9_AF4 = {
    .pin = GPIO_PIN_9,
    .mode = GPIO_MODE_ALTERNATE,
    .pull = GPIO_PULL_NONE,
    .outputType = GPIO_OTYPE_OPENDRAIN,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
    .alternateFunction = GPIO_ALTERNATE_4
};

static I2C_Configuration I2C1_Config = {
    .instance = I2C1,
    .clockFrequency = 42,
    .mode = I2C_MODE_STANDARD,
    .enableAck = false
};

static DMA_Configuration DMA2_Stream1_Config = {
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

static TIM_BaseConfig TIM6_Config = {
    .prescaler = 8399,
    .arr = 9,
    .interrupts = TIM_INTERRUPT_UPDATE
};

// IRQ Handlers
void DMA_TransferComplete(void) {
    data_received = 1;
}

void TIM6_UpdateHandler(void) {
    tick_count++;
    // Send 18ms LOW pulse every 1.25 seconds
    if (tick_count == 1250)
    {
        start_pulse = 1;
    }
    else if (tick_count == 1518)
    {
        tick_count = 0; // Reset counter
        transmission_started = 1; // Signal that transmission has started to the main loop
    }
}

void SystemClock_Init(void) {
    RCC->CR |= RCC_CR_HSION;                                                // Enable HSI
    while (!(RCC->CR & RCC_CR_HSIRDY));                                     // Wait for HSI to be ready

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                                      // Enable power interface clock
    PWR->CR &= ~PWR_CR_VOS;                                                 // Clear bits
    PWR->CR |= (2 << PWR_CR_VOS_Pos);                                       // Set the voltage regulator to Scale 2

    FLASH->ACR |= FLASH_ACR_DCEN;                                           // Enable data cache
    FLASH->ACR |= FLASH_ACR_ICEN;                                           // Enable instruction cache
    FLASH->ACR |= FLASH_ACR_PRFTEN;                                         // Enable prefetch buffer
    FLASH->ACR &= ~FLASH_ACR_LATENCY;                                       // Clear bits
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;                                    // Set the flash latency to 2 wait states

    RCC->CFGR &= ~RCC_CFGR_HPRE;                                            // Clear bits
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                                        // Don't divide AHB clock frequency
    RCC->CFGR &= ~RCC_CFGR_PPRE1;                                           // Clear bits
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                                       // Divide APB1 clock frequency by 2
    RCC->CFGR &= ~RCC_CFGR_PPRE2;                                           // Don't divide APB2 clock frequency

    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;                                    // Set HSI as PLL clock source
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;                                      // Clear bits
    RCC->PLLCFGR |= (16 << RCC_PLLCFGR_PLLM_Pos);                           // Set main PLL division factor to 16
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;                                      // Clear bits
    RCC->PLLCFGR |= (336 << RCC_PLLCFGR_PLLN_Pos);                          // Set main PLL multiplication factor to 336
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;                                      // Clear bits
    RCC->PLLCFGR |= (1 << RCC_PLLCFGR_PLLP_Pos);                            // Set main PLL division factor to 4

    RCC->CR |= RCC_CR_PLLON;                                                // Enable PLL
    while(!(RCC->CR & RCC_CR_PLLRDY));                                      // Wait for PLL to be ready

    RCC->CFGR &= ~RCC_CFGR_SW;                                              // Clear bits
    RCC->CFGR |= RCC_CFGR_SW_PLL;                                           // Set PLL as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);                 // Wait for PLL to be set as system clock

    SystemCoreClockUpdate();                                                // Update SystemCoreClock variable
}

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

int main(void) {
    SystemClock_Init();
    Interrupt_Init();
    DMA_Init();
    GPIO_Init();
    I2C_Init();
    SSD1306_Init();
    TIM1_Init();
    TIM6_Init();

    while(1) {
        if (start_pulse) {
            start_pulse = 0;
            GPIO_Configure(GPIOA, &PA8_Output);
            GPIO_Reset(GPIOA, GPIO_PIN_8); // Start the pulse
        }

        if (transmission_started) {
            transmission_started = 0;
            GPIO_Configure(GPIOA, &PA8_AF1);
            DMA_EnableStream(DMA2_Stream1);

            // TIM1->CR1 &= ~TIM_CR1_CEN;                  // Disable TIM1
            TIM1->CNT = 0;                              // Reset the counter
            TIM1->EGR |= TIM_EGR_UG;                    // Initialize all registers
            TIM1->CR1 |= TIM_CR1_CEN;                   // Start the counter
        }

        if (data_received) { // Start decoding
            data_received = 0;
            const uint16_t threshold = 100; // Used to measure pulse widths
            memset(decoded_data, 0, 5);

            for (int i = 2; i < 42; ++i) {
                uint16_t pulse_width = timestamps[i] - timestamps[i - 1];
                int bit_index = i - 2;
                if (pulse_width > threshold)
                    decoded_data[bit_index / 8] |= (1 << (7 - (bit_index % 8)));
            }

            if ((
                decoded_data[0] +
                decoded_data[1] +
                decoded_data[2] +
                decoded_data[3]
                ) != decoded_data[4]) { // Check for data corruption
                sprintf(i2c_buffer, "DATA WAS CORRUPTED!\n");
                SSD1306_DrawText(i2c_buffer, 0, 2);
            }
            else { // Send decoded data if checksum is ok
                sprintf(i2c_buffer, "   TEMPERATURE: %d.%dC\n      HUMIDITY: %d.%d%%\n", decoded_data[2], decoded_data[3], decoded_data[0], decoded_data[1]);
                SSD1306_DrawIcon(thermometer_icon, 7, 1);
                SSD1306_DrawText(i2c_buffer, 0, 5);
            }
        }
    }
}
