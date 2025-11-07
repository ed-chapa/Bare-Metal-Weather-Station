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
        if (config->interrupts & TIM_INTERRUPT_CC1) {
            tim->DIER |= TIM_DIER_CC1IE;
        }
        if (config->interrupts & TIM_INTERRUPT_CC2) {
            tim->DIER |= TIM_DIER_CC2IE;
        }
        if (config->interrupts & TIM_INTERRUPT_CC3) {
            tim->DIER |= TIM_DIER_CC3IE;
        }
        if (config->interrupts & TIM_INTERRUPT_CC4) {
            tim->DIER |= TIM_DIER_CC4IE;
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

void TIM_IC_Configure(TIM_TypeDef *tim, TIM_InputCaptureConfig *config) {
    // Configure prescaler and auto-reload
    tim->PSC = config->prescaler;
    tim->ARR = config->arr;

    // Configure channel
    switch (config->channel) {
        case TIM_CHANNEL_1:
            tim->CCMR1 &= ~TIM_CCMR1_CC1S;
            tim->CCMR1 |= (config->icSelection << TIM_CCMR1_CC1S_Pos);
            tim->CCMR1 &= ~TIM_CCMR1_IC1F;
            tim->CCMR1 |= (config->icFilter << TIM_CCMR1_IC1F_Pos);

            tim->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
            tim->CCER |= config->icPolarity;

            tim->CCER |= TIM_CCER_CC1E;
            if (config->interrupts & TIM_INTERRUPT_CC1) {
                tim->DIER |= TIM_DIER_CC1IE;
            }
            break;

        case TIM_CHANNEL_2:
            tim->CCMR1 &= ~TIM_CCMR1_CC2S;
            tim->CCMR1 |= (config->icSelection << TIM_CCMR1_CC2S_Pos);
            tim->CCMR1 &= ~TIM_CCMR1_IC2F;
            tim->CCMR1 |= (config->icFilter << TIM_CCMR1_IC2F_Pos);

            tim->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
            tim->CCER |= (config->icPolarity << 4); // shifted for CC2

            tim->CCER |= TIM_CCER_CC2E;
            if (config->interrupts & TIM_INTERRUPT_CC2) {
                tim->DIER |= TIM_DIER_CC2IE;
            }
            break;

        case TIM_CHANNEL_3:
            tim->CCMR2 &= ~TIM_CCMR2_CC3S;
            tim->CCMR2 |= (config->icSelection << TIM_CCMR2_CC3S_Pos);
            tim->CCMR2 &= ~TIM_CCMR2_IC3F;
            tim->CCMR2 |= (config->icFilter << TIM_CCMR2_IC3F_Pos);

            tim->CCER &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);
            tim->CCER |= (config->icPolarity << 8);

            tim->CCER |= TIM_CCER_CC3E;
            if (config->interrupts & TIM_INTERRUPT_CC3) {
                tim->DIER |= TIM_DIER_CC3IE;
            }
            break;

        case TIM_CHANNEL_4:
            tim->CCMR2 &= ~TIM_CCMR2_CC4S;
            tim->CCMR2 |= (config->icSelection << TIM_CCMR2_CC4S_Pos);
            tim->CCMR2 &= ~TIM_CCMR2_IC4F;
            tim->CCMR2 |= (config->icFilter << TIM_CCMR2_IC4F_Pos);

            tim->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
            tim->CCER |= (config->icPolarity << 12);

            tim->CCER |= TIM_CCER_CC4E;
            if (config->interrupts & TIM_INTERRUPT_CC4) {
                tim->DIER |= TIM_DIER_CC4IE;
            }
            break;
    }

    // Generate update event to load PSC/ARR
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

// TIM1 Update Interrupt Handler
void TIM1_UP_TIM10_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF; // Clear update flag

        IRQ_Callback callback = IRQ_GetCallback(TIM1_UP_IRQn);
        if (callback) {
            callback();
        }
    }
}

// TIM1 Capture/Compare Interrupt Handler
void TIM1_CC_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_CC1IF) {
        TIM1->SR &= ~TIM_SR_CC1IF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_CC_IRQn);
        if (callback) callback();
    }
    if (TIM1->SR & TIM_SR_CC2IF) {
        TIM1->SR &= ~TIM_SR_CC2IF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_CC_IRQn);
        if (callback) callback();
    }
    if (TIM1->SR & TIM_SR_CC3IF) {
        TIM1->SR &= ~TIM_SR_CC3IF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_CC_IRQn);
        if (callback) callback();
    }
    if (TIM1->SR & TIM_SR_CC4IF) {
        TIM1->SR &= ~TIM_SR_CC4IF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_CC_IRQn);
        if (callback) callback();
    }
}

// TIM1 Trigger and Commutation Interrupt Handler
void TIM1_TRG_COM_TIM11_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_TIF) {
        TIM1->SR &= ~TIM_SR_TIF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_TRG_COM_TIM11_IRQn);
        if (callback) callback();
    }
    if (TIM1->SR & TIM_SR_COMIF) {
        TIM1->SR &= ~TIM_SR_COMIF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_TRG_COM_TIM11_IRQn);
        if (callback) callback();
    }
}

// TIM1 Break Interrupt Handler
void TIM1_BRK_TIM9_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_BIF) {
        TIM1->SR &= ~TIM_SR_BIF;
        IRQ_Callback callback = IRQ_GetCallback(TIM1_BRK_TIM9_IRQn);
        if (callback) callback();
    }
}
