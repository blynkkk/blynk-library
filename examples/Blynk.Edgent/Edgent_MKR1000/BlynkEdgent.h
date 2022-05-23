
extern "C" {
  void app_loop();
  void restartMCU();
}

#include "Settings.h"
#include <SPI.h>
#include <WiFi101.h>

#define BLYNK_SEND_ATOMIC
//#define BLYNK_USE_SSL

#include <Blynk.h>
#include <Adapters/BlynkWiFiCommon.h>

WiFiSSLClient _blynkWifiClient;
//WiFiClient _blynkWifiClient;

BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#ifndef BLYNK_NEW_LIBRARY
#error "Old version of Blynk library is in use. Please replace it with the new one."
#endif

#if !defined(BLYNK_TEMPLATE_ID) || !defined(BLYNK_DEVICE_NAME)
#error "Please specify your BLYNK_TEMPLATE_ID and BLYNK_DEVICE_NAME"
#endif

BlynkTimer edgentTimer;

#include "BlynkState.h"
#include "ConfigStore.h"
#include "ResetButton.h"
#include "ConfigMode.h"
#include "Indicator.h"
#include "OTA.h"
#include "Console.h"


inline
void BlynkState::set(State m) {
  if (state != m && m < MODE_MAX_VALUE) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;

    // You can put your state handling here,
    // i.e. implement custom indication
  }
}

void printDeviceBanner()
{
  Blynk.printBanner();
  DEBUG_PRINT("--------------------------");
  DEBUG_PRINT(String("Product:  ") + BLYNK_DEVICE_NAME);
  DEBUG_PRINT(String("Firmware: ") + BLYNK_FIRMWARE_VERSION " (build " __DATE__ " " __TIME__ ")");
  if (configStore.getFlag(CONFIG_FLAG_VALID)) {
    DEBUG_PRINT(String("Token:    ") +
                String(configStore.cloudToken).substring(0,4) +
                " - •••• - •••• - ••••");
  }
  DEBUG_PRINT(String("Device:   ") + BLYNK_INFO_DEVICE);
  DEBUG_PRINT(String("WiFi FW:  ") + WiFi.firmwareVersion());
  DEBUG_PRINT("--------------------------");
}

void runBlynkWithChecks() {
  Blynk.run();
  if (BlynkState::get() == MODE_RUNNING) {
    if (!Blynk.connected()) {
      if (WiFi.status() == WL_CONNECTED) {
        BlynkState::set(MODE_CONNECTING_CLOUD);
      } else {
        BlynkState::set(MODE_CONNECTING_NET);
      }
    }
  }
}

class Edgent {

public:
  void begin()
  {
    // Required to get MAC
    WiFi.begin();

    indicator_init();
    button_init();
    config_init();
    printDeviceBanner();
    console_init();

    if (configStore.getFlag(CONFIG_FLAG_VALID)) {
      BlynkState::set(MODE_CONNECTING_NET);
    } else if (config_load_blnkopt()) {
      DEBUG_PRINT("Firmware is preprovisioned");
      BlynkState::set(MODE_CONNECTING_NET);
    } else {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  void run() {
    app_loop();
    switch (BlynkState::get()) {
    case MODE_WAIT_CONFIG:       
    case MODE_CONFIGURING:       enterConfigMode();    break;
    case MODE_CONNECTING_NET:    enterConnectNet();    break;
    case MODE_CONNECTING_CLOUD:  enterConnectCloud();  break;
    case MODE_RUNNING:           runBlynkWithChecks(); break;
    case MODE_OTA_UPGRADE:       enterOTA();           break;
    case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
    case MODE_RESET_CONFIG:      enterResetConfig();   break;
    default:                     enterError();         break;
    }
  }

} BlynkEdgent;

void app_loop() {
    edgentTimer.run();
    edgentConsole.run();
}

