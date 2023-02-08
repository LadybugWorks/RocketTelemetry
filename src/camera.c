#include "camera.h"
#include <stdint.h>
#include <unistd.h>

#define packed_struct __attribute__((packed))

//
// RunCam Protocol (RCP)
//

// Values are from:
// https://support.runcam.com/hc/en-us/articles/360014537794-RunCam-Device-Protocol

#define RCP_PACKET_HEADER 0xCC
// This is only here to denote where the crc would go
#define RCP_CRC 0x00

#define RCP_CMD_GET_DEVICE_INFO 0x00 //
#define RCP_CMD_CAMERA_CONTROL 0x01 //
//#define RCP_CMD_5KEY_SIMULATION_PRESS 0x02
//#define RCP_CMD_5KEY_SIMULATION_RELEASE 0x03
//#define RCP_CMD_5KEY_CONNECTION 0x05
//#define RCP_CMD_REQUEST_FC_ATTITUDE 0x50
#define RCP_CMD_GET_SETTINGS 0x10 //
#define RCP_CMD_READ_SETTING_DETAIL 0x11 //
#define RCP_CMD_WRITE_SETTING 0x13 //
#define RCP_CMD_DISP_FILL_REGION 0x20 //
#define RCP_CMD_DISP_WRITE_CHAR 0x21 //
#define RCP_CMD_DISP_WRITE_HORT_STRING 0x22 //
#define RCP_CMD_DISP_WRITE_VERT_STRING 0x23 //
#define RCP_CMD_DISP_WRITE_CHARS 0x24 //

#define RCP_FEATURE_SIMULATE_POWER_BUTTON (1<<0)
#define RCP_FEATURE_SIMULATE_WIFI_BUTTON (1<<1)
#define RCP_FEATURE_CHANGE_MODE (1<<2)
#define RCP_FEATURE_SIMULATE_5_KEY_OSD_CABLE (1<<3)
#define RCP_FEATURE_DEVICE_SETTINGS_ACCESS (1<<4)
#define RCP_FEATURE_DISPLAYP_PORT (1<<5)
#define RCP_FEATURE_START_RECORDING (1<<6)
#define RCP_FEATURE_STOP_RECORDING (1<<7)
#define RCP_FEATURE_FC_ATTITUDE (1<<9)

#define RCP_ACTION_SIMULATE_WIFI_BTN 0x00
#define RCP_ACTION_SIMULATE_POWER_BTN 0x01
#define RCP_ACTION_CHANGE_MODE 0x02
#define RCP_ACTION_CHANGE_START_RECORDING 0x03
#define RCP_ACTION_CHANGE_STOP_RECORDING 0x04

#define RCP_SETTING_DISP_CHARSET 0
#define RCP_SETTING_DISP_COLUMNS 1
#define RCP_SETTING_DISP_TV_MODE 2
#define RCP_SETTING_DISP_SDCARD_CAPACIT 3
#define RCP_SETTING_DISP_REMAIN_RECORDING_TIME 4
#define RCP_SETTING_DISP_RESOLUTION 5
#define RCP_SETTING_DISP_CAMERA_TIME 6

// sets the CRC at the end of a packet
#define rcp_set_crc(pkt) pkt[sizeof(pkt) - 1] = crc8_dvb_s2_buffer(pkt, sizeof(pkt) - 1)

//
// Helper Procs
//

// Source: 
// RunCam Protocol
// https://support.runcam.com/hc/en-us/articles/360014537794-RunCam-Device-Protocol
uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a)
{
    crc ^= a;
    for (int ii = 0; ii < 8; ++ii) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0xD5;
        } else {
            crc = crc << 1;
        }
    }
    
    return crc;
}

// This just wraps the crc8_dvb_s2(..) so it works with a buffer
static inline uint8_t crc8_dvb_s2_buffer(void *buffer, size_t size)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < size; i++)
        crc = crc8_dvb_s2(crc, ((uint8_t*)buffer)[i]);
    return crc;
}

//
// Camera Interface
//

// Lifetime Stuff

void camera_init(camera_t *camera)
{
}

int camera_open(camera_t *camera, const char *dev_path)
{
    return serial_open(&camera->serial, dev_path, B115200);
}

void camera_close(camera_t *camera)
{
    serial_close(&camera->serial);
}

// Commands

// @note(boyd)
// You'll notice some packets have hard-coded CRC,
// that's because they will always be the same.

typedef struct rcp_device_info_t rcp_device_info_t;
__attribute__((packed))
struct rcp_device_info_t
{
    uint8_t header;
    uint8_t protocol_version;
    uint16_t features;
    uint8_t crc;
};

#include <stdio.h>

int camera_get_device_info(camera_t *camera, camera_info_t *info)
{
    static uint8_t packet[] = 
    { 
        RCP_PACKET_HEADER,
        RCP_CMD_GET_DEVICE_INFO,
        0x60
    };
    serial_write(packet, sizeof(packet), &camera->serial);
    
    sleep(1);
    
    rcp_device_info_t dev_info;
    const ssize_t target = 5;
    if (serial_read(&dev_info, target, &camera->serial) != target)
        return -1;
    
    uint8_t crc = crc8_dvb_s2_buffer(&dev_info, target - 1);
    if (crc != dev_info.crc)
        return -2;
    
    info->protocol_version = dev_info.protocol_version;
    info->features = dev_info.features;
    return 0;
}

void camera_start_recording(camera_t *camera)
{
    static uint8_t packet[4] = 
    { 
        RCP_PACKET_HEADER,
        RCP_CMD_CAMERA_CONTROL,
        RCP_ACTION_CHANGE_START_RECORDING,
        0x98
    };
    serial_write(packet, sizeof(packet), &camera->serial);
}

void camera_stop_recording(camera_t *camera)
{
    static uint8_t packet[4] = 
    { 
        RCP_PACKET_HEADER,
        RCP_CMD_CAMERA_CONTROL,
        RCP_ACTION_CHANGE_STOP_RECORDING,
        0xCC
    };
    serial_write(packet, sizeof(packet), &camera->serial);
}
