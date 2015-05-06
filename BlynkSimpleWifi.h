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

    void wifi_conn(const char* ssid, const char* pass)
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
             status = WiFi.begin((char*)ssid, pass);
             if (status == WL_CONNECTED) {
            	 break;
             } else {
            	 ::delay(5000);
             }
         }
         BLYNK_LOG("Connected to wifi");
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        wifi_conn(ssid, pass);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress ip,
               uint16_t port)
    {
        Base::begin(auth);
        wifi_conn(ssid, pass);
        this->conn.begin(ip, port);
    }

};

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
