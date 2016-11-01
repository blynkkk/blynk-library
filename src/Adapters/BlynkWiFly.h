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

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "RN-XV"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <WiFlyHQ.h>

class BlynkTransportWiFly
{
public:
    BlynkTransportWiFly()
        : wifly(NULL)
        , domain(NULL)
        , port(0)
    {}

    void setWiFly(WiFly* rnxv) {
        wifly = rnxv;
    }

    void begin(const char* h,  uint16_t p) {
        domain = h;
        port = p;
    }

    bool connect() {
        return wifly->open(domain, port);
    }
    void disconnect() { wifly->close(); }

    size_t read(void* buf, size_t len) {
        return wifly->readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        wifly->write((const uint8_t*)buf, len);
        return len;
    }

    bool connected() { return wifly->isConnected(); }
    int available() { return wifly->available(); }

private:
    WiFly*      wifly;
    const char* domain;
    uint16_t    port;
};

class BlynkWiFly
    : public BlynkProtocol<BlynkTransportWiFly>
{
    typedef BlynkProtocol<BlynkTransportWiFly> Base;
public:
    BlynkWiFly(BlynkTransportWiFly& transp)
        : Base(transp)
        , wifly(NULL)
    {}

    void connectWiFi(const char* ssid, const char* pass) {
        if (!wifly->isAssociated()) {
            BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
            wifly->setSSID(ssid);
            if (pass && strlen(pass)) {
                 wifly->setPassphrase(pass);
            }
            wifly->enableDHCP();
            if (!wifly->join()) {
                BLYNK_FATAL("Failed.");
            }
        } else {
            BLYNK_LOG1(BLYNK_F("Already connected to WiFi"));
        }
        if (wifly->isConnected()) {
            wifly->close();
        }
        wifly->setIpFlags(1 << 1);
    }

    void config(WiFly&      rnxv,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        wifly = &rnxv;
        this->conn.setWiFly(wifly);
        this->conn.begin(domain, port);
    }

    void begin( const char* auth,
                WiFly&      rnxv,
                const char* ssid,
                const char* pass,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        wifly = &rnxv;
        connectWiFi(ssid, pass);
        config(rnxv, auth, domain, port);
        while(this->connect() != true) {}
    }

private:
    WiFly* wifly;
};

#endif
