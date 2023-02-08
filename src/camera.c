#include "camera.h"
#include <stdint.h>
#include <assert.h>

void camera_init(camera_t *camera)
{
    pthread_mutex_init(&camera->mutex, NULL);
}

void camera_open(camera_t *camera, const char *dev)
{
    camera->stream = fopen(dev, "a+");
    assert(camera->stream != NULL);
    printf("stream=%p\n", camera->stream);
}

void camera_close(camera_t *camera)
{
    fclose(camera->stream);
}

void camera_start_recording(camera_t *camera)
{
    static uint8_t packet[4] = { 0xCC, 0x01, 0x03, 0x98 };
    pthread_mutex_lock(&camera->mutex);
    fwrite(packet, 1, sizeof(packet), camera->stream);
    pthread_mutex_unlock(&camera->mutex);
}

void camera_stop_recording(camera_t *camera)
{
    static uint8_t packet[4] = { 0xCC, 0x01, 0x04, 0xCC };
    pthread_mutex_lock(&camera->mutex);
    fwrite(packet, 1, sizeof(packet), camera->stream);
    pthread_mutex_unlock(&camera->mutex);
}
