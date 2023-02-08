/* date = February 7th 2023 11:05 am */

#ifndef RADIO_H
#define RADIO_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

typedef struct radio_t
{
    pthread_mutex_t mutex;
    FILE *stream;
} radio_t;

// operation procs
void radio_init(radio_t *radio);
void radio_open(radio_t *radio, const char *dev);
void radio_close(radio_t *radio);
void radio_send(radio_t *radio, const void *data, size_t size);
size_t radio_recv(radio_t *radio, void *buffer, size_t size);
void radio_flush(radio_t *radio);
// config procs
int radio_begin_cfg(radio_t *radio);
void radio_end_cfg(radio_t *radio);
void radio_set_netid(radio_t *radio, uint32_t netid);
void radio_set_txpower(radio_t *radio, uint32_t power);
void radio_set_minfreq(radio_t *radio, uint32_t freq);
void radio_set_maxfreq(radio_t *radio, uint32_t freq);
void radio_set_nodeid(radio_t *radio, uint32_t nodeid);
// command procs
void radio_setreg(radio_t *radio, uint8_t n, uint32_t value);
void radio_reboot(radio_t *radio);
void radio_save_cfg(radio_t *radio);
void radio_reset_cfg(radio_t *radio);

#endif //RADIO_H
