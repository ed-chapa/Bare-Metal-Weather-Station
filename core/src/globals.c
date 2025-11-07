#include "globals.h"

volatile uint8_t transmission_started = 0;
volatile uint8_t ready_to_receive = 0;
volatile uint8_t data_received = 0;
volatile uint32_t tick_count = 0;
volatile uint32_t start_pulse = 0;
volatile uint8_t time_index = 0;

char i2c_buffer[128] = {0};
uint8_t decoded_data[5] = {0};
uint16_t timestamps[43] = {0};
