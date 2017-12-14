/**
 * @file       BlynkSimpleRFduinoBLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleRFduinoBLE_h
#define BlynkSimpleRFduinoBLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "RFduinoBLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>
#include <RFduinoBLE.h>

class BlynkTransportRFduinoBLE
{
public:
    BlynkTransportRFduinoBLE()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char BLYNK_UNUSED *h, uint16_t BLYNK_UNUSED p) {}

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
        millis_time_t start = BlynkMillis();
        while (BlynkMillis() - start < BLYNK_TIMEOUT_MS) {
            if (available() < len) {
                BlynkDelay(1);
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
        RFduinoBLE.send((const char*)buf, len);
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
    static BlynkTransportRFduinoBLE* instance;

private:
    bool mConn;

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkSimpleRFduinoBLE
    : public BlynkProtocol<BlynkTransportRFduinoBLE>
{
    typedef BlynkProtocol<BlynkTransportRFduinoBLE> Base;
public:
    BlynkSimpleRFduinoBLE(BlynkTransportRFduinoBLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin();
    }
};

BlynkTransportRFduinoBLE* BlynkTransportRFduinoBLE::instance = NULL;

static BlynkTransportRFduinoBLE _blynkTransport;
BlynkSimpleRFduinoBLE Blynk(_blynkTransport);


void RFduinoBLE_onConnect()
{
  BLYNK_LOG1("Device connected");
  Blynk.startSession();
}

void RFduinoBLE_onDisconnect()
{
  BLYNK_LOG1("Device disconnected");
  Blynk.disconnect();
}

void RFduinoBLE_onReceive(char* data, int len)
{
  _blynkTransport.putData((uint8_t*)data, len);
}

#include <BlynkWidgets.h>

#endif
