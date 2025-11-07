#include "interrupt.h"

#include "globals.h"

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
