//  HomeLab monitor
//  NodeMCU and I2C Oled display

// Eduardo Queiroz

#include <Wire.h>
#include "SSD1306Wire.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>



//Starts the OLED display
SSD1306Wire  display(0x3c, D1, D2);



#define WIFI_SSID "YOUR WIFI SSID"
#define WIFI_PASS "YOUR WIFI PASSWORD"
                                                                                                            // Change the "10.10.1.10" value with whatever you server IP is
#define NETDATA_URL_UPTIME "http://10.10.1.10:19999/api/v1/data?chart=system.uptime&after=-1"               // Server uptime
#define NETDATA_URL_CPU "http://10.10.1.10:19999/api/v1/data?chart=system.cpu&after=-1"                     // CPU usage
#define NETDATA_URL_RAM "http://10.10.1.10:19999/api/v1/data?chart=system.ram&after=-1"                     // RAM usage
//#define NETDATA_URL_NET "http://10.10.1.10:19999/api/v1/data?chart=system.net&after=-1"                   // Network inbound and outbound. This does not work if you're using NetData installed via plugin
#define NETDATA_URL_CPUTEMP "http://10.10.1.10:19999/api/v1/data?chart=cpu.temperature&after=-1"            // CPU temperature



int incomingByte = 0;

WiFiClient client;
HTTPClient http;

void setup()
{
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();

  connectIfNeeded();
}

void connectIfNeeded() {

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      display.clear();
      display.drawString(63, 26, "Connecting");                       //Displays "Connecting" on the screen while trying to connect to the WiFi network 
      display.display();
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    display.clear();
    display.drawString(63, 26, "Connected");
    display.display();
    delay(500);
  }
}


JsonObject& getJson(const char * url, int bufferSize) {
  
                                                                       
  http.begin(client, url);
  http.GET();                                                          //Getting data over REST. You can repeat for the endpoints you want to fetch
  String payload = http.getString();
  DynamicJsonBuffer jsonBuffer(bufferSize);
  return jsonBuffer.parseObject(payload);
}



void getUpTime(const char * url, char * buf) {
  
  JsonObject& root = getJson(url, 200);
  if (!root.success()) {
    return;
  }

  long uptime_secs_t = root["data"][0][1];
  int uptime_hours = uptime_secs_t / 60 / 60;                           //The uptime value is provided in seconds. You can calculate it how you want. I'm using hours
  String convertion = String(uptime_hours);

  display.clear();
  display.drawString(63, 9, "Uptime");
  display.drawString(55, 40, convertion);
  display.drawString(75, 40, "h");
  display.display();
  delay(3000);
}

void getCPUUsage(const char * url, char * buf) {
  JsonObject& root = getJson(url, 455);
  if (!root.success()) {
    return;
  }

  char float_tmp[7];
  JsonArray& data_0 = root["data"][0];
  dtostrf((float)data_0[1] + (float)data_0[2]  + (float)data_0[3] + (float)data_0[4] + (float)data_0[5] + (float)data_0[6] + (float)data_0[7] + (float)data_0[8] + (float)data_0[9] + (float)data_0[10] + (float)data_0[11], 3, 2, float_tmp);

  display.clear();                             //Calculating the average utilization of all the CPU cores. You can add or remove floats according to the number of cores your CPU has. In this example is 12
  display.drawString(63, 9, "CPU");
  display.drawString(60, 40, float_tmp);
  display.drawString(90, 40, "%");
  display.display();
  delay(3000);
}


void getCPUTemp(const char * url, char * buf) {
  
  JsonObject& root = getJson(url, 455);
  if (!root.success()) {
    return;
  }

  char float_tmp[11];
  JsonArray& data_0 = root["data"][0];
  dtostrf(((float)data_0[1] + (float)data_0[2]  + (float)data_0[3] + (float)data_0[4] + (float)data_0[5] + (float)data_0[6] + (float)data_0[7] + (float)data_0[8] + (float)data_0[9] + (float)data_0[10] + (float)data_0[11]) / 12, 2, 1, float_tmp);
  
                                                //Calculating the average temperature of all the CPU cores. You can add or remove floats according to the number of cores your CPU has. In this example is 12
  display.clear();
  display.drawString(63, 9, "CPU");
  display.drawString(58, 40, float_tmp);
  display.drawString(88, 40, "°C");
  display.display();
  delay(3000);
}


void getRAMUsage(const char * url, char * buf) {
  
  JsonObject& root = getJson(url, 455);
  if (!root.success()) {
    return;
  }

  JsonArray& data_0 = root["data"][0];

   char float_tmp[9];
   float ram = data_0[1];
   float freeRam = ram *  0.00097;              
   dtostrf(freeRam, 3, 1, float_tmp);
   display.clear();
   display.drawString(63, 9, "RAM");
   display.drawString(30, 40, float_tmp);
   display.drawString(58, 40, "GB");
   display.drawString(92, 40, "Free");
   display.display();
   delay(3000);
}



char * tmp;

void loop()
{
 char * buf = (char*)malloc(sizeof(char) * 256);

  tmp = (char*)malloc(sizeof(char) * 32);  
  getUpTime(NETDATA_URL_UPTIME, tmp);
  strcat(buf, tmp);
  free(tmp);

  tmp = (char*)malloc(sizeof(char) * 32);  
  getCPUUsage(NETDATA_URL_CPU, tmp);
  strcat(buf, tmp);
  free(tmp);

  tmp = (char*)malloc(sizeof(char) * 32);  
  getCPUTemp(NETDATA_URL_CPUTEMP, tmp);
  strcat(buf, tmp);
  free(tmp);


  tmp = (char*)malloc(sizeof(char) * 128);
  getRAMUsage(NETDATA_URL_RAM, tmp);
  strcat(buf, tmp);
  free(tmp);
  
}
