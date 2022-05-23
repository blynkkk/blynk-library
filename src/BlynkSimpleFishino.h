/**
 * @file       BlynkSimpleFishino.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jun 2016
 * @brief
 *
 */

#ifndef BlynkSimpleFishino_h
#define BlynkSimpleFishino_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Fishino"
#endif

#ifdef ESP8266
#error This code is not intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <Fishino.h>

class BlynkFishino
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkFishino(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        SPI.begin();
        SPI.setClockDivider(SPI_CLOCK_DIV2);

        while(!Fishino.reset()) {
            BLYNK_LOG1(BLYNK_F("Fishino RESET FAILED"));
        }

        Fishino.setMode(STATION_MODE);
        if (pass && strlen(pass)) {
            while (!Fishino.begin(ssid, pass)) {
                BlynkDelay(500);
            }
        } else {
            while (!Fishino.begin(ssid)) {
                BlynkDelay(500);
            }
        }

        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        Fishino.staStartDHCP();
        while(Fishino.status() != STATION_GOT_IP)
        {
            BlynkDelay(500);
        }

        IPAddress myip = Fishino.localIP();
        BLYNK_LOG_IP("IP: ", myip);
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

};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static FishinoClient _blynkFishinoClient;
  static BlynkArduinoClient _blynkTransport(_blynkFishinoClient);
  BlynkFishino Blynk(_blynkTransport);
#else
  extern BlynkFishino Blynk;
#endif

#include <BlynkWidgets.h>

#endif
