/**
 * @file       BlynkSimpleNanoBLE.h
 * @author     Jan Hermes
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2021 Jan Hermes
 * @date       September 2021
 * @brief
 *
 */

#ifndef BlynkSimpleArduinoNano_BLE_h
#define BlynkSimpleArduinoNano_BLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "ArduinoNano_BLE"
#endif
#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>

#include <ArduinoBLE.h>

#define SERVICE_UUID           "713D0000-503E-4C75-BA94-3148F18D941E"
#define CHARACTERISTIC_UUID_RX "713D0003-503E-4C75-BA94-3148F18D941E"
#define CHARACTERISTIC_UUID_TX "713D0002-503E-4C75-BA94-3148F18D941E"

class BlynkTransportArduinoNano_BLE
{

public:
    BlynkTransportArduinoNano_BLE()
        : mConn (false)
          , mName ("Blynk")
    {}

    void setDeviceName(const char* name) {
        mName = name;
    }

    // IP redirect not available
    void begin(char BLYNK_UNUSED *h, uint16_t BLYNK_UNUSED p) {}

    void begin(const char* name) {
        mName = name;
        instance = this;
        // Create the BLE Device
        BLE.begin();
        BLE.setLocalName(mName);
        BLE.setDeviceName(mName);

        // // Start the service
        BLE.setAdvertisedService(pService);

        pService.addCharacteristic(pCharacteristicTX);
        pService.addCharacteristic(pCharacteristicRX);
        BLE.addService(pService);

        pCharacteristicTX.writeValue((uint8_t) 0);
        pCharacteristicRX.writeValue((uint8_t) 0);

        BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
        BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
        pCharacteristicRX.setEventHandler(BLEWritten, rxCharWritten);
        pCharacteristicTX.setEventHandler(BLESubscribed, txCharSubscribed);

        // Start advertising
        BLE.advertise();
    }

    bool connect() {
        mBuffRX.clear();
        return mConn = true;
    }

    void disconnect() {
        mConn = false;
    }

    bool connected() const {
        return mConn;
    }

    size_t read(void* buf, size_t len) {
        millis_time_t start = millis();
        while (millis() - start < BLYNK_TIMEOUT_MS) {
            if (available() < len) {
                BLE.poll();
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
        pCharacteristicTX.writeValue((uint8_t*)buf, len);
        return len;
    }

    size_t available() {

        noInterrupts();
        size_t rxSize = mBuffRX.size();
        interrupts();
        return rxSize;
    }

private:

    static BlynkTransportArduinoNano_BLE* instance;

    static void rxCharWritten(BLEDevice central, BLECharacteristic ch) {
        if (!instance) {
            return;
        }
        noInterrupts();
        const uint8_t* data = ch.value();
        uint32_t len = ch.valueLength();
        BLYNK_DBG_DUMP(">> ", data, len);
        instance->mBuffRX.put(data, len);
        interrupts();
    }

    static
        void txCharSubscribed(BLEDevice central, BLECharacteristic ch);

    static
        void blePeripheralDisconnectHandler(BLEDevice central);

    static
        void blePeripheralConnectHandler(BLEDevice central);

private:
    bool mConn;
    const char* mName;
    BLEService pService{SERVICE_UUID};
    BLECharacteristic pCharacteristicRX = BLEStringCharacteristic{CHARACTERISTIC_UUID_RX, BLEWrite | BLEWriteWithoutResponse, 512};
    BLECharacteristic pCharacteristicTX = BLEStringCharacteristic{CHARACTERISTIC_UUID_TX, BLERead | BLENotify, 512};

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkArduinoNano_BLE
    : public BlynkProtocol<BlynkTransportArduinoNano_BLE>
{
    typedef BlynkProtocol<BlynkTransportArduinoNano_BLE> Base;
public:
    explicit BlynkArduinoNano_BLE(BlynkTransportArduinoNano_BLE& transp)
        : Base(transp)

    {}

    void begin(const char* auth, const char* name)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin(name);
    }

    void setDeviceName(const char* name) {
        conn.setDeviceName(name);
    }

};


BlynkTransportArduinoNano_BLE* BlynkTransportArduinoNano_BLE::instance = nullptr;
static BlynkTransportArduinoNano_BLE blynkTransportBLE;

BlynkArduinoNano_BLE Blynk(blynkTransportBLE);
inline
    void BlynkTransportArduinoNano_BLE::txCharSubscribed(BLEDevice central, BLECharacteristic ch) {
    Blynk.startSession();
}

inline
    void BlynkTransportArduinoNano_BLE::blePeripheralConnectHandler(BLEDevice central) {
}

inline
    void BlynkTransportArduinoNano_BLE::blePeripheralDisconnectHandler(BLEDevice central) {
    Blynk.disconnect();
}

#include <BlynkWidgets.h>

#endif
