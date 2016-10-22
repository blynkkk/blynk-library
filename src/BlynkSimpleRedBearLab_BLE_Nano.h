/**
 * @file       BlynkSimpleRedBearLab_BLE_Nano.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleRedBearLab_BLE_Nano_h
#define BlynkSimpleRedBearLab_BLE_Nano_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "RBL_BLE_Nano"
#endif

#define BLYNK_NO_YIELD
#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo2.h>
#include <BLE_API.h>

/*
 * The Nordic UART Service
 */
static const uint8_t uart_base_uuid[]     = {0x71, 0x3D, 0, 0, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t uart_base_uuid_rev[] = {0x1E, 0x94, 0x8D, 0xF1, 0x48, 0x31, 0x94, 0xBA, 0x75, 0x4C, 0x3E, 0x50, 0, 0, 0x3D, 0x71};
static const uint8_t uart_tx_uuid[]       = {0x71, 0x3D, 0, 3, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t uart_rx_uuid[]       = {0x71, 0x3D, 0, 2, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t uart_dev_name[]      = "Blynk";

#define TXRX_BUF_LEN 20
uint8_t txPayload[TXRX_BUF_LEN] = {0,};
uint8_t rxPayload[TXRX_BUF_LEN] = {0,};

GattCharacteristic  txCharacteristic (uart_tx_uuid, txPayload, 1, TXRX_BUF_LEN,
                                      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
                                      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

GattCharacteristic  rxCharacteristic (uart_rx_uuid, rxPayload, 1, TXRX_BUF_LEN,
                                      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

GattCharacteristic *uartChars[] = {&txCharacteristic, &rxCharacteristic};

GattService         uartService(uart_base_uuid, uartChars, sizeof(uartChars) / sizeof(GattCharacteristic*));

BLE  ble;

class BlynkTransportRedBearLab_BLE_Nano
{
public:
    BlynkTransportRedBearLab_BLE_Nano()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char* h, uint16_t p) {}

    void begin() {
        instance = this;

        ble.gap().onConnection(connectCallback);
        ble.gap().onDisconnection(disconnectCallback);

        ble.gattServer().addService(uartService);
        ble.gattServer().onDataWritten(writeCallback);
        ble.gattServer().onDataSent(sentCallback);

        // Setup advertising
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                              uart_base_uuid_rev, sizeof(uart_base_uuid));

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
                ble.waitForEvent();
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
        ble.updateCharacteristicValue(rxCharacteristic.getValueAttribute().getHandle(), (uint8_t*)buf, len);
        return len;
    }

    size_t available() {
        noInterrupts();
        size_t rxSize = mBuffRX.size();
        interrupts();
        return rxSize;
    }

private:
    static BlynkTransportRedBearLab_BLE_Nano* instance;

    static
    void writeCallback(const GattWriteCallbackParams *params)
    {
        if (!instance)
            return;
      noInterrupts();
      //BLYNK_DBG_DUMP(">> ", params->data, params->len);
      instance->mBuffRX.put(params->data, params->len);
      interrupts();
    }

    static
    void sentCallback(unsigned count)
    {
      //Serial.print("SENT: ");
      //Serial.println(count);
    }

    static
    void connectCallback(const Gap::ConnectionCallbackParams_t *params);

    static
    void disconnectCallback(const Gap::DisconnectionCallbackParams_t *params);

private:
    bool mConn;

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkRedBearLab_BLE_Nano
    : public BlynkProtocol<BlynkTransportRedBearLab_BLE_Nano>
{
    typedef BlynkProtocol<BlynkTransportRedBearLab_BLE_Nano> Base;
public:
    BlynkRedBearLab_BLE_Nano(BlynkTransportRedBearLab_BLE_Nano& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin();
    }
};

BlynkTransportRedBearLab_BLE_Nano* BlynkTransportRedBearLab_BLE_Nano::instance = NULL;

static BlynkTransportRedBearLab_BLE_Nano _blynkTransport;
BlynkRedBearLab_BLE_Nano Blynk(_blynkTransport);

void BlynkTransportRedBearLab_BLE_Nano::connectCallback(const Gap::ConnectionCallbackParams_t *params)
{
  BLYNK_LOG1("Device connected");
  Blynk.startSession();
}

void BlynkTransportRedBearLab_BLE_Nano::disconnectCallback(const Gap::DisconnectionCallbackParams_t *params)
{
  BLYNK_LOG1("Device disconnected");
  //__disable_irq();
  Blynk.disconnect();
  //__enable_irq();
  ble.startAdvertising();
}

#include <BlynkWidgets.h>

#endif
