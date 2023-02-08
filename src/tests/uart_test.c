#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char **argv)
{
    printf("UART Test\n");
    
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
    
    return 0;
}