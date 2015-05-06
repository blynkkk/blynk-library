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

    void wifi_conn(const char* ssid, const char* pass)
    {
        BLYNK_LOG("Connecting to %s", ssid);
        WiFi.begin(ssid, pass);
        while (WiFi.status() != WL_CONNECTED) {
            ::delay(500);
        }
        BLYNK_LOG("Connected to WiFi");
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
