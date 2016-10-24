/**
 * @file       BlynkWildFire.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkWildFire_h
#define BlynkWildFire_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "CC3000"
#endif

#define BLYNK_SEND_ATOMIC

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <IPAddress.h>
#include <WildFire_CC3000.h>
#include <utility/netapp.h>

class BlynkTransportWildFire
{
public:
    BlynkTransportWildFire(WildFire_CC3000& cc3000)
        : cc3000(cc3000)
        , host(NULL)
        , addr(0)
        , port(0)
    {}

    void begin(uint32_t a, uint16_t p) {
        host = NULL;
        port = p;
        addr = a;
    }

    void begin(const char* h, uint16_t p) {
        host = h;
        port = p;
    }

    bool connect() {
        if (host) {
            BLYNK_LOG2(BLYNK_F("Looking for "), host);
            while (addr == 0) {
                if (!cc3000.getHostByName((char*)host, &addr)) {
                    BLYNK_LOG1(BLYNK_F("Couldn't locate server"));
                    ::delay(500);
                }
            }
        }
        uint8_t* a = (uint8_t*)&addr;
        BLYNK_LOG_IP_REV("Connecting to ", a);
        client = cc3000.connectTCP(addr, port);
        return client.connected();
    }

    void disconnect() { client.close(); }

    size_t read(void* buf, size_t len) {
        char* beg = (char*)buf;
        char* end = beg + len;
        while (beg < end) {
            int c = client.read();
            if (c < 0)
                break;
            *beg++ = (char)c;
        }
        return len;
    }
    size_t write(const void* buf, size_t len) {
        return client.write((const uint8_t*)buf, len);
    }

    bool connected() { return client.connected(); }
    int available() { return client.available(); }

private:
    WildFire_CC3000& cc3000;
    WildFire_CC3000_Client client;
    const char* host;
    uint32_t    addr;
    uint16_t    port;
};

class BlynkWildFire
    : public BlynkProtocol<BlynkTransportWildFire>
{
    typedef BlynkProtocol<BlynkTransportWildFire> Base;
public:
    BlynkWildFire(WildFire_CC3000& cc3000, BlynkTransportWildFire& transp)
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
                BLYNK_LOG1(BLYNK_F("CC3000 upgrade needed?"));
            }
        }
#endif

        // Remove socket inactivity timeout
        /*unsigned long aucDHCP       = 14400;
        unsigned long aucARP        = 3600;
        unsigned long aucKeepalive  = 30;
        unsigned long aucInactivity = 0;
        int iRet = netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity);
        if (iRet != 0) {
            BLYNK_FATAL("Cannot set netapp timeout!");
        }*/

        /*if (!cc3000.deleteProfiles())
        {
            BLYNK_FATAL("Fail deleting old profiles");
        }*/
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        if (!cc3000.connectToAP(ssid, pass, secmode))
        {
            BLYNK_FATAL("Failed to connect to AP");
        }
        BLYNK_LOG1(BLYNK_F("Getting IP..."));
        while (!cc3000.checkDHCP())
        {
            ::delay(100);
        }

        uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
        if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
        {
            BLYNK_FATAL("DHCP failed.");
        }
#ifdef BLYNK_PRINT
        uint8_t* addr = (uint8_t*)&ipAddress;
        BLYNK_LOG_IP_REV("IP: ", addr);
        addr = (uint8_t*)&gateway;
        BLYNK_LOG_IP_REV("GW: ", addr);
        addr = (uint8_t*)&dnsserv;
        BLYNK_LOG_IP_REV("DNS: ", addr);
#endif
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
    WildFire_CC3000& cc3000;
};

#endif
