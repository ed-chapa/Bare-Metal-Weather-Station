#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"



//////////////// Init structs ////////////////

static GPIO_Configuration PA8_Output = {
    .pin = GPIO_PIN_8,
    .mode = GPIO_MODE_OUTPUT,
    .outputType = GPIO_OTYPE_PUSHPULL,
    .outputSpeed = GPIO_OSPEED_VERYHIGH,
};

// PA8 = TIM1_CH1 (AF1)
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

volatile uint8_t transmission_started = 0;
volatile uint8_t ready_to_receive = 0;
volatile uint8_t data_received = 0;
volatile uint32_t tick_count = 0;
volatile uint32_t start_pulse = 0;
volatile uint8_t time_index = 0;

char i2c_buffer[128] = {0};
uint8_t decoded_data[5] = {0};
uint16_t timestamps[43] = {0};

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

void DMA_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;                                     // Enable clock for DMA2

    // DMA2_Stream1 for TIM1
    DMA2_Stream1->NDTR = 43;                                                // Set the number of data transfer to 42
    DMA2_Stream1->PAR = (uint32_t) &(TIM1->CCR1);                           // Set peripheral address to TIM1->CCR1
    DMA2_Stream1->M0AR = (uint32_t) timestamps;                             // Set memory address to timestamps
    DMA2_Stream1->CR |= (6 << DMA_SxCR_CHSEL_Pos);                          // Select channel 6
    DMA2_Stream1->CR &= ~DMA_SxCR_DIR;                                      // Set transfer direction to peripheral-to-memory
    DMA2_Stream1->CR &= ~DMA_SxCR_PINC;                                     // Disable peripheral increment mode
    DMA2_Stream1->CR |= DMA_SxCR_MINC;                                      // Enable memory increment mode
    DMA2_Stream1->CR |= (1 << DMA_SxCR_PSIZE_Pos);                          // Set peripheral data size to half-word (16-bit)
    DMA2_Stream1->CR |= (1 << DMA_SxCR_MSIZE_Pos);                          // Set memory data size to half-word (16-bit)
    DMA2_Stream1->CR |= DMA_SxCR_TCIE;                                      // Enable transfer complete interrupt

    // Enable interrupt in the NVIC
    NVIC_SetPriority(DMA2_Stream1_IRQn, 2);
    NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

void GPIO_Init(void) {
    GPIO_EnableClock(GPIOA);
    GPIO_EnableClock(GPIOB);

    GPIO_Configure(GPIOA, &PA8_Output);
    GPIO_Set(GPIOA, GPIO_PIN_8); // Set initially HIGH

    GPIO_Configure(GPIOB, &PB8_AF4);
    GPIO_Configure(GPIOB, &PB9_AF4);
}

void TIM1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;                                     // Enable TIM1 clock

    // Configure TIM1 for input capture
    TIM1->PSC = 83;                                                         // Set clock frequency to 1 MHz
    TIM1->ARR = 0xFFFF;                                                     // Set ARR to its max value to prevent overflows
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;                                         // Clear bits
    TIM1->CCMR1 |= (1 << TIM_CCMR1_CC1S_Pos);                               // IC1 mapped to TI1
    TIM1->CCMR1 |= (2 << TIM_CCMR1_IC1F_Pos);                               // Set filter duration to 4 cycles
    TIM1->CCER &= ~TIM_CCER_CC1P;                                           // Clear bits
    TIM1->CCER |= (1 << TIM_CCER_CC1P_Pos);                                 // Capture falling edges
    TIM1->CCER |=  TIM_CCER_CC1E;                                           // Enable input capture for channel 1
    TIM1->DIER |= TIM_DIER_CC1DE;                                           // Enable DMA request on input capture for channel 1
}

void TIM6_Init() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 8399;                                                       // Set frequency to 10 kHz
    TIM6->ARR = 9;                                                          // Overflow every 1 ms
    TIM6->DIER |= TIM_DIER_UIE;                                             // Enable update interrupt
    TIM6->EGR |= TIM_EGR_UG;                                                // Initialize all registers

    // Enable interrupt in the NVIC
    NVIC_SetPriority(TIM6_DAC_IRQn, 1);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    TIM6->CR1 |= TIM_CR1_CEN;                                               // Start the counter
}

void DMA2_Stream1_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF1) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF1; // Clear interrupt flag
        data_received = 1;
    }
}

void TIM6_DAC_IRQHandler(void) {
    if (TIM6->SR & TIM_SR_UIF) {
        TIM6->SR &= ~TIM_SR_UIF; // Clear interrupt flag
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
}

int main(void) {
    SystemClock_Init();
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

            DMA2_Stream1->CR &= ~DMA_SxCR_EN;           // Disable DMA stream
            while (DMA2_Stream1->CR & DMA_SxCR_EN);     // Wait until it's actually disabled
            DMA2_Stream1->CR |= DMA_SxCR_EN;            // Enable DMA Stream
            TIM1->CR1 &= ~TIM_CR1_CEN;                  // 1. Disable TIM1
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
