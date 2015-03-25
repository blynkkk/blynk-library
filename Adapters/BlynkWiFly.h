/**
 * @file       BlynkWiFly.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkWiFly_h
#define BlynkWiFly_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "WiFly"
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "RN-XV"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

class BlynkTransportWiFly
{
public:
    BlynkTransportWiFly()
        : client(NULL)
    {}

    void begin_domain(const char* d,  uint16_t p) {
        if (client)
            delete client;
        client = new WiFlyClient(d, p);
    }

    void begin_addr(IPAddress a, uint16_t p) {
        if (client)
            delete client;
        uint32_t addr = a;
        client = new WiFlyClient((uint8_t*)addr, p);
    }

    bool connect() {
        if (!client)
            return 0;
        if (client->_domain) {
            BLYNK_LOG("Connecting to %s:%d", client->_domain, client->_port);
        } else if (client->_ip) {
            BLYNK_LOG("Connecting to %d.%d.%d.%d:%d",
                client->_ip[0], client->_ip[1], client->_ip[2], client->_ip[3], client->_port);
        }
        return client->connect();
    }
    void disconnect() { client->stop(); }

    size_t read(void* buf, size_t len) {
        return client->readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return client->write((const uint8_t*)buf, len);
    }

    void flush()	{ client->flush(); }
    bool connected() { return client->connected(); }
    int available() { return client->available(); }

private:
    WiFlyClient* client;
};

class BlynkWiFly
    : public BlynkProtocol<BlynkTransportWiFly>
{
    typedef BlynkProtocol<BlynkTransportWiFly> Base;
public:
    BlynkWiFly(BlynkTransportWiFly& transp)
        : Base(transp)
    {}

    void begin( const char* auth,
                const char* ssid,
                const char* pass,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        WiFly.begin();
        if (!WiFly.join(ssid, pass)) {
            BLYNK_FATAL("WiFly: Association failed.");
        }
        this->conn.begin_domain(domain, port);
    }

    void begin( const char* auth,
                const char* ssid,
                const char* pass,
                IPAddress addr,
                uint16_t port)
    {
        Base::begin(auth);
        WiFly.begin();
        if (!WiFly.join(ssid, pass)) {
            BLYNK_FATAL("WiFly: Association failed.");
        }
        this->conn.begin_addr(addr, port);
    }

};

#endif
