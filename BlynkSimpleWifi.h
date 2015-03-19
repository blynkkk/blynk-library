/**
 * @file       BlynkSimpleWifi.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleWifi_h
#define BlynkSimpleWifi_h

#include <Blynk/BlynkProtocol.h>
#include <BlynkArduinoClient.h>
#include <WiFi.h>

class BlynkWifi
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkWifi(BlynkArduinoClient& conn)
        : BlynkProtocol<BlynkArduinoClient>(conn)
    {}

    void wifi_conn(const char* ssid, const char* pass)
    {
        int status = WL_IDLE_STATUS;
         // check for the presence of the shield:
         if (WiFi.status() == WL_NO_SHIELD) {
             BLYNK_FATAL("WiFi shield not present");
         }

         // attempt to connect to Wifi network:
         while (status != WL_CONNECTED) {
             BLYNK_LOG("Attempting to connect to SSID: %s", ssid);
             status = WiFi.begin((char*)ssid, pass);
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
