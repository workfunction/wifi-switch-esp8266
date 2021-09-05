#include "server.h"
#include "eeprom.h"
#include "network.h"
#include "ota.h"
#include "recovery.h"
#include "utli.h"

void setup(void)
{
    pinMode(GPIO_PIN, OUTPUT);
    led_blink_slow(LED_BUILTIN);
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    recovery_init();

    int wifi_stat = wifi_init(0);
    if (wifi_stat == WIFI_STAT_SETUP) {
        led_blink_fast(LED_BUILTIN);
    } else if (wifi_stat == WIFI_STAT_CONN) {
        led_light(LED_BUILTIN);
    }
    ota_setup();
    server_init();
    //while (1) {};
}

void loop(void)
{
    ESP.wdtFeed();
    ota_loop();
    server_loop();
}
