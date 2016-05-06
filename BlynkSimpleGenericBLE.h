/**
 * @file       BlynkSimpleGenericBLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleGenericBLE_h
#define BlynkSimpleGenericBLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "BLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20
#define BLYNK_MSG_LIMIT 0

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo2.h>
#include <CurieBLE.h>

BLEPeripheral     blePeripheral;
BLEService        bleService  ("713D0000-503E-4C75-BA94-3148F18D941E");
BLECharacteristic rxChar      ("713D0003-503E-4C75-BA94-3148F18D941E", BLEWrite | BLEWriteWithoutResponse, BLE_MAX_ATTR_DATA_LEN);
BLECharacteristic txChar      ("713D0002-503E-4C75-BA94-3148F18D941E", BLERead  | BLENotify, BLE_MAX_ATTR_DATA_LEN);

class BlynkTransportGenericBLE
{
public:
    BlynkTransportGenericBLE()
        : mConn (false)
    {}

    bool connect() {
        mBuffRX.clear();
        mConn = true;
        return true;
    }

    void disconnect() {
    	mConn = false;
    }

    bool connected() {
        return mConn;
    }

    size_t read(void* buf, size_t len) {
        uint32_t start = millis();
        while (millis() - start < 1500) {
            if (available() < len) {
                BLYNK_LOG1("poll");
                blePeripheral.poll();
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
        //BLYNK_DBG_DUMP("<< ", buf, len);
        txChar.setValue((uint8_t*)buf, len);
        return len;
    }

    void process(const void* data, size_t len) {
        noInterrupts();
        //BLYNK_DBG_DUMP(">> ", data, len);
        mBuffRX.put((uint8_t*)data, len);
        interrupts();
    }

    size_t available() {
        noInterrupts();
        size_t rxSize = mBuffRX.size();
        interrupts();
        return rxSize;
    }

private:
    bool mConn;
    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkGenericBLE
    : public BlynkProtocol<BlynkTransportGenericBLE>
{
    typedef BlynkProtocol<BlynkTransportGenericBLE> Base;
public:
    BlynkGenericBLE(BlynkTransportGenericBLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        disconnect();
    }

    void bleConnect() {
    	startSession();
    }

    void bleDisconnect() { disconnect(); }

    void bleProcess(const void* data, size_t len) {
        conn.process(data, len);
    }

private:
};

static BlynkTransportGenericBLE _blynkTransport;
BlynkGenericBLE Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
