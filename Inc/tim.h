#ifndef TIM_H
#define TIM_H

#include "stm32f4xx.h"

typedef enum {
    TIM_INTERRUPT_NONE    = 0,  // No interrupt
    TIM_INTERRUPT_UPDATE  = 1,  // Update event (UIF)
    TIM_INTERRUPT_CC      = 2,  // Capture/Compare (CCxIF)
    TIM_INTERRUPT_TRIGGER = 4,  // Trigger event (TIF)
    TIM_INTERRUPT_BREAK   = 8,  // Break (BIF, advanced timers only)
    TIM_INTERRUPT_COM     = 10,  // Commutation (COMIF, advanced timers only)
    TIM_INTERRUPT_ALL     = 11   // All interrupts
} TIM_Interrupt;

typedef struct {
    uint16_t prescaler;
    uint16_t arr;
    uint32_t priority;
    TIM_Interrupt interrupts;
} TIM_BaseConfig;

typedef struct {
    uint16_t prescaler;
    uint16_t arr;
    uint32_t priority;
    TIM_Interrupt interrupts;
    uint8_t channel;          // CC channel (1–4)
    uint32_t icPolarity;      // Rising/Falling/Both
    uint32_t icSelection;     // Direct/Indirect/Trigger
    uint32_t icFilter;        // Input filter
} TIM_InputCaptureConfig;

void TIM_EnableClock(TIM_TypeDef *tim);
void TIM_DisableClock(TIM_TypeDef *tim);
void TIM_BASE_Configure(TIM_TypeDef *tim, TIM_BaseConfig *config);
void TIM_EnableCounter(TIM_TypeDef *tim);
void TIM_DisableCounter(TIM_TypeDef *tim);
void TIM_ResetCounter(TIM_TypeDef *tim);

#endif
