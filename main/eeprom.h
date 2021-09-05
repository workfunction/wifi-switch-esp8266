#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 300
#define ADDR_BOOT 200
#define ADDR_SSID 0
#define ADDR_PASSWD 100

void eeprom_write_string(int addrOffset, const String &strToWrite);
String eeprom_read_string(int addrOffset);
void eeprom_write_byte(int addrOffset, const char data);
char eeprom_read_byte(int addrOffset);
