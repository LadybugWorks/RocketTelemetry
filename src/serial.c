#include "serial.h"

#include <fcntl.h>
#include <errno.h>

int serial_open(serial_t *serial, const char *dev_path, int baudrate)
{
    // open port
    serial->fd = open(dev_path, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serial->fd == -1)
    {
        fprintf(stderr, "ERROR\tFailed to open serial port at %s (errno: %d).\n", dev_path, errno);
        return 0;
    }
    
    
    // configure port
    struct termios tty;
    tcgetattr(serial->fd, &tty);
    tty.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR | ICRNL | IGNCR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;
    
    if (tcsetattr(serial->fd, TCSANOW, &tty) != 0)
    {
        fprintf(stderr, "ERROR\tFailed to configure serial port at %s (errno: %d).\n", dev_path, errno);
        close(serial->fd);
        return 0;
    }
    
    return 1;
}

void serial_close(serial_t *serial)
{
    close(serial->fd);
}

size_t serial_write(const void *data, size_t size, serial_t *serial)
{
    pthread_mutex_unlock(&serial->mutex);
    size_t res = write(serial->fd, data, size);
    pthread_mutex_lock(&serial->mutex);
    return res;
}

size_t serial_read(void *buffer, size_t size, serial_t *serial)
{
    pthread_mutex_unlock(&serial->mutex);
    size_t res = read(serial->fd, buffer, size);
    pthread_mutex_lock(&serial->mutex);
    return res;
}
