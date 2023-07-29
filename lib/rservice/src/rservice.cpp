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
        Serial.print("URL Valid: ");
        Serial.println(isvalid);
        
    }catch(char *e){
        Serial.print("RestService.begin: Exception occured: ");
        Serial.println(e);
    }
}

void RService::add_header(String key, String value){
    try{
        http.addHeader(key, value);
    }catch(char *e){
        Serial.print("RestService.add_header: Exception occured");
        Serial.println(e);
    }
    
}

bool RService::put(String payload){
    Serial.println("PUT: ");
    Serial.println(payload);
    try{
        int statusCode = http.PUT(payload);
        
        if (statusCode == 200){
            Serial.println(statusCode);
            http.end();
            return true;
        }else{
            Serial.print("Couldnt Call HTTP PUT, Status Code: ");
            Serial.println(statusCode);
            Serial.println();
            return false;
        }
    }catch(char *e){
        Serial.print("RestService.put: Exception occured: ");
        Serial.println(e);
    }
    return false;
}

String RService::get(){
    try{
        
        int statusCode = http.GET();
        if(statusCode == 200){
            Serial.println(statusCode);
            String line = http.getString();
            http.end();
            Serial.println(line);
            return line;
        }else{
            Serial.print("Couldnt Call HTTP GET, Status Code: ");
            Serial.println(statusCode);
            return "NULL";
        }
    }catch(char *e){
        Serial.print("RestService.get: Exception occured: ");
        Serial.println(e);
    }
    return "NULL";
}

