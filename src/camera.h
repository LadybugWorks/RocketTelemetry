/* date = February 7th 2023 11:38 am */

#ifndef CAMERA_H
#define CAMERA_H

#include "serial.h"

#include <stdint.h>

typedef struct camera_t
{
    serial_t serial;
} camera_t;

typedef struct camera_info_t
{
    uint8_t protocol_version;
    uint16_t features;
} camera_info_t;

void camera_init(camera_t *camera);
int camera_open(camera_t *camera, const char *dev_path);
void camera_close(camera_t *camera);
int camera_get_device_info(camera_t *camera, camera_info_t *info);
void camera_start_recording(camera_t *camera);
void camera_stop_recording(camera_t *camera);

#endif //CAMERA_H