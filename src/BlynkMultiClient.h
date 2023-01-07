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
    struct Entry {
        String    name;
        T*        client;
        uint16_t  port;
    };
public:
    BlynkArduinoClientMulti() {}

    void addClient(const String& name, T& c, uint16_t port = BLYNK_DEFAULT_PORT_SSL) {
        if (_conn_qty == 0) {
            BlynkArduinoClientGen<T>::setClient(&c);
            BlynkArduinoClientGen<T>::port = port;
        }
        if (_conn_qty < BLYNK_MAX_CLIENT_QTY) {
            Entry& e = _conn_array[_conn_qty++];
            e.name =   name;
            e.client = &c;
            e.port =   port;
        }
    }

    bool connect() {
        for (int i = 0; i < _conn_qty; i++) {
            BlynkArduinoClientGen<T>::setClient(_conn_array[i].client);
            BlynkArduinoClientGen<T>::port = _conn_array[i].port;
            if (BlynkArduinoClientGen<T>::connect()) {
                BLYNK_LOG2(_conn_array[i].name, BLYNK_F(" connection OK"));
                return true;
            } else {
                BLYNK_LOG2(_conn_array[i].name, BLYNK_F(" connection failed"));
            }
        }
        return false;
    }

    void disconnect() {
        BlynkArduinoClientGen<T>::isConn = false;
        for (int i = 0; i < _conn_qty; i++) {
            _conn_array[i].client->stop();
        }
    }

private:
    Entry   _conn_array[BLYNK_MAX_CLIENT_QTY];
    int     _conn_qty = 0;
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

    template<typename T>
    void addClient(const String& name, T& client, uint16_t port = BLYNK_DEFAULT_PORT_SSL) {
        this->conn.addClient(name, client, port);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN)
    {
        Base::begin(auth);
        this->conn.begin(domain, 0);
    }

    void config(const char* auth,
                IPAddress   ip)
    {
        Base::begin(auth);
        this->conn.begin(ip, 0);
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
