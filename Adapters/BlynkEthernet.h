/**
 * @file       BlynkEthernet.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkEthernet_h
#define BlynkEthernet_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "W5000"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

static const byte _blynkEthernetMac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

class BlynkEthernet
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkEthernet(BlynkArduinoClient& transp)
        : Base(transp)
    {}

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

    // DHCP with domain
    void begin( const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_DEFAULT_PORT,
                const byte mac[]   = _blynkEthernetMac)
    {
        Base::begin(auth);
        BLYNK_LOG("Getting IP...");
        if (!Ethernet.begin((byte*)mac)) {
            BLYNK_FATAL("DHCP Failed!");
        }
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(domain, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    // Static IP with domain
    void begin( const char* auth,
                const char* domain,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                const byte mac[]= _blynkEthernetMac)
    {
        Base::begin(auth);
        BLYNK_LOG("Using static IP");
        Ethernet.begin((byte*)mac, local);
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(domain, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    // Static IP with domain, gateway, etc
    void begin( const char* auth,
                const char* domain,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                IPAddress gateway,
                IPAddress subnet,
                const byte mac[]= _blynkEthernetMac)
    {
        Base::begin(auth);
        BLYNK_LOG("Using static IP");
        Ethernet.begin((byte*)mac, local, dns, gateway, subnet);
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(domain, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    // DHCP with server IP
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port    = BLYNK_DEFAULT_PORT,
                const byte mac[] = _blynkEthernetMac)
    {
        Base::begin(auth);
        BLYNK_LOG("Getting IP...");
        if (!Ethernet.begin((byte*)mac)) {
            BLYNK_FATAL("DHCP Failed!");
        }
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(addr, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    // Static IP with server IP
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port,
                IPAddress local,
                const byte mac[] = _blynkEthernetMac)
    {
        BLYNK_LOG("Using static IP");
        Base::begin(auth);
        Ethernet.begin((byte*)mac, local);
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(addr, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

    // Static IP with server IP, DNS, gateway, etc
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                IPAddress gateway,
                IPAddress subnet,
                const byte mac[] = _blynkEthernetMac)
    {
        BLYNK_LOG("Using static IP");
        Base::begin(auth);
        Ethernet.begin((byte*)mac, local, dns, gateway, subnet);
        // give the Ethernet shield a second to initialize:
        ::delay(1000);
        this->conn.begin(addr, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
    }

};

#endif
