///
/// @file		BlynkCC3200.h
/// @brief		Header
/// @details	LaunchPad CC3200 Adapter for Blink
/// @n
/// @n @b		Project Blink_CC3200
/// @n          Extension for Blink by Volodymyr Shymanskyy
/// @see        http://www.blynk.cc
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
///
/// @date		July 22, 2015 19:00
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2015
/// @copyright	CC = BY CA NC
///
/// @see		ReadMe.txt for references
///

/**
 * @file       BlynkCC3200.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkCC3200_h
#define BlynkCC3200_h

#define BLYNK_INFO_CONNECTION "CC3200"

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>

#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

class BlynkTransportCC3200
{
public:
    BlynkTransportCC3200(WiFiClass& cc3200)
    : cc3000(cc3200), addr(0), port(0)
    {}
    
    void begin(uint32_t a, uint16_t p) {
        port = p;
        addr = a;
    }
    
    bool connect() {
        uint8_t* a = (uint8_t*)&addr;
        BLYNK_LOG("Connecting to %d.%d.%d.%d:%d", a[3], a[2], a[1], a[0], port);
        client = client.connect(addr, port);
        return client.connected();
    }
    
    void disconnect() { client.stop(); }
    
    size_t read(void* buf, size_t len) {
        return client.readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return client.write((const uint8_t*)buf, len);
    }
    
    bool connected() { return client.connected(); }
    int available() { return client.available(); }
    
private:
    WiFiClass& cc3000;
    WiFiClient client;
    uint32_t    addr;
    uint16_t    port;
};

class BlynkCC3200
: public BlynkProtocol<BlynkTransportCC3200>
{
    typedef BlynkProtocol<BlynkTransportCC3200> Base;
public:
    BlynkCC3200(WiFiClass& cc3200, BlynkTransportCC3200& transp)
    : Base(transp), cc3200(cc3200)
    {}
    
    void connectWiFi(const char* ssid,
                     const char* pass,
                     uint8_t     secmode)
    {
        BLYNK_LOG("Connecting to %s...", ssid);
        cc3200.begin((char*)ssid, (char*)pass);
        while (cc3200.status() != WL_CONNECTED)
        {
            ::delay(100);
        }
        BLYNK_LOG("Getting IP address...");
        while (WiFi.localIP() == INADDR_NONE)
        {
            ::delay(100);
        }
#ifdef BLYNK_PRINT
        IPAddress addr = WiFi.localIP();
        BLYNK_LOG("IP:  %d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);
#endif
    }
    
    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        IPAddress ip(0,0,0,0);
        BLYNK_LOG("Looking for %s", domain);
        while (cc3200.hostByName((char*)domain, ip) < 0) {
            ::delay(500);
        }
        this->conn.begin(ip, port);
    }
    
    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }
    
    void begin( const char* auth,
               const char* ssid,
               const char* pass,
               uint8_t     secmode,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass, secmode);
        config(auth, domain, port);
    }
    
    void begin( const char* auth,
               const char* ssid,
               const char* pass,
               uint8_t     secmode,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        connectWiFi(ssid, pass, secmode);
        config(auth, ip, port);
    }
private:
    WiFiClass& cc3200;
};

#endif
