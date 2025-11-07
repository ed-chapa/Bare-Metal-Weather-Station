#ifndef DMA_H
#define DMA_H

#include "stm32f4xx.h"

typedef enum {
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_2 = 2,
    DMA_CHANNEL_3 = 3,
    DMA_CHANNEL_4 = 4,
    DMA_CHANNEL_5 = 5,
    DMA_CHANNEL_6 = 6,
    DMA_CHANNEL_7 = 7
} DMA_Channel;

typedef enum {
    DMA_MSIZE_BYTE = 0,
    DMA_MSIZE_HALFWORD = 1,
    DMA_MSIZE_WORD = 2
} DMA_MemorySize;

typedef enum {
    DMA_PSIZE_BYTE = 0,
    DMA_PSIZE_HALFWORD = 1,
    DMA_PSIZE_WORD = 2
} DMA_PeripheralSize;

typedef enum {
    DMA_MINC_DISABLE = 0,
    DMA_MINC_ENABLE = 1
} DMA_MemoryIncrease;

typedef enum {
    DMA_PINC_DISABLE = 0,
    DMA_PINC_ENABLE = 1
} DMA_PeripheralIncrease;

typedef enum {
    DMA_DIRECTION_PERIPHERAL_TO_MEMORY = 0,
    DMA_DIRECTION_MEMORY_TO_PERIPHERAL = 1,
    DMA_DIRECTION_MEMORY_TO_MEMORY = 2
} DMA_Direction;

typedef enum {
    DMA_INTERRUPT_NONE = 0,
    DMA_INTERRUPT_TC = 1,
    DMA_INTERRUPT_HT = 2,
    DMA_INTERRUPT_TE = 4,
    DMA_INTERRUPT_DME = 8,
    DMA_INTERRUPT_FE = 10,
    DMA_INTERRUPT_ALL = 11 
} DMA_Interrupt;

typedef struct {
    DMA_Stream_TypeDef *stream;
    DMA_Channel channel;
    DMA_MemorySize memorySize;
    DMA_PeripheralSize peripheralSize;
    DMA_MemoryIncrease memoryIncrease;
    DMA_PeripheralIncrease peripheralIncrease;
    DMA_Direction transferDirection;
    DMA_Interrupt interrupts;
    uint32_t memoryAddress;
    uint32_t peripheralAddress;
    uint16_t numberOfDataTransfers;
} DMA_Configuration;

void DMA_EnableClock(DMA_TypeDef *dma);
void DMA_DisableClock(DMA_TypeDef *dma);
void DMA_Configure(DMA_Stream_TypeDef *stream, DMA_Configuration *config);
void DMA_EnableStream(DMA_Stream_TypeDef *stream);
void DMA_DisableStream(DMA_Stream_TypeDef *stream);

#endif
