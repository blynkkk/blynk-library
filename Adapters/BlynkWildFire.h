/**
 * @file       BlynkWildFire.h
 * @author     Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkWildFire_h
#define BlynkWildFire_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "WildFire"
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "CC3000"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <IPAddress.h>
#include <WildFire_CC3000.h>

class BlynkTransportWildFire
{
public:
    BlynkTransportWildFire(WildFire_CC3000& cc3000)
        : cc3000(cc3000), port(0)
    {}

    void begin(IPAddress a, uint16_t p) {
        port = p;
        addr = a;
    }

    bool connect() {
        BLYNK_LOG("Connecting to %d.%d.%d.%d:%d", addr[3], addr[2], addr[1], addr[0], port);
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

    void flush() { }
    bool connected() { return client.connected(); }
    int available() { return client.available(); }

private:
    WildFire_CC3000& cc3000;
    WildFire_CC3000_Client client;
    IPAddress   addr;
    uint16_t    port;
};

class BlynkWildFire
    : public BlynkProtocol<BlynkTransportWildFire>
{
    typedef BlynkProtocol<BlynkTransportWildFire> Base;
public:
    BlynkWildFire(WildFire_CC3000& cc3000, BlynkTransportWildFire& conn)
        : Base(conn), cc3000(cc3000)
    {}

    void wifi_begin (const char* ssid,
                     const char* pass,
                     uint8_t secmode)
    {
        if (!cc3000.begin())
        {
            BLYNK_FATAL("Couldn't begin()! Check your wiring?");
        }
        BLYNK_LOG("Connecting to %s...", ssid);
        if (!cc3000.connectToAP(ssid, pass, secmode)) {
            BLYNK_FATAL("Failed to connect to AP");
        }
        BLYNK_LOG("Getting IP address...");
        while (!cc3000.checkDHCP())
        {
            delay(100);
        }
#ifdef BLYNK_PRINT
        uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
        if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
        {
            BLYNK_FATAL("Unable to get the IP Address");
        }
        uint8_t* addr = (uint8_t*)&ipAddress;
        BLYNK_LOG("My IP: %d.%d.%d.%d", addr[3], addr[2], addr[1], addr[0]);
#endif
    }

    void begin( const char* auth,
                const char* ssid,
                const char* pass,
                uint8_t secmode,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        wifi_begin(ssid, pass, secmode);
        uint32_t ip = 0;
        BLYNK_LOG("Looking for %s", domain);
        while (ip == 0) {
            if (!cc3000.getHostByName((char*)domain, &ip)) {
                BLYNK_LOG("Couldn't locate server");
                delay(500);
            }
        }

        this->conn.begin(ip, port);
    }

    void begin( const char* auth,
                const char* ssid,
                const char* pass,
                uint8_t secmode,
                IPAddress addr,
                uint16_t port)
    {
        Base::begin(auth);
        wifi_begin(ssid, pass, secmode);
        this->conn.begin(addr, port);
    }
private:
    WildFire_CC3000& cc3000;
};

#endif
