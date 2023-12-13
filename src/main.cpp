#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "rservice.h"
#include <ArduinoJson.h>

typedef struct {
  String ip;
  String name;
  int ttl;
  String type;
} Record;

Record dnsRecord;
String public_ip = "";
String daddy_ip = "";
String daddy_auth = "XX";

const char* ssid = "SASKTEL02B8";
const char* password = "123123123";
bool failed = false;

void wifi_init();
String get_public_ip();
void get_dns_record();
bool synchronize_ip(String public_ip);
StaticJsonDocument<200> json_parse(String val);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  // put your setup code here, to run once:
  delay(1000);
  WiFi.hostname("DynamicDNS_Node");
  Serial.println("Connecting ...");
  wifi_init();

  // slow blinking indicates trying to connect.
  while (!WiFi.isConnected() && !failed){
      Serial.print('.');
      digitalWrite(LED_BUILTIN, LOW);
      delay(800);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(800);
  }

  // 3 fast blinks indicates connected
  int i=3;
  while (i>0){
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
      digitalWrite(LED_BUILTIN, HIGH);
      i--;
  }
  delay(2000);
  Serial.print("Connected: ");
  Serial.print(ssid);
  Serial.print(" as ");
  Serial.println("DynamicDNS_Node");
  
  RService::init();
  get_dns_record();
  // 2 fast blinks indicates fetch of dns, can be success, can be false.
  i=2;
  while (i>0){
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      i--;
  }
  Serial.println("Setting high");
}

int interval = 300000;

void loop() {
  if (WiFi.isConnected()){
    public_ip = get_public_ip();

    // LED Stays on whiles its synchronizing.
    if (dnsRecord.ip != public_ip){
      digitalWrite(LED_BUILTIN, LOW);
      bool success = synchronize_ip(public_ip);
      if (success) {
        dnsRecord.ip = public_ip;
        Serial.println("IP Sync Successful!");  
      }else{
        Serial.println("IP Sync Failed!");
      }
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }else{
    Serial.println("Error: WiFi not Connected");
    delay(50000);
    interval = interval - 50000;
  }
  
  delay(10000);
  interval = interval - 10000;
  if (interval <= 0){
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    interval = 300000;
  }
}


bool synchronize_ip(String public_ip){
  if (public_ip != ""){
    Serial.println("Synchornizing IP");
      String url = "https://api.godaddy.com/v1/domains/mahershi.com/records/A/app/";
      String payload;
      StaticJsonDocument<200> doc;

      doc[0]["data"] = public_ip;
      doc[0]["name"] = dnsRecord.name;
      doc[0]["ttl"] = dnsRecord.ttl;
      doc[0]["type"] = dnsRecord.type;

      
      serializeJson(doc, payload);
      RService::begin(url);
      RService::add_header("Content-Type", "application/json");
      RService::add_header("Authorization", daddy_auth);
      bool success = RService::put(payload);


      return success;
  }
  

  return true;
}

void get_dns_record(){
  String url = "https://api.godaddy.com/v1/domains/mahershi.com/records/A/app/";
  RService::begin(url);
  RService::add_header("Authorization", daddy_auth);
  String data = RService::get();

  if (data != "NULL"){
    StaticJsonDocument<200> doc = json_parse(data);
    String ip = doc[0]["data"];
    String name = doc[0]["name"];
    String type = doc[0]["type"];
    int ttl = doc[0]["ttl"];
    dnsRecord.ip = ip;
    dnsRecord.name = name;
    dnsRecord.type = type;
    dnsRecord.ttl = ttl;
    Serial.println("\nDNS Record");
    Serial.println(dnsRecord.ip);
    Serial.println(dnsRecord.name);
    Serial.println(dnsRecord.type);
    Serial.println(dnsRecord.ttl);
    Serial.println();
  }else{
    Serial.println("Error: get_dns_record: Received NULL");
  }
  

}

String get_public_ip(){
  RService::begin(
    "https://api.seeip.org/jsonip?"
  );
  String data = RService::get();
  if (data != "NULL"){
    StaticJsonDocument<200> doc = json_parse(data);
    String ip = doc["ip"];
    return ip;
  }else{
    Serial.println("Error: get_public_ip: Received NULL");
  }
  return "NULL";
  
}

StaticJsonDocument<200> json_parse(String val){
  StaticJsonDocument<200> doc;
  try{
    DeserializationError err = deserializeJson(doc, val);
    Serial.print("Deserialization: ");
    Serial.println(err.c_str());
    Serial.println();
    return doc;
  }catch(...){
    Serial.println("DS Error");
  }
  return doc;
}

void wifi_init(){
  failed = false;
  try{
    WiFi.begin(ssid, password);
  }
  catch(char* e){
    failed = true;
    Serial.println("WiFi Exception: ");
    Serial.println(e);
  }
}
