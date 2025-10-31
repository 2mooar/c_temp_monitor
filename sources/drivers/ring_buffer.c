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
    uint16_t h = rb->head;
    uint16_t next = (uint16_t)((h + 1) & RB_MASK);
    
    if(next != rb->tail) /* drop if full, for simplicity*/
    { 
        rb->buf[h] = v; rb->head = next; 
    } 
}

bool RingBuffer_Pop(RingBufferU16* rb, uint16_t* out)
{
    uint16_t t = rb->tail;
    
    if(t == rb->head) return false;
    
    *out = rb->buf[t];
    
    rb->tail = (uint16_t)((t + 1) & RB_MASK);
    
    return true;
}
