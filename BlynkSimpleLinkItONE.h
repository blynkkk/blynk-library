/**
 * @file       BlynkSimpleLinkItONE.h
 * @author     WuLung Hsu
 * @license    This project is released under the MIT License (MIT)
 * @date       July 17 2015
 * @brief	   Bring LinkItONE to support
 *
 */

#ifndef BlynkSimpleLinkItONE_h
#define BlynkSimpleLinkItONE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "LinkItONE"
#endif


#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

class BlynkWifi
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkWifi(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void wifi_conn(const char* ssid, const char* pass, int wifi_auth)
    {
        // attempt to connect to Wifi network:
        LWiFi.begin();
        //while (!LWiFi.connectWPA(pSSID, pPassword)) {
        while(!LWiFi.connect(ssid, LWiFiLoginInfo((LWiFiEncryption)wifi_auth, pass))){
            delay(1000);
            BLYNK_LOG("Wifi Retry!");
        }
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               int wifi_auth,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        wifi_conn(ssid, pass,wifi_auth);
        this->conn.begin(domain, port);
    }

};


static LWiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
