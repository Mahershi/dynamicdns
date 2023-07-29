#ifndef RService_H
#define RService_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

class RService{
    static WiFiClientSecure client;
    static HTTPClient http;

    public:
        static void init();
        static void begin(
            String url,
            String *qp_keys = NULL,
            String *qp_values = NULL,
            int qp_size = 0
        );
        static String get();
        static bool put(String payload);
        static void add_header(
            String key,
            String value
        );

};


#endif