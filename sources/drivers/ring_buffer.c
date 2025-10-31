#include <stdio.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBufferU16* rb)
{
    if (!rb) return false;
     
    rb->head = rb->tail = 0; 
    
    return true;
}

void RingBuffer_Push(RingBufferU16* rb, uint16_t v)
{
    uint16_t h = atomic_load_explicit(&rb->head, memory_order_relaxed);
    uint16_t t = atomic_load_explicit(&rb->tail, memory_order_acquire);
    uint16_t next = (uint16_t)((h + 1) & RB_MASK);
    if (next == t) return;             // full

    rb->buf[h] = v;                          // write data
    atomic_store_explicit(&rb->head, next, memory_order_release); // publish
}

bool RingBuffer_Pop(RingBufferU16* rb, uint16_t* out)
{
    uint16_t t = atomic_load_explicit(&rb->tail, memory_order_relaxed);
    uint16_t h = atomic_load_explicit(&rb->head, memory_order_acquire);
    if (t == h) return false;                // empty

    *out = rb->buf[t];                       // consume after acquire
    uint16_t next = (uint16_t)((t + 1) & RB_MASK);
    atomic_store_explicit(&rb->tail, next, memory_order_release); // publish
    return true;
}
