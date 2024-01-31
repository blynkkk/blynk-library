/**
 * @file       BlynkSimpleEsp8266.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleEsp8266_h
#define BlynkSimpleEsp8266_h

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#include <version.h>

#if ESP_SDK_VERSION_NUMBER < 0x020200
#error Please update your ESP8266 Arduino Core
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <ESP8266WiFi.h>

class BlynkWifi
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkWifi(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        WiFi.mode(WIFI_STA);
        if (WiFi.status() != WL_CONNECTED) {
            if (pass && strlen(pass)) {
                WiFi.begin(ssid, pass);
            } else {
                WiFi.begin(ssid);
            }
        }
        while (WiFi.status() != WL_CONNECTED) {
            BlynkDelay(500);
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        (void)myip; // Eliminate warnings about unused myip
        BLYNK_LOG_IP("IP: ", myip);
    }
    
    bool connectWiFi_t(const char* ssid, 
                       const char* pass, 
                       uint16_t timeout = 10000)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        WiFi.mode(WIFI_STA);
        if (WiFi.status() != WL_CONNECTED) {
          if (pass && strlen(pass)) {
            WiFi.begin(ssid, pass);
          } else {
            WiFi.begin(ssid);
          }
        }
		int WFB = timeout / 500;  // set default 20 cycles 
        while ((WiFi.status() != WL_CONNECTED) && (WFB>0)) {
          BlynkDelay(500);
		  --WFB;
        }
        if (WiFi.status() != WL_CONNECTED) return false;		
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));
        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
		return true;
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port);
        while(this->connect() != true) {}
    }
    
    bool begin_t(const char* auth,
                 const char* ssid,
                 const char* pass,
                 IPAddress   ip,
                 uint16_t    port   = BLYNK_DEFAULT_PORT,
			     uint16_t wifi_timeout = 30000)
    {
		if (WiFi.status() != WL_CONNECTED) {
		  uint16_t connect_count = wifi_timeout / 10000;
          while ((connectWiFi_t(ssid, pass, wifi_timeout / 3)) && (connect_count>0)) --connect_count;   
		  if (WiFi.status() != WL_CONNECTED) return false;
		}
        config(auth, ip, port);
		return this->connect();
    } 

};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static WiFiClient _blynkWifiClient;
  static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
  BlynkWifi Blynk(_blynkTransport);
#else
  extern BlynkWifi Blynk;
#endif

#include <BlynkWidgets.h>

#endif
