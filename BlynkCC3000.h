/**
 * @file       BlynkCC3000.h
 * @author     Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkCC3000_h
#define BlynkCC3000_h

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adafruit_CC3000.h>

class BlynkTransportCC3000
{
public:
    BlynkTransportCC3000(Adafruit_CC3000& cc3000)
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

    void disconnect() { client.stop(); }

    size_t read(void* buf, size_t len) {
        return client.readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return client.write((const uint8_t*)buf, len);
    }

    void flush() { client.flush(); }
    bool connected() { return client.connected(); }
    int available() { return client.available(); }

private:
    Adafruit_CC3000& cc3000;
    Adafruit_CC3000_Client client;
    IPAddress   addr;
    uint16_t    port;
};

class BlynkCC3000
    : public BlynkProtocol<BlynkTransportCC3000>
{
    typedef BlynkProtocol<BlynkTransportCC3000> Base;
public:
    BlynkCC3000(Adafruit_CC3000& cc3000, BlynkTransportCC3000& conn)
        : cc3000(cc3000), Base(conn)
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
        while (ip == 0) {
            if (!cc3000.getHostByName((char*)domain, &ip)) {
                BLYNK_LOG("Couldn't resolve %s", domain);
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
    Adafruit_CC3000& cc3000;
};

#endif
