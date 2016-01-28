/**
 * @file       BlynkSimpleShieldEsp8266.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jun 2015
 * @brief
 *
 */

#ifndef BlynkSimpleShieldEsp8266_h
#define BlynkSimpleShieldEsp8266_h

#ifdef ESP8266
#error This code is not intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "ESP8266"
#endif

#define BLYNK_SEND_ATOMIC

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>
#include <ESP8266_SoftSer.h>

class BlynkTransportShieldEsp8266
{
    static void onData(uint8_t mux_id, uint32_t len, void* ptr) {
        ((BlynkTransportShieldEsp8266*)ptr)->onData(mux_id, len);
    }

    void onData(uint8_t mux_id, uint32_t len) {
        //sBLYNK_LOG("Got %d..", len);
        while (len) {
            if (client->getUart()->available()) {
                uint8_t b = client->getUart()->read();
                if(!buffer.push(b)) {
                    BLYNK_LOG("Buffer overflow");
                }
                len--;
            }
        }
    }

public:
    BlynkTransportShieldEsp8266()
        : client(NULL)
        , status(false)
        , domain(NULL)
        , port(0)
    {}

    void begin_domain(ESP8266* esp8266, const char* d,  uint16_t p) {
        client = esp8266;
        client->setOnData(onData, this);
        domain = d;
        port = p;
    }

    bool connect() {
        if (!domain || !port)
            return false;
        status = client->createTCP(domain, port);
        return status;
    }

    void disconnect() {
        status = false;
        buffer.clear();
        client->releaseTCP();
    }

    size_t read(void* buf, size_t len) {
        uint32_t start = millis();
        //BLYNK_LOG("Waiting: %d, Occuied: %d", len, buffer.getOccupied());
        while ((buffer.getOccupied() < len) && (millis() - start < 1500)) {
            client->run();
        }
        return buffer.read((uint8_t*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        if (client->send((const uint8_t*)buf, len)) {
            return len;
        }
        return 0;
    }

    bool connected() { return status; }

    int available() {
        client->run();
	//BLYNK_LOG("Still: %d", buffer.getOccupied());
        return buffer.getOccupied();
    }

private:
    ESP8266* client;
    bool status;
    BlynkFifo<uint8_t,256> buffer;
    const char* domain;
    uint16_t    port;
};

class BlynkWifi
    : public BlynkProtocol<BlynkTransportShieldEsp8266>
{
    typedef BlynkProtocol<BlynkTransportShieldEsp8266> Base;
public:
    BlynkWifi(BlynkTransportShieldEsp8266& transp)
        : Base(transp)
        , wifi(NULL)
    {}

    bool connectWiFi(const char* ssid, const char* pass)
    {
        delay(500);
        BLYNK_LOG("Connecting to %s", ssid);
        /*if (!wifi->restart()) {
            BLYNK_LOG("Failed to restart");
            return false;
        }*/
        if (!wifi->setEcho(0)) {
            BLYNK_LOG("Failed to disable Echo");
            return false;
        }
        if (!wifi->setOprToStation()) {
            BLYNK_LOG("Failed to set STA mode");
            return false;
        }
        if (wifi->joinAP(ssid, pass)) {
            BLYNK_LOG("IP: %s", wifi->getLocalIP().c_str());
        } else {
            BLYNK_LOG("Failed to connect WiFi");
            return false;
        }
        if (!wifi->disableMUX()) {
            BLYNK_LOG("Failed to disable MUX");
        }
        BLYNK_LOG("Connected to WiFi");
        return true;
    }

    void config(ESP8266&    esp8266,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        wifi = &esp8266;
        this->conn.begin_domain(wifi, domain, port);
    }

    void begin(const char* auth,
               ESP8266&    esp8266,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(esp8266, auth, domain, port);
        connectWiFi(ssid, pass);
    }

private:
    ESP8266* wifi;
};

static BlynkTransportShieldEsp8266 _blynkTransport;
BlynkWifi Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
