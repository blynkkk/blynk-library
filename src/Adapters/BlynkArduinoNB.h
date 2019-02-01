/**
 * @file       BlynkArduinoNB.h
 * @author     Riccardo Rizzo
 * @license    This project is released under the MIT License (MIT)
 * @date       Dec 2018
 * @brief
 *
 */

#ifndef BlynkArduinoNB_h
#define BlynkArduinoNB_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "MKRNB"
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
#include <MKRNB.h>

class BlynkSIM
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkSIM(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    bool connectNetwork(NB& modem, GPRS& gprs, const char* pin)
    {
        BLYNK_LOG1(BLYNK_F("Modem init..."));
        if (!modem.begin(pin)) {
           BLYNK_FATAL(BLYNK_F("Cannot init"));
           return false;
        }

        BLYNK_LOG3(BLYNK_F("Connecting to "),"", BLYNK_F(" ..."));
        if (!gprs.attachGPRS()) {
            BLYNK_FATAL(BLYNK_F("Connect GPRS failed"));
            return false;
        }

        BLYNK_LOG1(BLYNK_F("Connected to GPRS"));
        return true;
    }

    void config(NB&        nb,
                GPRS&       gprs,
                NBClient&  nbclient,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        modem = &nb;
        modemGPRS = &gprs;

        this->conn.setClient(&nbclient);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               NB&        nb,
               GPRS&       gprs,
               NBClient&  nbclient,
               const char* pin,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(nb, gprs, nbclient, auth, domain, port);
        connectNetwork(nb, gprs, pin);
        while(this->connect() != true) {}
    }

private:
    NB*       modem;
    GPRS*     modemGPRS;
    NBClient* nbclient;
};

#endif
