#include "eeprom.h"

void eeprom_write_string(int addrOffset, const String &strToWrite)
{
    byte len = strToWrite.length();
    EEPROM.write(addrOffset, len);
    for (int i = 0; i < len; i++)
    {
        EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
    }
    EEPROM.commit();
}

String eeprom_read_string(int addrOffset)
{
    int newStrLen = EEPROM.read(addrOffset);
    char data[newStrLen + 1];
    for (int i = 0; i < newStrLen; i++)
    {
        data[i] = EEPROM.read(addrOffset + 1 + i);
    }
    data[newStrLen] = '\0';

    return String(data);
}

void eeprom_write_byte(int addrOffset, const char data)
{
    EEPROM.write(addrOffset, data);
}

char eeprom_read_byte(int addrOffset)
{
    EEPROM.read(addrOffset);
}
