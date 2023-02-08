/* date = February 7th 2023 9:23 am */

#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

#define PACKET_DATA_MAX_LENGTH 12
#define PACKET_BUFFER_SIZE 16

#define PACKET_ID_HEARTBEAT 'H'
#define PACKET_ID_COMMAND   'C'
#define PACKET_ID_DATA      'D'
#define PACKET_ID_ACK       'A'

#define PACKET_CMD_SLEEP    'S'
#define PACKET_CMD_WAKEUP   'W'
#define PACKET_CMD_STARTCAM 'R'
#define PACKET_CMD_STOPCAM  'E'

typedef struct packet_t
{
    struct packet_t *next;
    union
    {
        uint8_t raw[1 + PACKET_DATA_MAX_LENGTH];
        struct
        {
            uint8_t id;
            // @todo crc
            uint8_t data[PACKET_DATA_MAX_LENGTH];
        };
    };
} packet_t;

typedef struct packet_buffer_t
{
    pthread_mutex_t mutex;
    sem_t semaphore;
    // pool
    packet_t pool[PACKET_BUFFER_SIZE];
    packet_t *free_head;
    // fifo
    packet_t *fifo_head;
    packet_t *fifo_tail;
} packet_buffer_t;

void packet_buffer_init(packet_buffer_t *buffer);
void packet_buffer_push(packet_buffer_t *buffer, const packet_t *packet);
packet_t packet_buffer_next(packet_buffer_t *buffer);

#endif //PACKET_H
