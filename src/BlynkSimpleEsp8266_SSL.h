/**
 * @file       BlynkSimpleEsp8266_SSL.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2016
 * @brief
 *
 */

#ifndef BlynkSimpleEsp8266_SSL_h
#define BlynkSimpleEsp8266_SSL_h

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#include <version.h>

#if ESP_SDK_VERSION_NUMBER < 0x020200
#error Please update your ESP8266 Arduino Core
#endif

// Fingerprint is not used by default
//#define BLYNK_DEFAULT_FINGERPRINT "FD C0 7D 8D 47 97 F7 E3 07 05 D3 4E E3 BB 8E 3D C0 EA BE 1C"

#if defined(BLYNK_SSL_USE_LETSENCRYPT)
  static const unsigned char BLYNK_DEFAULT_CERT_DER[] PROGMEM =
  #include <certs/dst_der.h>  // TODO: using DST Root CA X3 for now
  //#include <certs/isrgroot_der.h>
  //#include <certs/letsencrypt_der.h>
#else
  static const unsigned char BLYNK_DEFAULT_CERT_DER[] PROGMEM =
  #include <certs/blynkcloud_der.h>
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

template <typename Client>
class BlynkArduinoClientSecure
    : public BlynkArduinoClientGen<Client>
{
public:
    BlynkArduinoClientSecure(Client& client)
        : BlynkArduinoClientGen<Client>(client)
        , fingerprint(NULL)
    {}

    void setFingerprint(const char* fp) { fingerprint = fp; }

    bool setCACert(const uint8_t* caCert, unsigned caCertLen) {
        bool res = this->client->setCACert(caCert, caCertLen);
        if (!res) {
          BLYNK_LOG1("Failed to load root CA certificate!");
        }
        return res;
    }

    bool setCACert_P(const uint8_t* caCert, unsigned caCertLen) {
        bool res = this->client->setCACert_P(caCert, caCertLen);
        if (!res) {
          BLYNK_LOG1("Failed to load root CA certificate!");
        }
        return res;
    }

    bool connect() {
        // Synchronize time useing SNTP. This is necessary to verify that
        // the TLS certificates offered by the server are currently valid.
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
        time_t now = time(nullptr);
        while (now < 100000) {
          delay(500);
          now = time(nullptr);
        }
        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);
        String ntpTime = asctime(&timeinfo);
        ntpTime.trim();
        BLYNK_LOG2("NTP time: ", ntpTime);

        // Now try connecting
        if (BlynkArduinoClientGen<Client>::connect()) {
          if (fingerprint && this->client->verify(fingerprint, this->domain)) {
              BLYNK_LOG1(BLYNK_F("Fingerprint OK"));
              return true;
          } else if (this->client->verifyCertChain(this->domain)) {
              BLYNK_LOG1(BLYNK_F("Certificate OK"));
              return true;
          }
          BLYNK_LOG1(BLYNK_F("Certificate not validated"));
          return false;
        }
        return false;
    }

private:
    const char* fingerprint;
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
        BLYNK_LOG_IP("IP: ", myip);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
                const char* fingerprint = NULL)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);

        if (fingerprint) {
          this->conn.setFingerprint(fingerprint);
        } else {
          this->conn.setCACert_P(BLYNK_DEFAULT_CERT_DER, sizeof(BLYNK_DEFAULT_CERT_DER));
        }
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
                const char* fingerprint = NULL)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);

        if (fingerprint) {
          this->conn.setFingerprint(fingerprint);
        } else {
          this->conn.setCACert_P(BLYNK_DEFAULT_CERT_DER, sizeof(BLYNK_DEFAULT_CERT_DER));
        }
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* fingerprint = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, fingerprint);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* fingerprint = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, fingerprint);
        while(this->connect() != true) {}
    }

};

static WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
