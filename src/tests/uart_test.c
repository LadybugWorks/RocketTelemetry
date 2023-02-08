#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "../serial.h"

int main(int argc, const char **argv)
{
    printf("UART Test\n");
#if 0
    struct termios tty;
    int fd;
    int flags = O_RDWR | O_NOCTTY | O_NONBLOCK;
    
    fd = open("/dev/ttyS1", flags);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open port\n");
        return -1;
    }
    
    tcgetattr(fd, &tty);
    tty.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR | ICRNL | IGNCR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        fprintf(stderr, "error %d from tcsetattr\n", errno);
        return -1;
    }
    
    for (;;)
    {
        printf("wrote hello.\n");
        write(fd, "HELLO", 4);
        sleep(1);
    }
#else
    
    
    serial_t uart1;
    if (serial_open(&uart1, "/dev/ttyS1", SERIAL_BAUDRATE_115200) == -1)
    {
        fprintf(stderr, "ERROR\tFailed to open serial port.\n");
        return -1;
    }
    
    for (int i = 0; i < 16; i++)
        serial_write("TESLA\n", 6, &uart1);
    
    serial_close(&uart1);
    
#endif
    return 0;
}