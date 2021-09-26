/**
 * @file       BlynkSimpleRedBear_Duo_BLE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkSimpleRedBear_Duo_BLE_h
#define BlynkSimpleRedBear_Duo_BLE_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "RB_Duo_BLE"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 20
//#define BLYNK_SEND_THROTTLE 20

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <utility/BlynkFifo.h>


/*
 * BLE peripheral preferred connection parameters:
 *     - Minimum connection interval = MIN_CONN_INTERVAL * 1.25 ms, where MIN_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - Maximum connection interval = MAX_CONN_INTERVAL * 1.25 ms,  where MAX_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - The SLAVE_LATENCY ranges from 0x0000 to 0x03E8
 *     - Connection supervision timeout = CONN_SUPERVISION_TIMEOUT * 10 ms, where CONN_SUPERVISION_TIMEOUT ranges from 0x000A to 0x0C80
 */
#define MIN_CONN_INTERVAL          0x0028 // 50ms.
#define MAX_CONN_INTERVAL          0x0190 // 500ms.
#define SLAVE_LATENCY              0x0000 // No slave latency.
#define CONN_SUPERVISION_TIMEOUT   0x03E8 // 10s.

// Learn about appearance: http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
#define BLE_PERIPHERAL_APPEARANCE  BLE_APPEARANCE_UNKNOWN

#define BLE_DEVICE_NAME            "Blynk"

#define CHARACTERISTIC1_MAX_LEN    20
#define CHARACTERISTIC2_MAX_LEN    20

/******************************************************
 *               Variable Definitions
 ******************************************************/
static uint8_t service1_uuid[16]    = { 0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };
static uint8_t service1_tx_uuid[16] = { 0x71,0x3d,0x00,0x03,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };
static uint8_t service1_rx_uuid[16] = { 0x71,0x3d,0x00,0x02,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };

// GAP and GATT characteristics value
static uint8_t  appearance[2] = {
  LOW_BYTE(BLE_PERIPHERAL_APPEARANCE),
  HIGH_BYTE(BLE_PERIPHERAL_APPEARANCE)
};

static uint8_t  change[4] = {
  0x00, 0x00, 0xFF, 0xFF
};

static uint8_t  conn_param[8] = {
  LOW_BYTE(MIN_CONN_INTERVAL), HIGH_BYTE(MIN_CONN_INTERVAL),
  LOW_BYTE(MAX_CONN_INTERVAL), HIGH_BYTE(MAX_CONN_INTERVAL),
  LOW_BYTE(SLAVE_LATENCY), HIGH_BYTE(SLAVE_LATENCY),
  LOW_BYTE(CONN_SUPERVISION_TIMEOUT), HIGH_BYTE(CONN_SUPERVISION_TIMEOUT)
};

/*
 * BLE peripheral advertising parameters:
 *     - advertising_interval_min: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_interval_max: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_type:
 *           BLE_GAP_ADV_TYPE_ADV_IND
 *           BLE_GAP_ADV_TYPE_ADV_DIRECT_IND
 *           BLE_GAP_ADV_TYPE_ADV_SCAN_IND
 *           BLE_GAP_ADV_TYPE_ADV_NONCONN_IND
 *     - own_address_type:
 *           BLE_GAP_ADDR_TYPE_PUBLIC
 *           BLE_GAP_ADDR_TYPE_RANDOM
 *     - advertising_channel_map:
 *           BLE_GAP_ADV_CHANNEL_MAP_37
 *           BLE_GAP_ADV_CHANNEL_MAP_38
 *           BLE_GAP_ADV_CHANNEL_MAP_39
 *           BLE_GAP_ADV_CHANNEL_MAP_ALL
 *     - filter policies:
 *           BLE_GAP_ADV_FP_ANY
 *           BLE_GAP_ADV_FP_FILTER_SCANREQ
 *           BLE_GAP_ADV_FP_FILTER_CONNREQ
 *           BLE_GAP_ADV_FP_FILTER_BOTH
 *
 * Note:  If the advertising_type is set to BLE_GAP_ADV_TYPE_ADV_SCAN_IND or BLE_GAP_ADV_TYPE_ADV_NONCONN_IND,
 *        the advertising_interval_min and advertising_interval_max should not be set to less than 0x00A0.
 */
static advParams_t adv_params = {
  .adv_int_min   = 0x0030,
  .adv_int_max   = 0x0030,
  .adv_type      = BLE_GAP_ADV_TYPE_ADV_IND,
  .dir_addr_type = BLE_GAP_ADDR_TYPE_PUBLIC,
  .dir_addr      = {0,0,0,0,0,0},
  .channel_map   = BLE_GAP_ADV_CHANNEL_MAP_ALL,
  .filter_policy = BLE_GAP_ADV_FP_ANY
};

static uint8_t adv_data[] = {
  0x02,
  BLE_GAP_AD_TYPE_FLAGS,
  BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE,

  0x08,
  BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME,
  'B','i','s','c','u','i','t',

  0x11,
  BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE,
  0x1e,0x94,0x8d,0xf1,0x48,0x31,0x94,0xba,0x75,0x4c,0x3e,0x50,0x00,0x00,0x3d,0x71
};

static uint16_t character1_handle = 0x0000;
static uint16_t character2_handle = 0x0000;
static uint16_t character3_handle = 0x0000;

static uint8_t characteristic1_data[CHARACTERISTIC1_MAX_LEN] = { 0x01 };
static uint8_t characteristic2_data[CHARACTERISTIC2_MAX_LEN] = { 0x00 };


class BlynkTransportRedBearDuoBLE
{
public:
    BlynkTransportRedBearDuoBLE()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char BLYNK_UNUSED *h, uint16_t BLYNK_UNUSED p) {}

    void begin() {
        instance = this;


        //ble.debugLogger(true);
        // Initialize ble_stack.
        ble.init();

        // Register BLE callback functions
        ble.onConnectedCallback(deviceConnectedCallback);
        ble.onDisconnectedCallback(deviceDisconnectedCallback);
        ble.onDataWriteCallback(gattWriteCallback);

        // Add GAP service and characteristics
        ble.addService(BLE_UUID_GAP);
        ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME, ATT_PROPERTY_READ|ATT_PROPERTY_WRITE, (uint8_t*)BLE_DEVICE_NAME, sizeof(BLE_DEVICE_NAME));
        ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_APPEARANCE, ATT_PROPERTY_READ, appearance, sizeof(appearance));
        ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_PPCP, ATT_PROPERTY_READ, conn_param, sizeof(conn_param));

        // Add GATT service and characteristics
        ble.addService(BLE_UUID_GATT);
        ble.addCharacteristic(BLE_UUID_GATT_CHARACTERISTIC_SERVICE_CHANGED, ATT_PROPERTY_INDICATE, change, sizeof(change));

        // Add user defined service and characteristics
        ble.addService(service1_uuid);
        character1_handle = ble.addCharacteristicDynamic(service1_tx_uuid, ATT_PROPERTY_WRITE|ATT_PROPERTY_WRITE_WITHOUT_RESPONSE, characteristic1_data, CHARACTERISTIC1_MAX_LEN);
        character2_handle = ble.addCharacteristicDynamic(service1_rx_uuid, ATT_PROPERTY_NOTIFY, characteristic2_data, CHARACTERISTIC2_MAX_LEN);

        // Set BLE advertising parameters
        ble.setAdvertisementParams(&adv_params);

        // // Set BLE advertising data
        ble.setAdvertisementData(sizeof(adv_data), adv_data);

        // BLE peripheral starts advertising now.
        ble.startAdvertising();

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
                //blePeripheral->poll();
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
        ble.sendNotify(character2_handle, (uint8_t*)buf, len);
        return len;
    }

    size_t available() {
        noInterrupts();
        size_t rxSize = mBuffRX.size();
        interrupts();
        return rxSize;
    }

private:
    static BlynkTransportRedBearDuoBLE* instance;

    static
    int gattWriteCallback(uint16_t value_handle, uint8_t* data, uint16_t len) {
        if (!instance)
            return 0;
        noInterrupts();
        //BLYNK_DBG_DUMP(">> ", data, len);
        instance->mBuffRX.put(data, len);
        interrupts();
        return 0;
    }

    static
    void deviceConnectedCallback(BLEStatus_t status, uint16_t handle);

    static
    void deviceDisconnectedCallback(uint16_t handle);

private:
    bool mConn;

    BlynkFifo<uint8_t, BLYNK_MAX_READBYTES*2> mBuffRX;
};

class BlynkRedBearDuoBLE
    : public BlynkProtocol<BlynkTransportRedBearDuoBLE>
{
    typedef BlynkProtocol<BlynkTransportRedBearDuoBLE> Base;
public:
    BlynkRedBearDuoBLE(BlynkTransportRedBearDuoBLE& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
        state = DISCONNECTED;
        conn.begin();
    }
};

BlynkTransportRedBearDuoBLE* BlynkTransportRedBearDuoBLE::instance = NULL;

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkTransportRedBearDuoBLE _blynkTransport;
  BlynkRedBearDuoBLE Blynk(_blynkTransport);
#else
  extern BlynkRedBearDuoBLE Blynk;
#endif

void BlynkTransportRedBearDuoBLE::deviceConnectedCallback(BLEStatus_t status, uint16_t handle) {
  switch (status) {
  case BLE_STATUS_OK:
    BLYNK_LOG1("Device connected");
    Blynk.startSession();
    break;
  default:
    break;
  }
}

void BlynkTransportRedBearDuoBLE::deviceDisconnectedCallback(uint16_t handle){
    BLYNK_LOG1("Device disconnected");
    Blynk.disconnect();
}

#include <BlynkWidgets.h>

#endif
