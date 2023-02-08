#include "radio.h"
#include <unistd.h>

#define AT_CMD_GET_VERSION "ATI"
#define AT_CMD_GET_BOARD_TYPE "ATI2"
#define AT_CMD_GET_BOARD_FREQ "ATI3"
#define AT_CMD_GET_BOARD_VERSION "ATI4"
#define AT_CMD_GET_ALL_PARAMS "ATI5"
#define AT_CMD_GET_TDM_TIMING_REPORT "ATI6"
#define AT_CMD_GET_RSSI_SIGNAL_REPORT "ATI7"
#define AT_CMD_EXIT_COMMAND_MODE "ATO"
#define AT_CMD_GET_PARAM "ATS%d?"
#define AT_CMD_SET_PARAM "ATS%d=%d"
#define AT_CMD_REBOOT "ATZ"
#define AT_CMD_WRITE_CONFIG "AT&W"
#define AT_CMD_RESET_CONFIG "AT&F"

#define REG_ID_NETID   3
#define REG_ID_TXPOWER 4
#define REG_ID_MINFREQ 8
#define REG_ID_MAXFREQ 9
#define REG_ID_NODEID  15

/*
ATI Shows the radio version
ATI2 Shows the board type
ATI3 Shows board frequency
ATI4 Shows board version
ATI5 Shows all user settable EEPROM parameters
ATI6 Displays TDM timing report
ATI7 Displays RSSI signal report
ATO Exits AT command mode
ATSn? Displays radio parameter number ‘n’ 
ATSn=X Sets radio parameter number ‘n’ to ‘X’ 
ATZ Reboots the radio
AT&W Writes current parameters to EEPROM 
AT&F Resets all parameters to factory defaults 
AT&T=RSSI Enables RSSI debugging report AT&T=TDM Enables TDM debugging report
AT&T Disables debugging report
*/

#define ENTER_AT_COMMAND_MODE_STR "+++"

void radio_init(radio_t *radio)
{
    pthread_mutex_init(&radio->mutex, NULL);
}

void radio_open(radio_t *radio, const char *dev)
{
    radio->stream = fopen(dev, "w+");
}

void radio_close(radio_t *radio)
{
    fclose(radio->stream);
}

void radio_send(radio_t *radio, const void *data, size_t size)
{
    pthread_mutex_lock(&radio->mutex);
    fwrite(data, 1, size, radio->stream); 
    pthread_mutex_unlock(&radio->mutex);
}

size_t radio_recv(radio_t *radio, void *buffer, size_t size)
{
    pthread_mutex_lock(&radio->mutex);
    size_t res = fread(buffer, 1, size, radio->stream); 
    pthread_mutex_unlock(&radio->mutex);
    return res;
}

void radio_flush(radio_t *radio)
{
    pthread_mutex_lock(&radio->mutex);
    fflush(radio->stream);
    pthread_mutex_unlock(&radio->mutex);
}

int radio_begin_cfg(radio_t *radio)
{
    pthread_mutex_lock(&radio->mutex);
    fprintf(radio->stream, ENTER_AT_COMMAND_MODE_STR);
    sleep(2); // the doc says we should see "OK" after a second
    // @todo wait for OK with timeout
    return 1;
}

void radio_end_cfg(radio_t *radio)
{
    fprintf(radio->stream, AT_CMD_EXIT_COMMAND_MODE);
    pthread_mutex_unlock(&radio->mutex);
}

// @todo sanity check for values
void radio_set_netid(radio_t *radio, uint32_t netid)
{
    radio_setreg(radio, REG_ID_NETID, netid);
}

void radio_set_txpower(radio_t *radio, uint32_t power)
{
    radio_setreg(radio, REG_ID_TXPOWER, power);
}

void radio_set_minfreq(radio_t *radio, uint32_t freq)
{
    radio_setreg(radio, REG_ID_MINFREQ, freq);
}

void radio_set_maxfreq(radio_t *radio, uint32_t freq)
{
    radio_setreg(radio, REG_ID_MAXFREQ, freq);
}

void radio_set_nodeid(radio_t *radio, uint32_t nodeid)
{
    radio_setreg(radio, REG_ID_NODEID, nodeid);
}

// command procs
void radio_setreg(radio_t *radio, uint8_t n, uint32_t value)
{
    fprintf(radio->stream, AT_CMD_SET_PARAM, n, value);
}

void radio_reboot(radio_t *radio)
{
    fprintf(radio->stream, AT_CMD_REBOOT);
    pthread_mutex_unlock(&radio->mutex);
}

void radio_save_cfg(radio_t *radio)
{
    fprintf(radio->stream, AT_CMD_WRITE_CONFIG);
}

void radio_reset_cfg(radio_t *radio)
{
    fprintf(radio->stream, AT_CMD_RESET_CONFIG);
}
