#include "dht11.h"
#include <string.h>

static DHT11_HwInterface hw;

void DHT11_Init(DHT11_HwInterface *interface) {
    hw = *interface;
}

// Start an asynchronous read
bool DHT11_StartRead(void) {

}

// Check if data is ready
bool DHT11_DataReady(void) {

}

// Get the last reading
DHT11_Data DHT11_GetData(void) {

}

// int DHT11_Init(void) {

// }

// int DHT11_Decode(uint16_t *timestamps, uint8_t *data) {
//     if (!timestamps || !data) return DHT11_ERROR_PARAM; // invalid pointers

//     const uint16_t threshold = 100; // Used to measure pulse widths
//     memset(data, 0, 5);

//     for (int i = 2; i < 42; ++i) {
//         uint16_t pulse_width = timestamps[i] - timestamps[i - 1];
//         int bit_index = i - 2;
//         if (pulse_width > threshold)
//             data[bit_index / 8] |= (1 << (7 - (bit_index % 8)));
//     }

//     if ((data[0] + data[1] + data[2] + data[3]) != data[4]) { // Check for data corruption
//         return DHT11_ERROR_CHECKSUM;
//     } else {
//         return DHT11_OK;
//     }
// }

// int DHT11_Read(DHT11_Data *out) {
    
// }
