/**
 * @file       BlynkFishino.h
 * @author     Massimo Del Fedele
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Massimo Del Fedele
 * @date       Apr 2016
 * @brief
 *
 */

#ifndef BlynkFishino_h
#define BlynkFishino_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Fishino"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

class BlynkWifiCommon
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkWifiCommon(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
		if(!Fishino.reset()){
            BLYNK_FATAL("Fishino reset failed");
        }

	// go into station mode
	Fishino.setMode(STATION_MODE);

		// try forever to connect to AP
		// tenta la connessione finchè non riesce
		Serial << F("Connecting to AP...");
		while(!Fishino.begin(ssid, pass))
		{
			Serial << ".";
			delay(2000);
		}
		Serial << "OK\n";
		Fishino.staStartDHCP();

		// wait till connection is established
		Serial << F("Waiting for IP...");
		while(Fishino.status() != STATION_GOT_IP)
		{
			Serial << ".";
			delay(500);
		}
		Serial << "OK\n";

        IPAddress myip = Fishino.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
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

#endif
