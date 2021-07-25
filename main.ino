#include "server.h"
#include "eeprom.h"
#include "wifi.h"

void setup(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    int wifi_stat = wifi_init();
    server_init();
}

void loop(void)
{
    server_loop();
}
