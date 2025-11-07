#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

extern volatile uint8_t transmission_started;
extern volatile uint8_t ready_to_receive;
extern volatile uint8_t data_received;
extern volatile uint32_t tick_count;
extern volatile uint32_t start_pulse;
extern volatile uint8_t time_index;

extern char i2c_buffer[128];
extern uint8_t decoded_data[5];
extern uint16_t timestamps[43];

#endif
