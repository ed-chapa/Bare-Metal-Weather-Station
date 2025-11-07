#ifndef TIM_H
#define TIM_H

#include "stm32f4xx.h"

typedef enum {
    TIM_CHANNEL_1 = 0,   // CC1
    TIM_CHANNEL_2 = 1,   // CC2
    TIM_CHANNEL_3 = 2,   // CC3
    TIM_CHANNEL_4 = 3    // CC4
} TIM_Channel;

typedef enum {
    TIM_ICSELECTION_OUTPUT   = 0x0, // CCx as output
    TIM_ICSELECTION_DIRECTTI = 0x1, // CCx mapped to TIx
    TIM_ICSELECTION_INDIRECTTI = 0x2, // CCx mapped to TIy
    TIM_ICSELECTION_TRC      = 0x3  // CCx mapped to TRC
} TIM_ICSelection;

typedef enum {
    TIM_ICPOLARITY_RISING  = 0x0,               // CCxP = 0
    TIM_ICPOLARITY_FALLING = TIM_CCER_CC1P,     // CCxP = 1
    TIM_ICPOLARITY_BOTH    = TIM_CCER_CC1P | TIM_CCER_CC1NP // both edges
} TIM_ICPolarity;

typedef enum {
    TIM_ICFILTER_NONE          = 0x0,  // No filter, sampling fDTS
    TIM_ICFILTER_FCKINT_N2     = 0x1,  // fSAMPLING=fCK_INT, N=2
    TIM_ICFILTER_FCKINT_N4     = 0x2,  // fSAMPLING=fCK_INT, N=4
    TIM_ICFILTER_FCKINT_N8     = 0x3,  // fSAMPLING=fCK_INT, N=8

    TIM_ICFILTER_FDTS_DIV2_N6  = 0x4,  // fSAMPLING=fDTS/2, N=6
    TIM_ICFILTER_FDTS_DIV2_N8  = 0x5,  // fSAMPLING=fDTS/2, N=8
    TIM_ICFILTER_FDTS_DIV4_N6  = 0x6,  // fSAMPLING=fDTS/4, N=6
    TIM_ICFILTER_FDTS_DIV4_N8  = 0x7,  // fSAMPLING=fDTS/4, N=8

    TIM_ICFILTER_FDTS_DIV8_N6  = 0x8,  // fSAMPLING=fDTS/8, N=6
    TIM_ICFILTER_FDTS_DIV8_N8  = 0x9,  // fSAMPLING=fDTS/8, N=8
    TIM_ICFILTER_FDTS_DIV16_N5 = 0xA,  // fSAMPLING=fDTS/16, N=5
    TIM_ICFILTER_FDTS_DIV16_N6 = 0xB,  // fSAMPLING=fDTS/16, N=6

    TIM_ICFILTER_FDTS_DIV16_N8 = 0xC,  // fSAMPLING=fDTS/16, N=8
    TIM_ICFILTER_FDTS_DIV32_N5 = 0xD,  // fSAMPLING=fDTS/32, N=5
    TIM_ICFILTER_FDTS_DIV32_N6 = 0xE,  // fSAMPLING=fDTS/32, N=6
    TIM_ICFILTER_FDTS_DIV32_N8 = 0xF   // fSAMPLING=fDTS/32, N=8 (maximum filter)
} TIM_ICFilter;

typedef enum {
    TIM_INTERRUPT_NONE    = 0x00,
    TIM_INTERRUPT_UPDATE  = TIM_DIER_UIE,   // Update interrupt
    TIM_INTERRUPT_CC1     = TIM_DIER_CC1IE, // Capture/Compare 1
    TIM_INTERRUPT_CC2     = TIM_DIER_CC2IE, // Capture/Compare 2
    TIM_INTERRUPT_CC3     = TIM_DIER_CC3IE, // Capture/Compare 3
    TIM_INTERRUPT_CC4     = TIM_DIER_CC4IE, // Capture/Compare 4
    TIM_INTERRUPT_TRIGGER = TIM_DIER_TIE,   // Trigger
    TIM_INTERRUPT_BREAK   = TIM_DIER_BIE,   // Break (advanced timers)
    TIM_INTERRUPT_COM     = TIM_DIER_COMIE, // Commutation (advanced timers)
    TIM_INTERRUPT_ALL     = 0xFFFF          // All interrupts
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
    TIM_Interrupt interrupts;
    uint8_t channel;          // CC channel (1–4)
    uint32_t icPolarity;      // Rising/Falling/Both
    uint32_t icSelection;     // Direct/Indirect/Trigger
    uint32_t icFilter;        // Input filter
} TIM_InputCaptureConfig;

void TIM_EnableClock(TIM_TypeDef *tim);
void TIM_DisableClock(TIM_TypeDef *tim);
void TIM_BASE_Configure(TIM_TypeDef *tim, TIM_BaseConfig *config);
void TIM_IC_Configure(TIM_TypeDef *tim, TIM_InputCaptureConfig *config);
void TIM_EnableCounter(TIM_TypeDef *tim);
void TIM_DisableCounter(TIM_TypeDef *tim);
void TIM_ResetCounter(TIM_TypeDef *tim);

#endif
