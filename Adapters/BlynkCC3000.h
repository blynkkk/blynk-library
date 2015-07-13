/**
 * @file       BlynkCC3000.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkCC3000_h
#define BlynkCC3000_h

#define BLYNK_INFO_CONNECTION "CC3000"

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adafruit_CC3000.h>

class BlynkTransportCC3000
{
public:
    BlynkTransportCC3000(Adafruit_CC3000& cc3000)
        : cc3000(cc3000), addr(0), port(0)
    {}

    void begin(uint32_t a, uint16_t p) {
        port = p;
        addr = a;
    }

    bool connect() {
        uint8_t* a = (uint8_t*)&addr;
        BLYNK_LOG("Connecting to %d.%d.%d.%d:%d", a[3], a[2], a[1], a[0], port);
        client = cc3000.connectTCP(addr, port);
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
    Adafruit_CC3000& cc3000;
    Adafruit_CC3000_Client client;
    uint32_t    addr;
    uint16_t    port;
};

class BlynkCC3000
    : public BlynkProtocol<BlynkTransportCC3000>
{
    typedef BlynkProtocol<BlynkTransportCC3000> Base;
public:
    BlynkCC3000(Adafruit_CC3000& cc3000, BlynkTransportCC3000& transp)
        : Base(transp), cc3000(cc3000)
    {}

    void connectWiFi(const char* ssid,
                     const char* pass,
                     uint8_t     secmode)
    {
        if (!cc3000.begin())
        {
            BLYNK_FATAL("Couldn't begin()! Check your wiring?");
        }

#if !defined(CC3000_TINY_DRIVER) && defined(BLYNK_DEBUG)
        uint8_t major, minor;
        if(!cc3000.getFirmwareVersion(&major, &minor))
        {
            if(major != 0x1 || minor < 0x13) {
                BLYNK_LOG("CC3000 upgrade needed?");
            }
        }
#endif

        /*if (!cc3000.deleteProfiles())
        {
            BLYNK_FATAL("Fail deleting old profiles");
        }*/
        BLYNK_LOG("Connecting to %s...", ssid);
        if (!cc3000.connectToAP(ssid, pass, secmode))
        {
            BLYNK_FATAL("Failed to connect to AP");
        }
        BLYNK_LOG("Getting IP address...");
        while (!cc3000.checkDHCP())
        {
            ::delay(100);
        }
#ifdef BLYNK_PRINT
        uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
        if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
        {
            BLYNK_FATAL("Unable to get the IP Address");
        }
        uint8_t* addr = (uint8_t*)&ipAddress;
        BLYNK_LOG("IP:  %d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);
        addr = (uint8_t*)&gateway;
        BLYNK_LOG("GW:  %d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);
        addr = (uint8_t*)&dnsserv;
        BLYNK_LOG("DNS: %d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);
#endif
    }

    void config(const char* auth,
            	const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        uint32_t ip = 0;
        BLYNK_LOG("Looking for %s", domain);
        while (ip == 0) {
            if (!cc3000.getHostByName((char*)domain, &ip)) {
                BLYNK_LOG("Couldn't locate server");
                ::delay(500);
            }
        }
        this->conn.begin(ip, port);
    }

    void config(const char* auth,
            	IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(cc3000.IP2U32(ip[0],ip[1],ip[2],ip[3]), port);
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
    Adafruit_CC3000& cc3000;
};

#endif
