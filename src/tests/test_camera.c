#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../camera.h"

#define DEFAULT_RECORD_TIME 10
#define DEFAULT_DEV_PATH "dev/tty01"

int main(int argc, const char **argv)
{
    const char *dev_path = (argc >= 2) ? argv[1] : DEFAULT_DEV_PATH;
    int record_time = (argc >= 3) ? atoi(argv[2]) : DEFAULT_RECORD_TIME;
    
    printf("Camera Test:\n");
    printf("\tdev_path = %s\n", dev_path);
    printf("\trecord_time = %d\n", record_time);
    
    camera_t camera;
    camera_init(&camera);
    camera_open(&camera, dev_path);
    printf("Recording for %d seconds...\n", record_time);
    camera_start_recording(&camera);
    sleep(record_time);
    camera_stop_recording(&camera);
    printf("Recording done!\n");
    return 0;
}