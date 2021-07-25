#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 200

void writeStringToEEPROM(int addrOffset, const String &strToWrite);
String readStringFromEEPROM(int addrOffset);
