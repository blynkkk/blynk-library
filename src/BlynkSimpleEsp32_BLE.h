/**
 * @file       BlynkSimpleEsp32_BLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Nov 2017
 * @brief
 *
 */

#ifndef BlynkSimpleEsp32_BLE_h
#define BlynkSimpleEsp32_BLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Esp32_BLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID           "713D0000-503E-4C75-BA94-3148F18D941E"
#define CHARACTERISTIC_UUID_RX "713D0003-503E-4C75-BA94-3148F18D941E"
#define CHARACTERISTIC_UUID_TX "713D0002-503E-4C75-BA94-3148F18D941E"

class BlynkTransportEsp32_BLE :
    public BLEServerCallbacks,
    public BLECharacteristicCallbacks
{

public:
    BlynkTransportEsp32_BLE()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char BLYNK_UNUSED *h, uint16_t BLYNK_UNUSED p) {}

    void begin() {
        // Create the BLE Device
        BLEDevice::init("Blynk");

        // Create the BLE Server
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(this);

        // Create the BLE Service
        pService = pServer->createService(SERVICE_UUID);

        // Create a BLE Characteristic
        pCharacteristicTX = pService->createCharacteristic(
                            CHARACTERISTIC_UUID_TX,
                            BLECharacteristic::PROPERTY_NOTIFY
                          );

        pCharacteristicTX->addDescriptor(new BLE2902());

        pCharacteristicRX = pService->createCharacteristic(
                                              CHARACTERISTIC_UUID_RX,
                                              BLECharacteristic::PROPERTY_WRITE
                                            );

        pCharacteristicRX->setCallbacks(this);

        // Start the service
        pService->start();

        // Start advertising
        pServer->getAdvertising()->start();


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
                delay(1);
            } else {
                break;
            }
        }
        size_t res = mBuffRX.get((uint8_t*)buf, len);
        return res;
    }

    size_t write(const void* buf, size_t len) {
        pCharacteristicTX->setValue((uint8_t*)buf, len);
        pCharacteristicTX->notify();
        return len;
    }

    size_t available() {
        size_t rxSize = mBuffRX.size();
        return rxSize;
    }

private:

    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        uint8_t* data = (uint8_t*)rxValue.data();
        size_t len = rxValue.length();

        BLYNK_DBG_DUMP(">> ", data, len);
        mBuffRX.put(data, len);
      }
    }

private:
    bool mConn;

    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristicTX;
    BLECharacteristic *pCharacteristicRX;

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkEsp32_BLE
    : public BlynkProtocol<BlynkTransportEsp32_BLE>
{
    typedef BlynkProtocol<BlynkTransportEsp32_BLE> Base;
public:
    BlynkEsp32_BLE(BlynkTransportEsp32_BLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin();
    }
};


static BlynkTransportEsp32_BLE _blynkTransportBLE;
BlynkEsp32_BLE Blynk(_blynkTransportBLE);

inline
void BlynkTransportEsp32_BLE::onConnect(BLEServer* pServer) {
  BLYNK_LOG1(BLYNK_F("BLE connect"));
  connect();
  Blynk.startSession();
};

inline
void BlynkTransportEsp32_BLE::onDisconnect(BLEServer* pServer) {
  BLYNK_LOG1(BLYNK_F("BLE disconnect"));
  Blynk.disconnect();
  disconnect();
}


#include <BlynkWidgets.h>

#endif
