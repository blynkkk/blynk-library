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
  #if defined(TINY_GSM_MODEM_SIM800)
    #define BLYNK_INFO_CONNECTION  "SIM800"
  #elif defined(TINY_GSM_MODEM_SIM900)
    #define BLYNK_INFO_CONNECTION  "SIM900"
  #elif defined(TINY_GSM_MODEM_UBLOX)
    #define BLYNK_INFO_CONNECTION  "UBLOX"
  #elif defined(TINY_GSM_MODEM_BG96)
    #define BLYNK_INFO_CONNECTION  "BG96"
  #elif defined(TINY_GSM_MODEM_A6)
    #define BLYNK_INFO_CONNECTION  "A6"
  #elif defined(TINY_GSM_MODEM_A7)
    #define BLYNK_INFO_CONNECTION  "A7"
  #elif defined(TINY_GSM_MODEM_M590)
    #define BLYNK_INFO_CONNECTION  "M590"
  #elif defined(TINY_GSM_MODEM_XBEE)
    #define BLYNK_INFO_CONNECTION  "XBEE"
  #else
    #define BLYNK_INFO_CONNECTION  "TinyGSM"
  #endif
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
#include <TinyGsmClient.h>

class BlynkSIM
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkSIM(BlynkArduinoClient& transp)
        : Base(transp)
        , modem(NULL)
    {}

    bool connectNetwork(const char* apn, const char* user, const char* pass)
    {
        BLYNK_LOG1(BLYNK_F("Modem init..."));
        if (!modem->begin()) {
           BLYNK_FATAL(BLYNK_F("Cannot init"));
        }

        switch (modem->getSimStatus()) {
        case SIM_ERROR:  BLYNK_FATAL(BLYNK_F("SIM is missing"));    break;
        case SIM_LOCKED: BLYNK_FATAL(BLYNK_F("SIM is PIN-locked")); break;
        default: break;
        }

        BLYNK_LOG1(BLYNK_F("Connecting to network..."));
        if (modem->waitForNetwork()) {
          String op = modem->getOperator();
          BLYNK_LOG2(BLYNK_F("Network: "), op);
        } else {
          BLYNK_FATAL(BLYNK_F("Register in network failed"));
        }

        BLYNK_LOG3(BLYNK_F("Connecting to "), apn, BLYNK_F(" ..."));
        if (!modem->gprsConnect(apn, user, pass)) {
            BLYNK_FATAL(BLYNK_F("Connect GPRS failed"));
            return false;
        }

        BLYNK_LOG1(BLYNK_F("Connected to GPRS"));
        return true;
    }

    void config(TinyGsm&    gsm,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        modem = &gsm;
        client.init(modem);
        this->conn.setClient(&client);
        this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               TinyGsm&    gsm,
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
    TinyGsm*      modem;
    TinyGsmClient client;
};

#endif
