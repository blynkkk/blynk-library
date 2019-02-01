/**
 * @file       BlynkArduinoGSM.h
 * @author     Riccardo Rizzo
 * @license    This project is released under the MIT License (MIT)
 * @date       Dec 2018
 * @brief
 *
 */

#ifndef BlynkArduinoGSM_h
#define BlynkArduinoGSM_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "MKRGSM"
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
#include <MKRGSM.h>

class BlynkSIM
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkSIM(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    bool connectNetwork(GSM& modem, GPRS& gprs, const char* pin ,const char* apn, const char* user, const char* pass)
    {
        BLYNK_LOG1(BLYNK_F("Modem init..."));
        if (!modem.begin(pin)) {
           BLYNK_FATAL(BLYNK_F("Cannot init"));
           return false;
        }

        BLYNK_LOG3(BLYNK_F("Connecting to "), apn, BLYNK_F(" ..."));
        if (!gprs.attachGPRS(apn, user, pass)) {
            BLYNK_FATAL(BLYNK_F("Connect GPRS failed"));
            return false;
        }

        BLYNK_LOG1(BLYNK_F("Connected to GPRS"));
        return true;
    }

    void config(GSM&        gsm,
                GPRS&       gprs,
                GSMClient&  gsmclient,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        modem = &gsm;
        modemGPRS = &gprs;

        this->conn.setClient(&gsmclient);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               GSM&        gsm,
               GPRS&       gprs,
               GSMClient&  gsmclient,
               const char* pin,
               const char* apn,
               const char* user,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        config(gsm, gprs, gsmclient, auth, domain, port);
        connectNetwork(gsm, gprs, pin, apn, user, pass);
        while(this->connect() != true) {}
    }

private:
    GSM*       modem;
    GPRS*      modemGPRS;
    GSMClient* gsmclient;
};

#endif
