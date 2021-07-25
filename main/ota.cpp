#include <ArduinoOTA.h>

void ota_setup()
{
    // OTA「開始」的事件處理程式
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    
    // OTA「結束」的事件處理程式
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    
    // OTA「進行中」的事件處理程式
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    // OTA「錯誤」的事件處理程式
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    
    // 啟用OTA
    ArduinoOTA.begin();
}

void ota_loop()
{
    ArduinoOTA.handle();
}
