#ifndef RService_H
#define RService_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

class RService{
    static WiFiClientSecure client;
    static HTTPClient http;
    static WiFiClient client2;

    public:
        static void init();
        static void get(String url);
};


#endif