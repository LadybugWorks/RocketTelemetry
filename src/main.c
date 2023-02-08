#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "packet.h"
#include "radio.h"
#include "camera.h"

//
// Configs
//

#define RADIO_DEV_PATH "/dev/tty00"
#define CAMERA_DEV_PATH "/dev/tty00"

#define RADIO_NETID   42
#define RADIO_TXPOWER 0
#define RADIO_MINFREQ 0
#define RADIO_MAXFREQ 0

//
// Data
//

radio_t radio;
camera_t camera;

//
// Procs
//

void radio_interrupt(int signal)
{
    // read from radio
}

void* do_inbound_handling(void *arg)
{
    packet_buffer_t *buffer = (packet_buffer_t*)arg;
    
    while (1)
    {
        packet_t packet = packet_buffer_next(buffer);
        
        // handle packet
    }
}

void* do_outbound_handling(void *arg)
{
    packet_buffer_t *buffer = (packet_buffer_t*)arg;
    
    while (1)
    {
        packet_t packet = packet_buffer_next(buffer);
        
        // write packet to radio
    }
}

int main(int argc, const char **argv)
{
    // @todo handle args
    
    packet_buffer_t inbound_packet_buffer;
    packet_buffer_t outbound_packet_buffer;
    
    packet_buffer_init(&inbound_packet_buffer);
    packet_buffer_init(&outbound_packet_buffer);
    
    pthread_t inbound_handling_thread;
    pthread_t outbound_handling_thread;
    
    pthread_create(&inbound_handling_thread, NULL, do_inbound_handling, (void*)&inbound_packet_buffer);
    pthread_create(&outbound_handling_thread, NULL, do_outbound_handling, (void*)&outbound_packet_buffer);
    
    radio_init(&radio);
    radio_open(&radio, RADIO_DEV_PATH);
    radio_begin_cfg(&radio);
    radio_set_netid(&radio, RADIO_NETID);
    radio_set_txpower(&radio, RADIO_TXPOWER);
    radio_set_minfreq(&radio, RADIO_MINFREQ);
    radio_set_maxfreq(&radio, RADIO_MAXFREQ);
    radio_end_cfg(&radio);
    
    
    camera_init(&camera);
    camera_open(&camera, CAMERA_DEV_PATH);
    
    // handle stdin commands
    // @todo make energy efficient
    while (1)
    {
        printf("Still here.\n");
        sleep(5);
    }
    
    pthread_kill(inbound_handling_thread, SIGKILL);
    pthread_kill(outbound_handling_thread, SIGKILL);
    
    // we don't really need to do this but whatever...
    radio_close(&radio);
    camera_close(&camera);
    
    return 0;
}