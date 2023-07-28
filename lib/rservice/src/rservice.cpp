#include "rservice.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>


WiFiClientSecure RService::client;
HTTPClient RService::http;
WiFiClient RService::client2;

void RService::init(){
    client.setInsecure();
}

void RService::get(String url){
    try{     
        http.addHeader("Content-Type", "application/json");   
        bool isvalid = http.begin(client, url.c_str());
        Serial.println(isvalid);
        int statusCode = http.GET();
        Serial.println(statusCode);
        if(statusCode == 200){
            String line = http.getString();
            Serial.println(line);
        }else{
            Serial.print("Couldnt Call HTTP GET, Status Code: ");
            Serial.println(statusCode);
        }
    }catch(...){
        Serial.println("RestService.get: Exception occured");
    }
}

