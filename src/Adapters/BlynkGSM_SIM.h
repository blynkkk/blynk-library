/**
 * @file       BlynkGSM_SIM.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Nov 2016
 * @brief
 *
 */

#ifndef BlynkGSM_SIM_h
#define BlynkGSM_SIM_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "GSM SIM"
#endif

#ifndef BLYNK_GSM_MUX
#define BLYNK_GSM_MUX  1
#endif

#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 60
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_TIMEOUT_MS 5000

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>
#include <GSM_Lib.h>

typedef ESP8266 GSM;

class BlynkTransportSIM
{
    static void onData(uint8_t mux_id, uint32_t len, void* ptr) {
        ((BlynkTransportSIM*)ptr)->onData(mux_id, len);
    }

    void onData(uint8_t mux_id, uint32_t len) {
        if (mux_id != BLYNK_GSM_MUX) {
            return;
        }
        //BLYNK_LOG2("Got ", len);
        while (len) {
            if (client->getUart()->available()) {
                uint8_t b = client->getUart()->read();
                if(!buffer.push(b)) {
                    BLYNK_LOG1(BLYNK_F("Buffer overflow"));
                }
                len--;
            }
        }
    }

public:
    BlynkTransportSIM()
        : client(NULL)
        , status(false)
        , domain(NULL)
        , port(0)
    {}

    void setGsm(GSM* gsm) {
        client = gsm;
        client->setOnData(onData, this);
    }

    void begin(const char* d,  uint16_t p) {
        domain = d;
        port = p;
    }

    bool connect() {
        if (!domain || !port)
            return false;
        status = client->createTCP(BLYNK_GSM_MUX, domain, port);
        return status;
    }

    void disconnect() {
        status = false;
        buffer.clear();
        client->releaseTCP(BLYNK_GSM_MUX);
    }

    size_t read(void* buf, size_t len) {
        uint32_t start = millis();
        //BLYNK_LOG4("Waiting: ", len, " Occuied: ", buffer.getOccupied());
        while ((buffer.getOccupied() < len) && (millis() - start < 1500)) {
            client->run();
        }
        return buffer.read((uint8_t*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        if (client->send(BLYNK_GSM_MUX, (const uint8_t*)buf, len)) {
            return len;
        }
        return 0;
    }

    bool connected() { return status; }

    int available() {
        client->run();
        //BLYNK_LOG2("Still: ", buffer.getOccupied());
        return buffer.getOccupied();
    }

private:
    GSM* client;
    bool status;
    BlynkFifo<uint8_t,256> buffer;
    const char* domain;
    uint16_t    port;
};

class BlynkSIM
    : public BlynkProtocol<BlynkTransportSIM>
{
    typedef BlynkProtocol<BlynkTransportSIM> Base;
public:
    BlynkSIM(BlynkTransportSIM& transp)
        : Base(transp)
        , module(NULL)
    {}

    bool connectWiFi(const char* apn, const char* user, const char* pass)
    {
        BLYNK_LOG1(BLYNK_F("Initializing modem..."));
        if (!module->restart()) {
            BLYNK_FATAL(BLYNK_F("Failed to init modem"));
            return false;
        }

        // TODO: Check SIM: AT+CCID
        // Turn on verbose errors: ATI+CMEE=2
        // Wait for registered AT+CREG?

        delay(1000);
        if (!module->enableMUX()) {
            BLYNK_FATAL(BLYNK_F("Failed to enable MUX"));
        }
        BLYNK_LOG2(BLYNK_F("Connecting to "), apn);
        if (module->attachGprs(apn, user, pass)) {
            String my_ip = module->getLocalIP();
            BLYNK_LOG2(BLYNK_F("Local IP:"), my_ip);
        } else {
            BLYNK_FATAL(BLYNK_F("Failed to connect GSM"));
            return false;
        }

        BLYNK_LOG1(BLYNK_F("Connected to GSM"));
        return true;
    }

    void config(GSM&        gsm,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        module = &gsm;
        this->conn.setGsm(module);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               GSM&        gsm,
               const char* apn,
               const char* user,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(gsm, auth, domain, port);
        connectWiFi(apn, user, pass);
        while(this->connect() != true) {}
    }

private:
    GSM* module;
};

#endif
