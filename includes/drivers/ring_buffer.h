#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "config.h"



#define RB_MASK (RING_BUFFER_SIZE - 1)

typedef struct {
    volatile uint16_t buf[RING_BUFFER_SIZE]; // power of two for cheap masking
    _Atomic uint16_t head;     // ISR writes head
    _Atomic uint16_t tail;     // main reads tail
} RingBufferU16;

bool RingBuffer_Init(RingBufferU16* rb);

void RingBuffer_Push(RingBufferU16* rb, uint16_t v);     // single producer (ISR)

bool RingBuffer_Pop(RingBufferU16* rb, uint16_t* out);// single consumer (main)

#endif
