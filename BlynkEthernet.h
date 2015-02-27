/**
 * @file       BlynkEthernet.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkEthernet_h
#define BlynkEthernet_h

#include <Blynk/BlynkProtocol.h>
#include <BlynkArduinoClient.h>
#include <Ethernet.h>

static const byte _blynkEthernetMac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

class BlynkEthernet
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkEthernet(BlynkArduinoClient& conn)
        : BlynkProtocol<BlynkArduinoClient>(conn)
    {}

    void begin( const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_DEFAULT_PORT,
                const byte mac[]   = _blynkEthernetMac)
    {
        BLYNK_LOG("Using DHCP");
        Base::begin(auth);
        Ethernet.begin((byte*)mac);
        this->conn.begin(domain, NULL, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
        // give the Ethernet shield a second to initialize:
        delay(1000);
    }

    void begin( const char* auth,
                const char* domain,
                uint16_t port,
                IPAddress ip,
                const byte mac[]= _blynkEthernetMac)
    {
        BLYNK_LOG("Using static ip");
        Base::begin(auth);
        Ethernet.begin((byte*)mac, ip);
        this->conn.begin(domain, NULL, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
        // give the Ethernet shield a second to initialize:
        delay(1000);
    }

    void begin( const char* auth,
                const byte addr[],
                uint16_t port,
                const byte mac[] = _blynkEthernetMac)
    {
        BLYNK_LOG("Using DHCP");
        Base::begin(auth);
        Ethernet.begin((byte*)mac);
        this->conn.begin(NULL, addr, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
        // give the Ethernet shield a second to initialize:
        delay(1000);
    }

    void begin( const char* auth,
                const byte addr[],
                uint16_t port,
                IPAddress ip,
                const byte mac[] = _blynkEthernetMac)
    {
        BLYNK_LOG("Using static ip");
        Base::begin(auth);
        Ethernet.begin((byte*)mac, ip);
        this->conn.begin(NULL, addr, port);
        IPAddress myip = Ethernet.localIP();
        BLYNK_LOG("My IP: %d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
        // give the Ethernet shield a second to initialize:
        delay(1000);
    }

    void run(void)
    {
        if(!this->conn.connected()) {
            this->connect();
        }
        if (this->conn.available()) {
            this->processInput();
        }
    }

};

#endif
