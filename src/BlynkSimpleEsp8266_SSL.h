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

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "ESP8266"
#endif

#define BLYNK_DEFAULT_FINGERPRINT "A2 10 3C 79 7D 19 FB B2 50 0D 02 C6 39 8B 94 D0 02 B8 88 88"

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

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

    bool connect() {
        if (BlynkArduinoClientGen<Client>::connect()) {
		  if (fingerprint && !this->client.verify(fingerprint, this->domain)) {
			  BLYNK_LOG("Certificate doesn't match");
			  return false;
		  } else {
			  BLYNK_LOG("Certificate OK");
		  }
		  return true;
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
        BLYNK_LOG("Connecting to %s", ssid);
        WiFi.mode(WIFI_STA);
        if (pass && strlen(pass)) {
        	WiFi.begin(ssid, pass);
        } else {
        	WiFi.begin(ssid);
        }
        while (WiFi.status() != WL_CONNECTED) {
            ::delay(500);
        }
        BLYNK_LOG("Connected to WiFi");

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
				const char* fingerprint = BLYNK_DEFAULT_FINGERPRINT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
        this->conn.setFingerprint(fingerprint);
    }

    void config(const char* auth,
            	IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
				const char* fingerprint = BLYNK_DEFAULT_FINGERPRINT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
        this->conn.setFingerprint(fingerprint);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
			   const char* fingerprint = BLYNK_DEFAULT_FINGERPRINT)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, fingerprint);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
			   const char* fingerprint = BLYNK_DEFAULT_FINGERPRINT)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, fingerprint);
    }

};

static WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
