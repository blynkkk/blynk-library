/**
 * @file       BlynkMultiClient.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2022 Volodymyr Shymanskyy
 * @date       Oct 2022
 * @brief
 *
 */

#ifndef BlynkMultiClient_h
#define BlynkMultiClient_h

static const char BLYNK_DEFAULT_ROOT_CA[] =
#include <certs/letsencrypt_pem.h>

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

#ifndef BLYNK_MAX_CLIENT_QTY
#define BLYNK_MAX_CLIENT_QTY 4
#endif

template <typename T>
class BlynkArduinoClientMulti
    : public BlynkArduinoClientGen<T>
{
public:
    BlynkArduinoClientMulti() {}

    void addClient(T& c) {
        if (_conn_qty == 0) {
            BlynkArduinoClientGen<T>::setClient(&c);
        }
        if (_conn_qty < BLYNK_MAX_CLIENT_QTY) {
            _conn_array[_conn_qty++] = &c;
        }
    }

    bool connect() {
        for (int i = 0; i < _conn_qty; i++) {
            BlynkArduinoClientGen<T>::setClient(_conn_array[i]);
            if (BlynkArduinoClientGen<T>::connect()) {
                BLYNK_LOG3(BLYNK_F("Connection "), i, F(" OK"));
                return true;
            } else {
                BLYNK_LOG3(BLYNK_F("Connection "), i, F(" failed"));
            }
        }
        return false;
    }

    void disconnect() {
        BlynkArduinoClientGen<T>::isConn = false;
        for (int i = 0; i < _conn_qty; i++) {
            _conn_array[i]->stop();
        }
    }

private:
    T*  _conn_array[BLYNK_MAX_CLIENT_QTY] = { NULL, };
    int _conn_qty = 0;
};

template <typename Transport>
class BlynkMultiClient
    : public BlynkProtocol<Transport>
{
    typedef BlynkProtocol<Transport> Base;
public:
    BlynkMultiClient(Transport& transp)
        : Base(transp)
    {}

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

};

typedef BlynkArduinoClientMulti<Client> BlynkTransport;

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkTransport   _blynkTransport;

  BlynkMultiClient<BlynkTransport> Blynk(_blynkTransport);
#else
  extern BlynkMultiClient<BlynkTransport> Blynk;
#endif

#include <BlynkWidgets.h>

#endif
