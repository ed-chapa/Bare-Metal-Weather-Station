#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include "globals.h"
#include "config.h"
#include "system_clock.h"
#include "gpio.h"
#include "irq.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "ssd1306.h"

int main(void) {
    SystemClock_Init();
    Interrupt_Init();
    DMA_Init();
    GPIO_Init();
    I2C_Init();
    SSD1306_Init(I2C1);
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
            TIM_EnableCounter(TIM1);
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
