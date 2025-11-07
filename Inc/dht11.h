#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DHT_GPIO_OUTPUT,
    DHT_GPIO_AF
} DHT_GPIO_Config;

typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
} DHT11_Data;

typedef struct {
    void (*gpio_config)(DHT_GPIO_Config gpio_config);   // reconfigure GPIO
    void (*gpio_set)(bool high);                        // drive the data line high/low
    void (*timer_start_dma)(void);                      // start timer capture with DMA
    void (*timer_stop)(void);                           // stop timer
} DHT11_HwInterface;

void DHT11_Init(void);

// Start an asynchronous read
bool DHT11_StartRead(void);

// Check if data is ready
bool DHT11_DataReady(void);

// Get the last reading
DHT11_Data DHT11_GetData(void);

#endif
