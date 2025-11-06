#include "tim.h"
#include "irq.h"

void TIM_EnableClock(TIM_TypeDef *tim) {
    if (tim == TIM1) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (tim == TIM5) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    } else if (tim == TIM6) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    } else if (tim == TIM9) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (tim == TIM11) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    }
}

void TIM_DisableClock(TIM_TypeDef *tim) {
    if (tim == TIM1) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
    } else if (tim == TIM5) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;
    } else if (tim == TIM6) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;
    } else if (tim == TIM9) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;
    } else if (tim == TIM11) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;
    }
}

void TIM_BASE_Configure(TIM_TypeDef *tim, TIM_BaseConfig *config) {
    // Configure prescaler and auto-reload
    tim->PSC = config->prescaler;
    tim->ARR = config->arr;

    if (config->interrupts != TIM_INTERRUPT_NONE) {
        if (config->interrupts & TIM_INTERRUPT_UPDATE) {
            tim->DIER |= TIM_DIER_UIE;
        }
        if (config->interrupts & TIM_INTERRUPT_CC) {
            tim->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE;
        }
        if (config->interrupts & TIM_INTERRUPT_TRIGGER) {
            tim->DIER |= TIM_DIER_TIE;
        }
        if (config->interrupts & TIM_INTERRUPT_BREAK) {
            tim->DIER |= TIM_DIER_BIE;
        }
        if (config->interrupts & TIM_INTERRUPT_COM) {
            tim->DIER |= TIM_DIER_COMIE;
        }
    }

    // Initialize registers
    tim->EGR |= TIM_EGR_UG;
}

void TIM_EnableCounter(TIM_TypeDef *tim) {
    tim->CR1 |= TIM_CR1_CEN;
}

void TIM_DisableCounter(TIM_TypeDef *tim) {
    tim->CR1 &= ~TIM_CR1_CEN;
}

void TIM_ResetCounter(TIM_TypeDef *tim) {
    tim->CNT = 0;
}

void TIM6_DAC_IRQHandler(void) {
    if (TIM6->SR & TIM_SR_UIF) {
        TIM6->SR &= ~TIM_SR_UIF; // Clear interrupt flag

        // Call registered callback
        IRQ_Callback callback = IRQ_GetCallback(TIM6_DAC_IRQn);
        if (callback) {
            callback();
        }

    }
}
