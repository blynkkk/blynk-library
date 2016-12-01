/**
 * @file       BlynkGsmClient.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Nov 2016
 * @brief
 *
 */

#ifndef BlynkGsmClient_h
#define BlynkGsmClient_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "GSM SIM"
#endif

#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 60
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_TIMEOUT_MS 5000

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <TinyGsmClient.h>

typedef BlynkArduinoClientGen<TinyGsmClient> BlynkTransportSIM;

class BlynkSIM
    : public BlynkProtocol<BlynkTransportSIM>
{
    typedef BlynkProtocol<BlynkTransportSIM> Base;
public:
    BlynkSIM(BlynkTransportSIM& transp)
        : Base(transp)
        , client(NULL)
    {}

    bool connectNetwork(const char* apn, const char* user, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), apn);
        if (client->networkConnect(apn, user, pass)) {
            //TODO:
            //String my_ip = client->getLocalIP();
            //BLYNK_LOG2(BLYNK_F("Local IP:"), my_ip);
        } else {
            BLYNK_FATAL(BLYNK_F("Failed to connect GSM"));
            return false;
        }

        BLYNK_LOG1(BLYNK_F("Connected to GSM"));
        return true;
    }

    void config(TinyGsmClient&  gsm,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        client = &gsm;
        this->conn.setClient(client);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               TinyGsmClient&  gsm,
               const char* apn,
               const char* user,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(gsm, auth, domain, port);
        connectNetwork(apn, user, pass);
        while(this->connect() != true) {}
    }

private:
    TinyGsmClient* client;
};

#endif
