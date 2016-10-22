/**
 * @file       BlynkSimpleSimbleeBLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleSimbleeBLE_h
#define BlynkSimpleSimbleeBLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "SimbleeBLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo2.h>
#include <SimbleeBLE.h>

class BlynkTransportSimbleeBLE
{
public:
    BlynkTransportSimbleeBLE()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char* h, uint16_t p) {}

    void begin() {
        instance = this;
    }

    bool connect() {
        mBuffRX.clear();
        return mConn = true;
    }

    void disconnect() {
        mConn = false;
    }

    bool connected() {
        return mConn;
    }

    size_t read(void* buf, size_t len) {
        uint32_t start = millis();
        while (millis() - start < BLYNK_TIMEOUT_MS) {
            if (available() < len) {
                ::delay(1);
            } else {
                break;
            }
        }
        noInterrupts();
        size_t res = mBuffRX.get((uint8_t*)buf, len);
        interrupts();
        return res;
    }

    size_t write(const void* buf, size_t len) {
        SimbleeBLE.send((const char*)buf, len);
        return len;
    }

    size_t available() {
        noInterrupts();
        size_t rxSize = mBuffRX.size();
        interrupts();
        return rxSize;
    }

    static
    int putData(uint8_t* data, uint16_t len) {
        if (!instance)
            return 0;
        noInterrupts();
        //BLYNK_DBG_DUMP(">> ", data, len);
        instance->mBuffRX.put(data, len);
        interrupts();
        return 0;
    }

private:
    static BlynkTransportSimbleeBLE* instance;

private:
    bool mConn;

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkSimpleSimbleeBLE
    : public BlynkProtocol<BlynkTransportSimbleeBLE>
{
    typedef BlynkProtocol<BlynkTransportSimbleeBLE> Base;
public:
    BlynkSimpleSimbleeBLE(BlynkTransportSimbleeBLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin();
    }
};

BlynkTransportSimbleeBLE* BlynkTransportSimbleeBLE::instance = NULL;

static BlynkTransportSimbleeBLE _blynkTransport;
BlynkSimpleSimbleeBLE Blynk(_blynkTransport);


void SimbleeBLE_onConnect()
{
  BLYNK_LOG1("Device connected");
  Blynk.startSession();
}

void SimbleeBLE_onDisconnect()
{
  BLYNK_LOG1("Device disconnected");
  Blynk.disconnect();
}

void SimbleeBLE_onReceive(char* data, int len)
{
  _blynkTransport.putData((uint8_t*)data, len);
}

#include <BlynkWidgets.h>

#endif
