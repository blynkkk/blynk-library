/**
 * @file       BlynkSimpleEsp32_SSL.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Dec 2017
 * @brief
 *
 */

#ifndef BlynkSimpleEsp32_SSL_h
#define BlynkSimpleEsp32_SSL_h

#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#if defined(BLYNK_SSL_USE_LETSENCRYPT)
  static const char BLYNK_DEFAULT_ROOT_CA[] =
  #include <certs/letsencrypt_pem.h>
#else
  static const char BLYNK_DEFAULT_ROOT_CA[] =
  #include <certs/blynkcloud_pem.h>
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFiClientSecure.h>

template <typename Client>
class BlynkArduinoClientSecure
    : public BlynkArduinoClientGen<Client>
{
public:
    BlynkArduinoClientSecure(Client& client)
        : BlynkArduinoClientGen<Client>(client)
        , caCert(NULL)
    {}

    void setRootCA(const char* fp) { caCert = fp; }

    bool connect() {
        this->client->setCACert(caCert);
        if (BlynkArduinoClientGen<Client>::connect()) {
          BLYNK_LOG1(BLYNK_F("Certificate OK"));
          return true;
        } else {
          BLYNK_LOG1(BLYNK_F("Secure connection failed"));
        }
        return false;
    }

private:
    const char* caCert;
};

template <typename Transport>
class BlynkWifi
    : public BlynkProtocol<Transport>
{
    typedef BlynkProtocol<Transport> Base;
public:
    BlynkWifi(Transport& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        WiFi.mode(WIFI_STA);
        if (pass && strlen(pass)) {
            WiFi.begin(ssid, pass);
        } else {
            WiFi.begin(ssid);
        }
        while (WiFi.status() != WL_CONNECTED) {
            BlynkDelay(500);
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
        this->conn.setRootCA(root_ca);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
        this->conn.setRootCA(root_ca);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, root_ca);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, root_ca);
        while(this->connect() != true) {}
    }

};

static WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
