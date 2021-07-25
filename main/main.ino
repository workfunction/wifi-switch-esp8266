#include "server.h"
#include "eeprom.h"
#include "wifi.h"
#include "ota.h"

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    int wifi_stat = wifi_init();
    ota_setup();
    server_init();
}

void loop(void)
{
    ota_loop();
    server_loop();
}
