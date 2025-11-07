#include "dma.h"
#include "irq.h"
#include <stdbool.h>

// Arrays for IRQs
static const IRQn_Type dma1_irqs[] = {
    DMA1_Stream0_IRQn,
    DMA1_Stream1_IRQn,
    DMA1_Stream2_IRQn,
    DMA1_Stream3_IRQn,
    DMA1_Stream4_IRQn,
    DMA1_Stream5_IRQn,
    DMA1_Stream6_IRQn,
    DMA1_Stream7_IRQn
};

static const IRQn_Type dma2_irqs[] = {
    DMA2_Stream0_IRQn,
    DMA2_Stream1_IRQn,
    DMA2_Stream2_IRQn,
    DMA2_Stream3_IRQn,
    DMA2_Stream4_IRQn,
    DMA2_Stream5_IRQn,
    DMA2_Stream6_IRQn,
    DMA2_Stream7_IRQn
};

IRQn_Type DMA_GetIRQ(DMA_Stream_TypeDef *stream) {
    if ((uint32_t)stream >= (uint32_t)DMA1_Stream0 && (uint32_t)stream <= (uint32_t)DMA1_Stream7) { // Check if stream address is in DMA1 range
        uint32_t index = ((uint32_t)stream - (uint32_t)DMA1_Stream0) / sizeof(DMA_Stream_TypeDef);
        return dma1_irqs[index];
    } else if ((uint32_t)stream >= (uint32_t)DMA2_Stream0 && (uint32_t)stream <= (uint32_t)DMA2_Stream7) { // Check if stream address is in DMA2 range
        uint32_t index = ((uint32_t)stream - (uint32_t)DMA2_Stream0) / sizeof(DMA_Stream_TypeDef);
        return dma2_irqs[index];
    }
    return (IRQn_Type)-1; // Invalid stream
}

void DMA_EnableClock(DMA_TypeDef *dma) {
    if (dma == DMA1) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    } else if (dma == DMA2) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    }
}
void DMA_DisableClock(DMA_TypeDef *dma) {
    if (dma == DMA1) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
    } else if (dma == DMA2) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
    }
}

void DMA_Configure(DMA_Stream_TypeDef *stream, DMA_Configuration *config) {
    // Select channel
    stream->CR &= ~DMA_SxCR_CHSEL;
    stream->CR |= (config->channel << DMA_SxCR_CHSEL_Pos);

    // Set memory and peripheral data sizes
    stream->CR &= ~DMA_SxCR_MSIZE;
    stream->CR |= (config->memorySize << DMA_SxCR_MSIZE_Pos);
    stream->CR &= ~DMA_SxCR_PSIZE;
    stream->CR |= (config->peripheralSize << DMA_SxCR_PSIZE_Pos);

    // Set memory and peripheral increment
    stream->CR &= ~DMA_SxCR_MINC;
    stream->CR |= (config->memoryIncrease << DMA_SxCR_MINC_Pos);
    stream->CR &= ~DMA_SxCR_PINC;
    stream->CR |= (config->peripheralIncrease << DMA_SxCR_PINC_Pos);

    // Set transfer direction
    stream->CR &= ~DMA_SxCR_DIR;
    stream->CR |= (config->transferDirection << DMA_SxCR_DIR_Pos);

    // Set number of transfers
    stream->NDTR = config->numberOfDataTransfers;

    // Set memory and peripheral address
    stream->PAR = config->peripheralAddress;
    stream->M0AR = config->memoryAddress;

    // Enable interrupts
    if (config->interrupts != DMA_INTERRUPT_NONE) {
        if (config->interrupts & DMA_INTERRUPT_HT) {
        stream->CR |= DMA_SxCR_HTIE;
        }
        if (config->interrupts & DMA_INTERRUPT_TC) {
            stream->CR |= DMA_SxCR_TCIE;
        }
        if (config->interrupts & DMA_INTERRUPT_TE) {
            stream->CR |= DMA_SxCR_TEIE;
        }
        if (config->interrupts & DMA_INTERRUPT_DME) {
            stream->CR |= DMA_SxCR_DMEIE;
        }
        if (config->interrupts & DMA_INTERRUPT_FE) {
            stream->FCR |= DMA_SxFCR_FEIE;
        }
    }
}

void DMA_EnableStream(DMA_Stream_TypeDef *stream) {
    stream->CR |= DMA_SxCR_EN;
}

void DMA_DisableStream(DMA_Stream_TypeDef *stream) {
    stream->CR &= ~DMA_SxCR_EN;
    while (stream->CR & DMA_SxCR_EN);     // Wait until stream is disabled
}


// IRQ Handlers
void DMA2_Stream0_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF0) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;

        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream0_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream1_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF1) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF1;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream1_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream2_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF2) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream2_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream3_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF3) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream3_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream4_IRQHandler(void) {
    if (DMA2->LISR & DMA_HISR_TCIF4) {
        DMA2->LIFCR |= DMA_HIFCR_CTCIF4;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream4_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream5_IRQHandler(void) {
    if (DMA2->LISR & DMA_HISR_TCIF5) {
        DMA2->LIFCR |= DMA_HIFCR_CTCIF5;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream5_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream6_IRQHandler(void) {
    if (DMA2->LISR & DMA_HISR_TCIF6) {
        DMA2->LIFCR |= DMA_HIFCR_CTCIF6;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream6_IRQn);
        if (callback) callback();
    }
}

void DMA2_Stream7_IRQHandler(void) {
    if (DMA2->LISR & DMA_HISR_TCIF7) {
        DMA2->LIFCR |= DMA_HIFCR_CTCIF7;
    
        IRQ_Callback callback = IRQ_GetCallback(DMA2_Stream7_IRQn);
        if (callback) callback();
    }
}
