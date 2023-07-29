#ifndef RService_H
#define RService_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


// RService Library, wrapping HTTPClient

// Natural order should be to
/*
    1. call init, if insecure https calls are required. secure https has not been added.
    2. call begin to setup url and query params.
    3. call add_headers to setup headers in the request. such as authentication and content-type.
    4. call get or put. other methods have not been implemented yet.

    Note: Do not call add_headers before begin. or after method call.
*/

class RService{
    // This is needed for HTTPS requests.
    static WiFiClientSecure client;
    // HTTPClient for request handling
    static HTTPClient http;

    public:
        // Initialized Client to allow insecure connections to HTTPS. Its okay to do so as long as you dont have any sensitive data.
        static void init();
        // Sets up URL and Query parameters for requests.
        static void begin(
            String url,
            String *qp_keys = NULL,
            String *qp_values = NULL,
            int qp_size = 0
        );
        // Trigger GET Request, returns payload as String
        static String get();
        // Trigeer GET Request, Accepts payload as String and returns bool based on success.
        static bool put(String payload);
        // Add headers to http request.
        static void add_header(
            String key,
            String value
        );

};


#endif