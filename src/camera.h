/* date = February 7th 2023 11:38 am */

#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <pthread.h>

typedef struct camera_t
{
    pthread_mutex_t mutex;
    FILE *stream;
} camera_t;

void camera_init(camera_t *camera);
void camera_open(camera_t *camera, const char *dev);
void camera_close(camera_t *camera);
void camera_start_recording(camera_t *camera);
void camera_stop_recording(camera_t *camera);

#endif //CAMERA_H