/**
 * @file       BlynkSimpleLinkItONE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jul 2015
 * @brief
 *
 */

#ifndef BlynkSimpleLinkItONE_h
#define BlynkSimpleLinkItONE_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "LinkIt ONE"
#endif

// cause this causes crashes...
#define BLYNK_NO_YIELD

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

class BlynkLinkItOneWifi
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkLinkItOneWifi(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass, int wifi_auth)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        LWiFi.begin();
        while(!LWiFi.connect(ssid, LWiFiLoginInfo((LWiFiEncryption)wifi_auth, pass))){
            BlynkDelay(1000);
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
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               int         wifi_auth,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass, wifi_auth);
        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               int         wifi_auth,
               IPAddress   ip,
               uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass, wifi_auth);
        config(auth, ip, port);
        while(this->connect() != true) {}
    }

};


static LWiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkLinkItOneWifi Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
