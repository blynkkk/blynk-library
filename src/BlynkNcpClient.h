/**
 * @file       BlynkNcpClient.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2023 Volodymyr Shymanskyy
 * @date       Apr 2023
 * @brief
 *
 */

#ifndef BlynkNcpClient_h
#define BlynkNcpClient_h

#include <BlynkRpcClient.h>
#include <Blynk/BlynkApiNCP.h>
#include <Blynk/BlynkUtility.h>
#include <Blynk/BlynkTimer.h>
#include <utility/BlynkNcpOtaImpl.h>

#if !defined(BLYNK_NCP_BAUD)
  #define BLYNK_NCP_BAUD 2000000
#endif

#if defined(LINUX)
  #include <compat/LibSerialPort.h>

  #if defined(NCP_PORT)
    // OK, use it
  #elif defined(BLYNK_NCP_TTGO_T7_S3)
    #define NCP_PORT "/dev/ttyACM0"
  #else
    #define NCP_PORT "/dev/ttyUSB0"
  #endif
  SerialPort SerialUSB(NCP_PORT);
#endif

class BlynkNcpClient
    : public BlynkApi<BlynkNcpClient>
{
private:

#if defined(BLYNK_NCP_SERIAL)
  #define SerialNCP       BLYNK_NCP_SERIAL
  void ncpInitialize() {
    // NOTE: NCP hardware initialization needs to be done
    // before calling Blynk.begin()
  }
#elif defined(ARDUINO_NANO_RP2040_CONNECT) && defined(__MBED__)
  #define SerialNCP       SerialNina
  void ncpInitialize() {
    // Power-up NCP
    pinMode(NINA_RESETN, OUTPUT);
    digitalWrite(NINA_RESETN, HIGH);
  }
#elif defined(ARDUINO_NANO_RP2040_CONNECT) // Pico Core
  #define SerialNCP       Serial2
  void ncpInitialize() {
    // Power-up NCP
    pinMode(NINA_RESETN, OUTPUT);
    digitalWrite(NINA_RESETN, HIGH);
    SerialNCP.setFIFOSize(2048);
    SerialNCP.setTX(SERIAL3_TX);
    SerialNCP.setRX(SERIAL3_RX);
  }
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
  #define SerialNCP       SerialNina
  void ncpInitialize() {
    // Power-up NCP
    pinMode(NINA_RESETN, OUTPUT);
    digitalWrite(NINA_RESETN, HIGH);
  }
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
  #define SerialNCP       SerialNina
  void ncpInitialize() {
    // Power-up NCP
    pinMode(NINA_RESETN, OUTPUT);
    digitalWrite(NINA_RESETN, LOW);
  }
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  #define SerialNCP       Serial1
  void ncpInitialize() {
    SerialNCP.setFIFOSize(2048);
  }
#elif defined(LINUX)
  #define SerialNCP       SerialUSB
  void ncpInitialize() {
    BLYNK_LOG("NCP port: %s", NCP_PORT);
  }
#else
  #error "Your device is not supported by Blynk.NCP out of the box"
#endif


#if defined(ARDUINO_NANO_RP2040_CONNECT)
  void ncpConfigure() {
    rpc_hw_initRGB(27, 25, 26, true);
    rpc_hw_setLedBrightness(128);
  }
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
  void ncpConfigure() {
  }
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
  void ncpConfigure() {
    rpc_hw_initRGB(26, 25, 27, false);
    rpc_hw_setLedBrightness(128);
  }
#elif defined(BLYNK_NCP_WITTY_CLOUD)
  void ncpConfigure() {
    rpc_hw_initUserButton(4, true);
    rpc_hw_initRGB(15, 12, 13, false);
    rpc_hw_setLedBrightness(255);
  }
#elif defined(BLYNK_NCP_TTGO_T7)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(19, false);
    rpc_hw_setLedBrightness(160);
  }
#elif defined(BLYNK_NCP_TTGO_T7_S3)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(17, false);
    rpc_hw_setLedBrightness(160);
  }
#elif defined(BLYNK_NCP_TTGO_TOI_PLUS)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(3, false);
    rpc_hw_setLedBrightness(255);
  }
#elif defined(BLYNK_NCP_MACCHINA_SUPERB)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(26, false);
    rpc_hw_setLedBrightness(128);
  }
#else
  void ncpConfigure() {
  }
#endif

  bool ncpConnect(uint32_t targetBaud)
  {
    // Start with the default NCP baud rate, then try the secondary
    const uint32_t initialBauds[2] = { 115200, targetBaud };
    for (int i = 0; i < 2; i++) {
      const uint32_t currentBaud = initialBauds[i];
      SerialNCP.begin(currentBaud);
      uint32_t tbeg = micros();
      if (RPC_STATUS_OK == rpc_system_ping()) {
        uint32_t tend = micros();
        BLYNK_LOG("NCP responding (baud %u, %u us)", currentBaud, tend-tbeg);
        if (currentBaud == targetBaud) {
          return true;
        }
        // Upgrade baud rate
        if (rpc_hw_setUartBaudRate(targetBaud)) {
          SerialNCP.flush();
          SerialNCP.begin(targetBaud);
          delay(20);
          tbeg = micros();
          if (RPC_STATUS_OK == rpc_system_ping()) {
            tend = micros();
            BLYNK_LOG("NCP responding (baud %u, %u us)", targetBaud, tend-tbeg);
            return true;
          } else {
            BLYNK_LOG("NCP not responding (baud: %u)", targetBaud);
            // TODO: handle this case, i.e:
            // Power-cycle the module, keep communicating on the initial baud rate
          }
        }
      }
    }
    return false;
  }


public:
    typedef void (*Callback0)();

    BlynkNcpClient() {
    }

    void onStateChange(Callback0 cbk)         { _onStateChange = cbk;   }
    void onNcpRebooting(Callback0 cbk)        { _onNcpRebooting = cbk;  }
    void onProvisioned(Callback0 cbk)         { _onProvisioned = cbk;   }
    void onTimeSync(Callback0 cbk)            { _onTimeSync = cbk;      }
    void onTimeChanged(Callback0 cbk)         { _onTimeChanged = cbk;   }

    String getNcpVersion() {
        const char* ncpFwVer = "unknown";
        rpc_blynk_getNcpVersion(&ncpFwVer);
        return ncpFwVer;
    }

    String getWiFiMAC() {
        const char* mac = "";
        rpc_hw_getWiFiMAC(&mac);
        return mac;
    }

    String getEthernetMAC() {
        const char* mac = "";
        rpc_hw_getEthernetMAC(&mac);
        return mac;
    }

    bool setVendorPrefix(const char* vendor) {
        return rpc_blynk_setVendorPrefix(vendor);
    }

    bool setVendorServer(const char* host) {
        return rpc_blynk_setVendorServer(host);
    }

    bool setTime(int64_t time) {
        return rpc_blynk_setTime(time);
    }

    bool initNCP(uint32_t timeout = 2000) {
        ncpInitialize();

        const int retries = timeout / 100;
        for (int retry = 0; ; retry++) {
          if (ncpConnect(BLYNK_NCP_BAUD)) {
            return true;
          } else if (retry >= retries) {
            return false;
          } else {
            delay(100);
          }
        }
    }

    bool begin(const char* tmpl_id, const char* tmpl_name)
    {
        // This structure is used by Blynk.Cloud to identify the firmware during OTA
        volatile const char firmwareTag[] BLYNK_PROGMEM = "blnkinf\0"
            BLYNK_PARAM_KV("mcu"    , BLYNK_FIRMWARE_VERSION)
            BLYNK_PARAM_KV("fw-type", BLYNK_FIRMWARE_TYPE)
            BLYNK_PARAM_KV("build"  , __DATE__ " " __TIME__)
            BLYNK_PARAM_KV("blynk"  , BLYNK_VERSION)
            "\0";
        (void)firmwareTag;

        if (!String(tmpl_id).startsWith("TMPL") ||
            !strlen(tmpl_name)
        ) {
          BLYNK_LOG("Invalid configuration of TEMPLATE_ID / TEMPLATE_NAME");
          return false;
        }

        rpc_blynk_setFirmwareInfo(BLYNK_FIRMWARE_TYPE,
                                  BLYNK_FIRMWARE_VERSION,
                                  __DATE__ " " __TIME__,
                                  BLYNK_VERSION);

        ncpConfigure();

        bool result = rpc_blynk_initialize(tmpl_id, tmpl_name);
        if (!result) {
          BLYNK_LOG("rpc_blynk_initialize failed");
        }
        return result;
    }

    RpcBlynkState getState() const {
        return _state;
    }

    void run() {
        rpc_run();
        ota_apply_if_needed();
        if (_needReboot) {
          BlynkReset();
        }
    }

    const char* getStateString() const {
        switch (_state) {
        case BLYNK_STATE_IDLE             : return "Idle";
        case BLYNK_STATE_CONFIG           : return "Configuration";
        case BLYNK_STATE_CONNECTING_NET   : return "Connecting Network";
        case BLYNK_STATE_CONNECTING_CLOUD : return "Connecting Cloud";
        case BLYNK_STATE_CONNECTED        : return "Connected";

        case BLYNK_STATE_NOT_INITIALIZED  : return "Not Initialized";
        case BLYNK_STATE_OTA_UPGRADE      : return "NCP Upgrade";
        case BLYNK_STATE_ERROR            : return "Error";

        default                           : return "Unknown";
        }
    }

private:

    static void Callback0_dummy() {}

    Callback0       _onStateChange  = Callback0_dummy;
    Callback0       _onNcpRebooting = Callback0_dummy;
    Callback0       _onProvisioned  = Callback0_dummy;
    Callback0       _onTimeSync     = Callback0_dummy;
    Callback0       _onTimeChanged  = Callback0_dummy;

    RpcBlynkState   _state;
    bool            _needReboot = false;

    friend void rpc_client_blynkStateChange_impl(uint8_t state);
    friend void rpc_client_processEvent_impl(uint8_t event);
    friend bool rpc_client_adjustTime_impl(int64_t time, int16_t offset);
    friend void rpc_client_syncTime_impl();
    friend bool rpc_system_reboot_impl();
};

/*
 * Blynk Instance
 */

BlynkNcpClient Blynk;

/*
 * Blynk RPC services implementation
 */

#include <BlynkRpcInfraArduino.h>

bool rpc_system_reboot_impl() {
    if (BlynkResetImplemented()) {
        Blynk._needReboot = true;
        return true;
    }
    return false;
}

void rpc_client_blynkVPinChange_impl(uint16_t vpin, buffer_t param)
{
    // NOTE: we could copy the buffer, but we use 0-copy instead
    // But we need to 0-terminate it, overwriting the CRC8
    param.data[param.length] = '\0';

    // Call the relevant handler
    BlynkReq req = { vpin };
    BlynkParam values(param.data, param.length);
    Blynk.callWriteHandler(req, values);
}

void rpc_client_blynkStateChange_impl(uint8_t state) {
    const RpcBlynkState next = (RpcBlynkState)state;
    const RpcBlynkState prev = Blynk._state;
    if (next != prev)
    {
        Blynk._state = next;
        if (prev == BLYNK_STATE_CONNECTED) {
            BlynkOnDisconnected();
        }
        Blynk._onStateChange();
        if (next == BLYNK_STATE_CONNECTED) {
            BlynkOnConnected();
        }
    }
}

void rpc_client_processEvent_impl(uint8_t event) {
    switch ((RpcEvent)event) {
    case RPC_EVENT_NCP_REBOOTING:       Blynk._onNcpRebooting();    break; // TODO: reinit NCP
    case RPC_EVENT_BLYNK_PROVISIONED:   Blynk._onProvisioned();     break;
    case RPC_EVENT_BLYNK_TIME_SYNC:     Blynk._onTimeSync();        break;
    case RPC_EVENT_BLYNK_TIME_CHANGED:  Blynk._onTimeChanged();     break;
    default: break;
    }
}

#endif
