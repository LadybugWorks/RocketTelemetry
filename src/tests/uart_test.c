#include "../serial.h"
#include <stdio.h>
#include <stdint.h>

// This file tests the serial api.
// It'll write "Clemson" to the port,
// then it'll echo any inputs.
//
// Warning!
// Make sure your UART pins are enabled.
// On the beaglebone black you can run
// $ config-pin p9.<pin number> uart
// Or put it in the device tree
//
// Usage:
//     uart_test [device]

int main(int argc, const char **argv)
{
    const char *device = (argc >= 2) ? argv[1] : "/dev/ttyS1";
    
    printf("Testing serial port \"%s\"\n", device);
    printf("Inputs should be echoed back.\n");
    
    serial_t serial;
    if (serial_open(&serial, device, SERIAL_BAUDRATE_115200) == -1)
    {
        fprintf(stderr, "ERROR\tFailed to open \"%s\".\n", device);
        return -1;
    }
    
    serial_write("Clemson\n", 8, &serial);
    
    uint8_t input;
    while (input != 'Z')
    {
        if (serial_read(&input, 1, &serial) == 1)
        {
            putc((char)input, stdout);
            serial_write(&input, 1, &serial); // Echo
        }
    }
    
    serial_close(&serial);
    
    return 0;
}