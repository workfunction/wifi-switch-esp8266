#include <Arduino.h>

enum wifi_status_t
{
    WIFI_STAT_CONN,
    WIFI_STAT_SETUP
};

/**
 * @brief 
 * Initalize wifi connection
 * @return int 
 * 0: wifi connected.
 * 1: saved ssid connection failed, go for AP mode.
 */
int wifi_init();
