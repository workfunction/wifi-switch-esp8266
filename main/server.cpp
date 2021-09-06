#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "server.h"
#include "eeprom.h"
#include "entities.h"

ESP8266WebServer server(80);

int statusCode;
String content;

String getContentType(String filename){
    if(server.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    return "text/plain";
}
 
bool handleFileRead(String path){
    Serial.println("handleFileRead: " + path); // 在序列埠顯示路徑
    
    if (path.endsWith("/")) {
        path += "index.html";
    }
    
    String contentType = getContentType(path);
    
    if (SPIFFS.exists(path)){
        File file = SPIFFS.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
    
        return true;
    }
    return false;
}

void handle_files()
{
    if (!handleFileRead(server.uri())) {
        server.send(404, "text/plain", "FileNotFound");
    }
}

void handle_index()
{
    Serial.println("Get index");
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
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

    eeprom_write_string(ADDR_SSID, ssid);
    eeprom_write_string(ADDR_PASSWD, password);

    handle_index();

    ESP.restart();
}

void server_init()
{
    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
 
    server.on("/ledon", []() {
        Serial.println("Get LED_ON");
        digitalWrite(GPIO_PIN, LOW);
        
        handle_index();
    });

    server.on("/ledoff", []() {
        Serial.println("Get LED_OFF");
        digitalWrite(GPIO_PIN, HIGH);
        
        handle_index();
    });

    server.on("/ssid", handle_ssid);

    server.onNotFound(handle_files);

    server.begin();

    Serial.println("HTTP server started");
}

void server_loop()
{
    server.handleClient();
    MDNS.update();
}
