#include "recovery.h"
#include "eeprom.h"
#include "network.h"
#include "ota.h"

struct bootflags
{
    unsigned char raw_rst_cause : 4;
    unsigned char raw_bootdevice : 4;
    unsigned char raw_bootmode : 4;

    unsigned char rst_normal_boot : 1;
    unsigned char rst_reset_pin : 1;
    unsigned char rst_watchdog : 1;

    unsigned char bootdevice_ram : 1;
    unsigned char bootdevice_flash : 1;
};

struct bootflags bootmode_detect(void) {
    int reset_reason, bootmode;
    asm (
        "movi %0, 0x60000600\n\t"
        "movi %1, 0x60000200\n\t"
        "l32i %0, %0, 0x114\n\t"
        "l32i %1, %1, 0x118\n\t"
        : "+r" (reset_reason), "+r" (bootmode) /* Outputs */
        : /* Inputs (none) */
        : "memory" /* Clobbered */
    );

    struct bootflags flags;

    flags.raw_rst_cause = (reset_reason&0xF);
    flags.raw_bootdevice = ((bootmode>>0x10)&0x7);
    flags.raw_bootmode = ((bootmode>>0x1D)&0x7);

    flags.rst_normal_boot = flags.raw_rst_cause == 0x1;
    flags.rst_reset_pin = flags.raw_rst_cause == 0x2;
    flags.rst_watchdog = flags.raw_rst_cause == 0x4;

    flags.bootdevice_ram = flags.raw_bootdevice == 0x1;
    flags.bootdevice_flash = flags.raw_bootdevice == 0x3;

    return flags;
}

void recovery_loop()
{
    /* Recovery wifi setup */
    int wifi_stat = wifi_init(1);
    ota_setup();

    while(true) {
        ESP.wdtFeed();
        ota_loop();
    }
}

void recovery_init()
{
    ESP.wdtFeed();
    struct bootflags flags = bootmode_detect();

    Serial.println("");

    if (flags.rst_normal_boot) {
        Serial.println("Normal booted.");
        eeprom_write_byte(ADDR_BOOT, 0);
    } else if (flags.rst_reset_pin) {
        Serial.println("Booted from reset.");
        eeprom_write_byte(ADDR_BOOT, 0);
    }else if (flags.rst_watchdog) {
        uint8_t wdt_reset_times = eeprom_read_byte(ADDR_BOOT);
        Serial.println("Recover from watchdog.");
        Serial.println(wdt_reset_times);

        if (wdt_reset_times < MAX_WDT_RESET_TIME) {
            eeprom_write_byte(ADDR_BOOT, wdt_reset_times + 1);
        } else {
            Serial.println("Too many watchdog reset.");
            recovery_loop();
        }
    }
}
