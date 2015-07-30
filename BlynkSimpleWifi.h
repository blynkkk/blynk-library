/**
 * @file       BlynkSimpleWifi.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleWifi_h
#define BlynkSimpleWifi_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "HDG204"
#endif

// Fix for WiFi shield (it has a crazy ping duration)
#define BLYNK_TIMEOUT_MS 6000UL

#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFi.h>

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
        int status = WL_IDLE_STATUS;
         // check for the presence of the shield:
         if (WiFi.status() == WL_NO_SHIELD) {
             BLYNK_FATAL("WiFi shield not present");
         }

         String fv = WiFi.firmwareVersion();
         if (fv != "1.1.0") {
             BLYNK_LOG("Please upgrade the firmware");
         }

         // attempt to connect to Wifi network:
         while (true) {
             BLYNK_LOG("Connecting to %s...", ssid);
             if (pass && strlen(pass)) {
            	 status = WiFi.begin((char*)ssid, (char*)pass);
             } else {
            	 status = WiFi.begin((char*)ssid);
             }
             if (status == WL_CONNECTED) {
                 break;
             } else {
                 ::delay(5000);
             }
         }
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
               uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass);
    	config(auth, domain, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass);
    	config(auth, ip, port);
    }

};

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
