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

#ifdef BLYNK_USE_SSL
  #define BLYNK_SERVER_PORT BLYNK_DEFAULT_PORT_SSL
#else
  #define BLYNK_SERVER_PORT BLYNK_DEFAULT_PORT
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

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
                uint16_t    port   = BLYNK_SERVER_PORT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_SERVER_PORT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

    // DHCP with domain
    void begin( const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_SERVER_PORT,
                const byte mac[]   = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Getting IP..."));
        if (!Ethernet.begin(SelectMacAddress(auth, mac))) {
            BLYNK_FATAL(BLYNK_F("DHCP Failed!"));
        }
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    // Static IP with domain
    void begin( const char* auth,
                const char* domain,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                const byte mac[] = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Using static IP"));
        Ethernet.begin(SelectMacAddress(auth, mac), local, dns);
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    // Static IP with domain, gateway, etc
    void begin( const char* auth,
                const char* domain,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                IPAddress gateway,
                IPAddress subnet,
                const byte mac[] = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Using static IP"));
        Ethernet.begin(SelectMacAddress(auth, mac), local, dns, gateway, subnet);
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    // DHCP with server IP
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port    = BLYNK_SERVER_PORT,
                const byte mac[] = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Getting IP..."));
        if (!Ethernet.begin(SelectMacAddress(auth, mac))) {
            BLYNK_FATAL(BLYNK_F("DHCP Failed!"));
        }
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, addr, port);
        while(this->connect() != true) {}
    }

    // Static IP with server IP
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port,
                IPAddress local,
                const byte mac[] = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Using static IP"));
        Ethernet.begin(SelectMacAddress(auth, mac), local);
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, addr, port);
        while(this->connect() != true) {}
    }

    // Static IP with server IP, DNS, gateway, etc
    void begin( const char* auth,
                IPAddress addr,
                uint16_t port,
                IPAddress local,
                IPAddress dns,
                IPAddress gateway,
                IPAddress subnet,
                const byte mac[] = NULL)
    {
        BLYNK_LOG1(BLYNK_F("Using static IP"));
        Ethernet.begin(SelectMacAddress(auth, mac), local, dns, gateway, subnet);
        // give the Ethernet shield a second to initialize:
        BlynkDelay(1000);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG_IP("IP:", myip);

        config(auth, addr, port);
        while(this->connect() != true) {}
    }

private:

    byte* SelectMacAddress(const char* token, const byte mac[])
    {
        if (mac != NULL) {
            return (byte*)mac;
        }

        macAddress[0] = 0xFE;
        macAddress[1] = 0xED;
        macAddress[2] = 0xBA;
        macAddress[3] = 0xFE;
        macAddress[4] = 0xFE;
        macAddress[5] = 0xED;

        int len = strlen(token);
        int mac_index = 1;
        for (int i=0; i<len; i++) {
            macAddress[mac_index++] ^= token[i];

            if (mac_index > 5) { mac_index = 1; }
        }
        /* BLYNK_LOG("MAC: %02X-%02X-%02X-%02X-%02X-%02X",
                  macAddress[0], macAddress[1],
                  macAddress[2], macAddress[3],
                  macAddress[4], macAddress[5]);
        */
        return macAddress;
    }

private:
    byte macAddress[6];

};

#endif
