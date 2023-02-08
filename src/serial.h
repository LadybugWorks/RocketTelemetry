/* date = February 7th 2023 7:08 pm */

#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>

#define SERIAL_BAUDRATE_57600 B57600
#define SERIAL_BAUDRATE_115200 B115200

typedef struct serial_t
{
    pthread_mutex_t mutex;
    int fd; // file descriptor to serial device
} serial_t;

int serial_open(serial_t *serial, const char *dev_path, int baudrate);
void serial_close(serial_t *serial);
ssize_t serial_write(const void *data, size_t size, serial_t *serial);
ssize_t serial_read(void *buffer, size_t size, serial_t *serial);

#endif //SERIAL_H
