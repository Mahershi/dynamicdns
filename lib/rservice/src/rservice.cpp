#include "rservice.h" 
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>


WiFiClientSecure RService::client;
HTTPClient RService::http;

void RService::init(){
    client.setInsecure();
}

void RService::begin(
    String url,
    String *qp_keys,
    String *qp_values,
    int qp_size
){
    try{ 
        for(int i=0; i<qp_size; i++){
            if (i==0){
                url = url + "?" + qp_keys[i] + "=" + qp_values[i];
            }else{
                url = url + "&" + qp_keys[i] + "=" + qp_values[i];
            }
        }
        Serial.println(url);
        bool isvalid = http.begin(client, url.c_str());
        Serial.print("URL is Valid: ");
        Serial.println(isvalid);
        
    }catch(...){
        Serial.println("RestService.begin: Exception occured");
    }
}

void RService::add_header(String key, String value){
    try{
        http.addHeader(key, value);
    }catch(...){
        Serial.println("RestService.add_header: Exception occured");
    }
    
}

String RService::get(){
    try{
        int statusCode = http.GET();
        Serial.println(statusCode);
        if(statusCode == 200){
            String line = http.getString();
            Serial.println(line);
            return line;
        }else{
            Serial.print("Couldnt Call HTTP GET, Status Code: ");
            Serial.println(statusCode);
            return "NULL";
        }
    }catch(...){
        Serial.println("RestService.get: Exception occured");
    }
    return "NULL";
}

