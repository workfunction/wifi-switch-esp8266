#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "server.h"
#include "eeprom.h"
#include "entities.h"

ESP8266WebServer server(80);

int statusCode;
String content;

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void handle_index()
{
    Serial.println("Get index");
    content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<p><form method='get' action='ledon'><label>LED ON </label><input type='submit'></form></p>";
    content += "<p><form method='get' action='ledoff'><label>LED OFF </label><input type='submit'></form></p>";
    
    content += "<p><form method='post' action='ssid'><label>SSID\t</label> <input type='text' name='ssid'> <br> <label>password\t</label> <input type='password' name='password'> <br> <input type='submit' value='update'></form></p>";

    content += "</html>";
    server.send(200, "text/html", content);
}

void handle_ssid()
{
    char ssid[100];
    char password[100];

    decode_html_entities_utf8(ssid, server.arg("ssid").c_str());
    decode_html_entities_utf8(password, server.arg("password").c_str());

    //strncpy(ssid, server.arg("ssid").c_str(), 100);
    //strncpy(password, server.arg("password").c_str(), 100);

    Serial.print("ssid: ");
    Serial.println(ssid);
    Serial.print("password: ");
    Serial.println(password);

    writeStringToEEPROM(0, ssid);
    writeStringToEEPROM(100, password);

    statusCode = 200;
    server.send(statusCode, "text/html", content);

    ESP.restart();
}

void server_init()
{
    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handle_index);
 
    server.on("/ledon", []() {
        Serial.println("Get LED_ON");
        digitalWrite(GPIO_PIN, LOW);
        statusCode = 200;
        server.send(statusCode, "text/html", content);
    });

    server.on("/ledoff", []() {
        Serial.println("Get LED_OFF");
        digitalWrite(GPIO_PIN, HIGH);
        statusCode = 200;
        server.send(statusCode, "text/html", content);
    });

    server.on("/ssid", handle_ssid);

    server.onNotFound(handleNotFound);

    server.begin();

    Serial.println("HTTP server started");
}

void server_loop()
{
    server.handleClient();
    MDNS.update();
}
