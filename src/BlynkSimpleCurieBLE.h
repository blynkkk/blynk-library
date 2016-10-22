/**
 * @file       BlynkSimpleCurieBLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleCurieBLE_h
#define BlynkSimpleCurieBLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "CurieBLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo2.h>
#include <CurieBLE.h>

class BlynkTransportCurieBLE
{
public:
    BlynkTransportCurieBLE()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char* h, uint16_t p) {}

    void begin(BLEPeripheral& per) {
        instance = this;

        blePeripheral = &per;
        // Add service and characteristic
        blePeripheral->setAdvertisedServiceUuid(bleService.uuid());
        blePeripheral->addAttribute(bleService);
        blePeripheral->addAttribute(rxChar);
        blePeripheral->addAttribute(txChar);

        unsigned char empty[0] = {};
        rxChar.setValue(empty, 0);
        txChar.setValue(empty, 0);

        // Assign event handlers for connected, disconnected to peripheral
        blePeripheral->setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
        rxChar.setEventHandler(BLEWritten,    rxCharWritten);
        txChar.setEventHandler(BLESubscribed, txCharSubscribed);
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
                blePeripheral->poll();
            } else {
                break;
            }
        }
        uint32_t key = interrupt_lock();
        size_t res = mBuffRX.get((uint8_t*)buf, len);
        interrupt_unlock(key);
        return res;
    }

    size_t write(const void* buf, size_t len) {
        txChar.setValue((uint8_t*)buf, len);
        return len;
    }

    size_t available() {
        uint32_t key = interrupt_lock();
        size_t rxSize = mBuffRX.size();
        interrupt_unlock(key);
        return rxSize;
    }

private:
    static BlynkTransportCurieBLE* instance;

    static
    void rxCharWritten(BLECentral& central, BLECharacteristic& ch) {
        if (!instance)
            return;
        uint32_t key = interrupt_lock();
        const uint8_t* data = ch.value();
        uint32_t len = ch.valueLength();
        //BLYNK_DBG_DUMP(">> ", data, len);
        instance->mBuffRX.put(data, len);
        interrupt_unlock(key);
    }

    static
    void txCharSubscribed(BLECentral& central, BLECharacteristic& ch);

    static
    void blePeripheralDisconnectHandler(BLECentral& central);

private:
    bool mConn;
    BLEPeripheral*    blePeripheral = NULL;
    BLEService        bleService = BLEService       ("713D0000-503E-4C75-BA94-3148F18D941E");
    BLECharacteristic rxChar     = BLECharacteristic("713D0003-503E-4C75-BA94-3148F18D941E", BLEWrite | BLEWriteWithoutResponse, BLE_MAX_ATTR_DATA_LEN);
    BLECharacteristic txChar     = BLECharacteristic("713D0002-503E-4C75-BA94-3148F18D941E", BLERead  | BLENotify,               BLE_MAX_ATTR_DATA_LEN);

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkCurieBLE
    : public BlynkProtocol<BlynkTransportCurieBLE>
{
    typedef BlynkProtocol<BlynkTransportCurieBLE> Base;
public:
    BlynkCurieBLE(BlynkTransportCurieBLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth, BLEPeripheral& per)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin(per);
    }
};

BlynkTransportCurieBLE* BlynkTransportCurieBLE::instance = NULL;

static BlynkTransportCurieBLE _blynkTransport;
BlynkCurieBLE Blynk(_blynkTransport);

inline
void BlynkTransportCurieBLE::txCharSubscribed(BLECentral& central, BLECharacteristic& ch) {
    Blynk.startSession();
}

inline
void BlynkTransportCurieBLE::blePeripheralDisconnectHandler(BLECentral& central) {
    Blynk.disconnect();
}

#include <BlynkWidgets.h>

#endif
