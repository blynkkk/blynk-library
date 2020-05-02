/**
 * @file       BlynkSpresenseLTE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2020
 * @brief
 *
 */

#ifndef BlynkSpresenseLTE_h
#define BlynkSpresenseLTE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "SprLTE"
#endif

#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 60
#endif

#ifndef BLYNK_TIMEOUT_MS
#define BLYNK_TIMEOUT_MS 6000
#endif

#define BLYNK_SEND_ATOMIC

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <LTE.h>

class BlynkSIM
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkSIM(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    bool connectNetwork(const char* apn, const char* user, const char* pass)
    {
        // If your SIM has PIN, pass it as a parameter of begin() in quotes
        BLYNK_LOG1(BLYNK_F("Connecting to network..."));
        while (true) {
            if (lte.begin() == LTE_SEARCHING) {
                if (lte.attach(apn, user, pass) == LTE_READY) {
                    BLYNK_LOG1(BLYNK_F("Connected to LTE"));
                    break;
                }
                BLYNK_LOG1(BLYNK_F("An error occurred, shutdown and try again."));
                lte.shutdown();
                sleep(1);
            }
        }
        return true;
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.setClient(&client);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               const char* apn,
               const char* user,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(auth, domain, port);
        connectNetwork(apn, user, pass);
        while(this->connect() != true) {}
    }

private:
    LTE       lte;
    LTEClient client;
};

#endif
