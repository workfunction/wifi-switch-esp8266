#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "wifi.h"
#include "eeprom.h"

#define STASSID "美味美"
#define STAPSK  "0911767387"

#define APSSID "orthtech_"
#define APPSK  "orthtech"

#define RETRY_TIMES 3

char ssid[100];
char password[100];
char status_str[2];

int default_connection()
{
    WiFi.mode(WIFI_STA);

    //writeStringToEEPROM(0, STASSID);
    //writeStringToEEPROM(100, STAPSK);

    strncpy(ssid, readStringFromEEPROM(0).c_str(), 100);
    strncpy(password, readStringFromEEPROM(100).c_str(), 100);

    Serial.println("");
    Serial.println("Starting to connect wifi...");

    Serial.print("ssid: \"");
    Serial.print(ssid);
    Serial.println("\"");

    Serial.print("password: \"");
    Serial.print(password);
    Serial.println("\"");

    WiFi.begin(ssid, password);

    Serial.print("Connecting");
    // Wait for connection
    while (WiFi.status() == WL_DISCONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\n");

    return WiFi.status();
}

void default_ap_setup()
{
    WiFi.mode(WIFI_AP);

    char mac[18] = {0};
    char ssid_postfix[5] = {0};

    strncpy(mac, WiFi.macAddress().c_str(), 18);

    ssid_postfix[0] = mac[12];
    ssid_postfix[1] = mac[13];
    ssid_postfix[2] = mac[15];
    ssid_postfix[3] = mac[16];

    strncpy(ssid, APSSID, 100);
    strncpy(password, APPSK, 100);
    strcat(ssid, ssid_postfix);

    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Please connect to:");
    Serial.println(ssid);

    Serial.print("Password:");
    Serial.println(password);

    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

int wifi_init()
{
    int ret = 0;
    for (int c = 0; c < RETRY_TIMES; c++) {
        ret = default_connection();
        if (ret == WL_CONNECTED) {
            break;
        }
        Serial.println("Connection failed, try again.");
    }

    if (ret != WL_CONNECTED) {
        Serial.println("Wifi connetcion failed, start setup AP");
        default_ap_setup();
        return 1;
    }

    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return 0;
}
