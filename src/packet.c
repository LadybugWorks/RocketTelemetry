#include "packet.h"

#include <string.h>

void packet_buffer_init(packet_buffer_t *buffer)
{
    memset(buffer, 0, sizeof(packet_buffer_t));
    sem_init(&buffer->semaphore, 0, 0);
    pthread_mutex_init(&buffer->mutex, NULL);
    
    // setup free list
    buffer->free_head = &buffer->pool[0];
    for (int i = 0; i < PACKET_BUFFER_SIZE - 1; i++)
        buffer->pool[i].next = &buffer->pool[i + 1];
}

void packet_buffer_push(packet_buffer_t *buffer, const packet_t *packet)
{
    packet_t *new_packet;
    pthread_mutex_lock(&buffer->mutex);
    {
        // alloc from pool
        if (buffer->free_head == NULL)
        {
            // timeout old packets if we run out
            // this is _dangerous_
            // maybe we should warn somebody?
            // @todo log warning
            sem_wait(&buffer->semaphore); // don't leave a resource hanging!
            buffer->free_head = buffer->fifo_head;
            buffer->free_head->next = NULL;
        }
        new_packet = buffer->free_head;
        buffer->free_head = buffer->free_head->next;
        
        // push to fifo
        buffer->fifo_tail->next = new_packet;
        new_packet->next = NULL;
    }
    pthread_mutex_unlock(&buffer->mutex);
    
    // blit into the packet and submit it
    memcpy(new_packet, packet, sizeof(packet_t));
    sem_post(&buffer->semaphore);
}

packet_t packet_buffer_next(packet_buffer_t *buffer)
{
    // wait for a packet
    sem_wait(&buffer->semaphore);
    
    packet_t packet;
    pthread_mutex_lock(&buffer->mutex);
    {
        // read from fifo
        packet_t *fifo_head = buffer->fifo_head;
        packet = *fifo_head;
        buffer->fifo_head = buffer->fifo_head->next;
        
        // free to pool
        fifo_head->next = buffer->free_head;
        buffer->free_head = fifo_head;
    }
    pthread_mutex_unlock(&buffer->mutex);
    
    return packet;
}
