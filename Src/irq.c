#include "irq.h"
#include <stddef.h>

#define MAX_IRQS 96
static IRQ_Callback irq_callbacks[MAX_IRQS] = {0};

void IRQ_RegisterCallback(IRQn_Type irq, IRQ_Callback callback) {
    if (irq >= 0 && irq < MAX_IRQS) {
        irq_callbacks[irq] = callback; NVIC_EnableIRQ(irq); // Enable the interrupt in NVIC
    }
}

IRQ_Callback IRQ_GetCallback(IRQn_Type irq) {
    if (irq >= 0 && irq < MAX_IRQS) {
        return irq_callbacks[irq];
    }
    return NULL;
}
