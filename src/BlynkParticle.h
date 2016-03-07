/**
 * @file       BlynkParticle.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkParticle_h
#define BlynkParticle_h

#define BLYNK_INFO_CONNECTION "CC3000"

#include "BlynkApiParticle.h"
#include "Blynk/BlynkProtocol.h"

class BlynkTransportParticle
{
public:
    BlynkTransportParticle()
        : domain(NULL), port(0)
    {}

    void begin(IPAddress a, uint16_t p) {
        domain = NULL;
        port = p;
        addr = a;
    }

    void begin(const char* d, uint16_t p) {
        domain = d;
        port = p;
    }

    bool connect() {
        if (domain) {
            BLYNK_LOG("Connecting to %s:%d", domain, port);
            return (1 == client.connect(domain, port));
        } else {
            BLYNK_LOG("Connecting to %d.%d.%d.%d:%d", addr[0], addr[1], addr[2], addr[3], port);
            return (1 == client.connect(addr, port));
        }
        return 0;
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
    TCPClient   client;
    IPAddress   addr;
    const char* domain;
    uint16_t    port;
};

class BlynkParticle
    : public BlynkProtocol<BlynkTransportParticle>
{
    typedef BlynkProtocol<BlynkTransportParticle> Base;
public:
    BlynkParticle(BlynkTransportParticle& transp)
        : Base(transp)
    {}

    void begin( const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t port      = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        ::delay(1000); // Give the board time to settle
        this->conn.begin(domain, port);
    }

    void begin( const char* auth,
                IPAddress addr,
                uint16_t port)
    {
        Base::begin(auth);
        ::delay(1000); // Give the board time to settle
        this->conn.begin(addr, port);
    }
private:

};

#endif
