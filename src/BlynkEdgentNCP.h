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

#if defined(BLYNK_NCP_BAUD)
  // OK, use it
#elif defined(BLYNK_NCP_SERIAL)
  #define BLYNK_NCP_BAUD  115200
#elif defined(ARDUINO_UNOWIFIR4)
  #define BLYNK_NCP_BAUD  460800
#elif defined(SEEED_WIO_TERMINAL)
  #define BLYNK_NCP_BAUD  2000000

  #define PIN_BLE_SERIAL_X_RX (84ul)
  #define PIN_BLE_SERIAL_X_TX (85ul)
  #define PAD_BLE_SERIAL_X_RX (SERCOM_RX_PAD_2)
  #define PAD_BLE_SERIAL_X_TX (UART_TX_PAD_0)
  #define SERCOM_BLE_SERIAL_X sercom0

  Uart rtl_uart(&SERCOM_BLE_SERIAL_X, PIN_BLE_SERIAL_X_RX, PIN_BLE_SERIAL_X_TX, PAD_BLE_SERIAL_X_RX, PAD_BLE_SERIAL_X_TX);

  extern "C" {
      void SERCOM0_0_Handler()
      {
        rtl_uart.IrqHandler();
      }
      void SERCOM0_1_Handler()
      {
          rtl_uart.IrqHandler();
      }
      void SERCOM0_2_Handler()
      {
          rtl_uart.IrqHandler();
      }
      void SERCOM0_3_Handler()
      {
          rtl_uart.IrqHandler();
      }
  }

#else
  #define BLYNK_NCP_BAUD  2000000
#endif

#if defined(LINUX)
  #include <compat/LibSerialPort.h>

  #if defined(BLYNK_NCP_PORT)
    // OK, use it
  #elif defined(BLYNK_NCP_TYPE_TTGO_T7_S3)
    #define BLYNK_NCP_PORT "/dev/ttyACM0"
  #else
    #define BLYNK_NCP_PORT "/dev/ttyUSB0"
  #endif
  SerialPort SerialUSB(BLYNK_NCP_PORT);
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
#elif defined(ARDUINO_TTGO_T_PICO_C3)
  #define SerialNCP       Serial2
  void ncpInitialize() {
    SerialNCP.setFIFOSize(2048);
    SerialNCP.setTX(8);
    SerialNCP.setRX(9);
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
#elif defined(ARDUINO_PORTENTA_C33)
  #define SerialNCP       Serial5
  #define NCP_RESETN      (101)
  void ncpInitialize() {
    // Power-up NCP
    pinMode(NCP_RESETN, OUTPUT);
    digitalWrite(NCP_RESETN, HIGH);
  }
#elif defined(ARDUINO_UNOWIFIR4)
  #define SerialNCP       Serial2
  void ncpInitialize() {
    // ESP32-S3 is always enabled
  }
#elif defined(ARDUINO_RASPBERRY_PI_PICO) && defined(__MBED__)
  #define SerialNCP       Serial1
  void ncpInitialize() {
  }
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  #define SerialNCP       Serial1
  void ncpInitialize() {
    SerialNCP.setFIFOSize(2048);
  }
#elif defined(SEEED_WIO_TERMINAL)
  #define SerialNCP       rtl_uart //RTL8720D
  void ncpInitialize() {
    // Power-up NCP
    pinMode(RTL8720D_CHIP_PU, OUTPUT);
    //digitalWrite(RTL8720D_CHIP_PU, LOW);
    //delay(100);
    digitalWrite(RTL8720D_CHIP_PU, HIGH);
  }
#elif defined(LINUX)
  #define SerialNCP       SerialUSB
  void ncpInitialize() {
    BLYNK_LOG("NCP port: %s", BLYNK_NCP_PORT);
  }
#else
  #error "Your board needs extra configuration to work with Blynk.NCP: please define BLYNK_NCP_SERIAL and perform NCP hardware initialization if needed"
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
#elif defined(BLYNK_NCP_TYPE_WITTY_CLOUD)
  void ncpConfigure() {
    rpc_hw_initUserButton(4, true);
    rpc_hw_initRGB(15, 12, 13, false);
    rpc_hw_setLedBrightness(255);
  }
#elif defined(BLYNK_NCP_TYPE_TTGO_T7)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(19, false);
    rpc_hw_setLedBrightness(160);
  }
#elif defined(BLYNK_NCP_TYPE_TTGO_T7_S3)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(17, false);
    rpc_hw_setLedBrightness(160);
  }
#elif defined(BLYNK_NCP_TYPE_TTGO_TOI_PLUS)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(3, false);
    rpc_hw_setLedBrightness(255);
  }
#elif defined(BLYNK_NCP_TYPE_MACCHINA_SUPERB)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
    rpc_hw_initLED(26, false);
    rpc_hw_setLedBrightness(128);
  }
#elif defined(BLYNK_NCP_TYPE_MICRODUINO_ESP8266)
  void ncpConfigure() {
    rpc_hw_initUserButton(0, true);
  }
#else
  void ncpConfigure() {
  }
#endif

  bool ncpConnect(long targetBaud)
  {
    // Start with the default NCP baud rate, then try the secondary
    const long initialBauds[3] = { 38400, 115200, targetBaud };
    for (int i = 0; i < 3; i++) {
      const long currentBaud = initialBauds[i];
      SerialNCP.begin(currentBaud);
      uint32_t tbeg = micros();
      if (RPC_STATUS_OK == rpc_ncp_ping()) {
        uint32_t tend = micros();
        BLYNK_LOG("NCP responding (baud %ld, %u us)", currentBaud, tend-tbeg);
        if (currentBaud == targetBaud) {
          _pingFails = 0;
          return true;
        }
        // Upgrade baud rate
        if (rpc_hw_setUartBaudRate(targetBaud)) {
          SerialNCP.flush();
          SerialNCP.begin(targetBaud);
          delay(20);
          tbeg = micros();
          if (RPC_STATUS_OK == rpc_ncp_ping()) {
            tend = micros();
            BLYNK_LOG("NCP responding (baud %ld, %u us)", targetBaud, tend-tbeg);
            _pingFails = 0;
            return true;
          } else {
            BLYNK_LOG("NCP not responding (baud: %ld)", targetBaud);
            // TODO: handle this case, i.e:
            // Power-cycle the module, keep communicating on the initial baud rate
          }
        }
      }
    }
    return false;
  }

  bool ncpSetup()
  {
    rpc_blynk_setFirmwareInfo(BLYNK_FIRMWARE_TYPE,
                              BLYNK_FIRMWARE_VERSION,
                              __DATE__ " " __TIME__,
                              BLYNK_VERSION);

    ncpConfigure();

    if (_vendor.length()) {
      rpc_blynk_setVendorPrefix(_vendor.c_str());
    }
    if (_server.length()) {
      rpc_blynk_setVendorServer(_server.c_str());
    }

    return rpc_blynk_initialize(_tmplID.c_str(), _tmplName.c_str());
  }

  bool ncpReinit() {
    if (!(_tmplID.length() && _tmplName.length())) {
      return false;
    }
    BLYNK_LOG("Reinitializing NCP...");
    const millis_time_t now = BlynkMillis();
    while (BlynkMillis() - now < 10000) {
      if (ncpConnect(BLYNK_NCP_BAUD)) {
        return ncpSetup();
      }
      delay(100);
    }
    return false;
  }

public:
    typedef void (*Callback0)();
    typedef void (*CallbackEvent)(RpcEvent e);

    BlynkNcpClient() {
    }

    void onStateChange(Callback0 cbk)         { _onStateChange = cbk;   }
    void onNcpRebooting(Callback0 cbk)        { _onNcpRebooting = cbk;  }
    void onProvisioned(Callback0 cbk)         { _onProvisioned = cbk;   }
    void onTimeSync(Callback0 cbk)            { _onTimeSync = cbk;      }
    void onTimeChanged(Callback0 cbk)         { _onTimeChanged = cbk;   }
    void onHwEvent(CallbackEvent cbk)         { _onHwEvent = cbk;       }

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

    void setVendorPrefix(const char* vendor) {
        _vendor = vendor;
    }

    void setVendorServer(const char* host) {
        _server = host;
    }

    bool setConfigTimeout(uint16_t seconds) { // 60..3600 (1 hour)
        return rpc_blynk_setConfigTimeout(seconds);
    }

    bool setConfigSkipLimit(uint16_t limit) { // 0, 5..50
        return rpc_blynk_setConfigSkipLimit(limit);
    }

    bool resetConfig() {
        return rpc_blynk_configReset();
    }

    bool setTime(int64_t time) {
        return rpc_blynk_setTime(time);
    }

    bool initNCP(uint32_t timeout = 10000) {
        printBanner();

        ncpInitialize();

        const millis_time_t tstart = BlynkMillis();
        while (BlynkMillis() - tstart < timeout) {
          if (ncpConnect(BLYNK_NCP_BAUD)) {
            return true;
          }
          delay(100);
        }
        return false;
    }

    // This provides a simple way to verify the NCP boot log, i.e:
    // [rpc port] Blynk.NCP started
    void passthrough(long baud) {
        if (baud > 0) {
          SerialNCP.begin(baud);
        }
        while (true) {
          while (SerialNCP.available()) {
            BLYNK_PRINT.write(SerialNCP.read());
          }
          delay(1);
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

        _tmplID   = tmpl_id;
        _tmplName = tmpl_name;

        if (_tmplID.startsWith("TMPL") && _tmplName.length()) {
          return ncpSetup();
        } else {
          BLYNK_LOG("Invalid configuration of TEMPLATE_ID / TEMPLATE_NAME");
          _tmplID = _tmplName = "";
          return false;
        }
    }

    bool connected() const {
        return _state == BLYNK_STATE_CONNECTED;
    }

    bool connect() {
        // TODO
        return connected();
    }

    void disconnect() {
        // TODO
    }

    RpcBlynkState getState() const {
        return _state;
    }

    void run() {
        rpc_run();
        ota_run();
        if (_needReboot) {
          BlynkReset();
        } else if (_needReinit) {
          _needReinit = false;
          ncpReinit();
        } else if (rpc_get_last_rx() > 30000) {
          //BLYNK_LOG("NCP ping");
          const uint8_t ncpState = rpc_blynk_getState();
          if (RPC_STATUS_OK == rpc_get_status()) {
            _pingFails = 0;
            if (BLYNK_STATE_NOT_INITIALIZED == ncpState) {
              ncpReinit();
            }
          } else if (++_pingFails >= 10) {
            ncpReinit();
          }
        }
    }

    const char* getStateString() const {
        return getStateString(_state);
    }

    const char* getStateString(RpcBlynkState state) const {
        switch (state) {
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

    void printBanner() {
#if defined(BLYNK_NO_FANCY_LOGO)
        BLYNK_LOG1(BLYNK_F("Blynk v" BLYNK_VERSION " on " BLYNK_INFO_DEVICE
            BLYNK_NEWLINE
            " #StandWithUkraine    https://bit.ly/swua" BLYNK_NEWLINE
            BLYNK_NEWLINE
        ));
#else
        BLYNK_LOG1(BLYNK_F(BLYNK_NEWLINE
            "    ___  __          __" BLYNK_NEWLINE
            "   / _ )/ /_ _____  / /__" BLYNK_NEWLINE
            "  / _  / / // / _ \\/  '_/" BLYNK_NEWLINE
            " /____/_/\\_, /_//_/_/\\_\\" BLYNK_NEWLINE
            "        /___/ v" BLYNK_VERSION " on " BLYNK_INFO_DEVICE BLYNK_NEWLINE
            BLYNK_NEWLINE
            " #StandWithUkraine    https://bit.ly/swua" BLYNK_NEWLINE
            BLYNK_NEWLINE
        ));
#endif
    }

private:

    static void Callback0_dummy() {}
    static void CallbackEv_dummy(RpcEvent) {}

    Callback0       _onStateChange  = Callback0_dummy;
    Callback0       _onNcpRebooting = Callback0_dummy;
    Callback0       _onProvisioned  = Callback0_dummy;
    Callback0       _onTimeSync     = Callback0_dummy;
    Callback0       _onTimeChanged  = Callback0_dummy;
    CallbackEvent   _onHwEvent      = CallbackEv_dummy;

    String          _vendor;
    String          _server;
    String          _tmplID;
    String          _tmplName;
    RpcBlynkState   _state;
    bool            _needReboot = false;
    bool            _needReinit = false;
    uint8_t         _pingFails = 0;

    friend void rpc_client_blynkStateChange_impl(uint8_t state);
    friend void rpc_client_processEvent_impl(uint8_t event);
    friend bool rpc_client_adjustTime_impl(int64_t time, int16_t offset);
    friend void rpc_client_syncTime_impl();
    friend bool rpc_mcu_reboot_impl();
};

/*
 * Blynk Instance
 */

BlynkNcpClient Blynk;

/*
 * Blynk RPC services implementation
 */

int rpc_uart_available() {
  // Return the amount of data in the UART input buffer
  return SerialNCP.available();
}
int rpc_uart_read() {
  // Get a byte from the UART input buffer
  return SerialNCP.read();
}
size_t rpc_uart_write(uint8_t data) {
  // Put a byte to the UART output buffer
  return SerialNCP.write(data);
}
void rpc_uart_flush() {
  // Wait for UART to output all the data
  SerialNCP.flush();
}
uint32_t rpc_system_millis() {
  // Return uptime in milliseconds
  // This is used to implement the RPC timeout
  return BlynkMillis();
}

bool rpc_mcu_reboot_impl() {
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
    const RpcEvent e = (RpcEvent)event;
    switch (e) {
    case RPC_EVENT_NCP_REBOOTING:
      Blynk._needReinit = true;
      Blynk._onNcpRebooting();
      break;
    case RPC_EVENT_BLYNK_PROVISIONED:   Blynk._onProvisioned();     break;
    case RPC_EVENT_BLYNK_TIME_SYNC:     Blynk._onTimeSync();        break;
    case RPC_EVENT_BLYNK_TIME_CHANGED:  Blynk._onTimeChanged();     break;

    case RPC_EVENT_HW_USER_CLICK:
    case RPC_EVENT_HW_USER_DBLCLICK:
    case RPC_EVENT_HW_USER_LONGPRESS:
    case RPC_EVENT_HW_USER_LONGRELEASE:
    case RPC_EVENT_HW_USER_RESET_START:
    case RPC_EVENT_HW_USER_RESET_CANCEL:
    case RPC_EVENT_HW_USER_RESET_DONE:
      Blynk._onHwEvent(e);
      break;
    }
}

#include <BlynkWidgets.h>

#endif
