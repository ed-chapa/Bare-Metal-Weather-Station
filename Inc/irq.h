#ifndef IRQ_H
#define IRQ_H

#include "stm32f4xx.h"

typedef void (*IRQ_Callback)(void);

void IRQ_RegisterCallback(IRQn_Type irq, IRQ_Callback callback);
IRQ_Callback IRQ_GetCallback(IRQn_Type irq);

#endif
