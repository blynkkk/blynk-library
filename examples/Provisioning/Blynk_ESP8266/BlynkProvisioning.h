
enum State {
  MODE_WAIT_CONFIG,
  MODE_BUTTON_HOLD,
  MODE_BUTTON_DONE,
  MODE_CONFIG_DEVICE_CONNECTED,
  MODE_CONFIGURING,
  MODE_CONNECTING_NET,
  MODE_CONNECTING_CLOUD,
  MODE_RUNNING,
  MODE_OTA_UPGRADE,
  MODE_ERROR,

  MODE_MAX_VALUE
} g_State;

#include "Settings.h"
#include "ConfigStore.h"
#include "ConfigMode.h"
#include "ResetButton.h"
#include "Indicator.h"

class Provisioning {

public:
  void begin()
  {
    // Delay as Microsoft Serial Ballpoint driver workaround
    delay(500);
    Serial.begin(BOARD_SERIAL_TERMINAL_BAUD);
    DEBUG_PRINT("");
    DEBUG_PRINT("Hardware v" + String(BOARD_HARDWARE_VERSION));
    DEBUG_PRINT("Firmware v" + String(BOARD_FIRMWARE_VERSION));
    
    randomSeed(ESP.getChipId());
    
    indicator_init();
    button_init();
    config_init();
    
    if (digitalRead(BOARD_BUTTON_PIN) == LOW) {
      DEBUG_PRINT("Button pressed -> resetting config...");
      config_reset();
    }
  
    if (configStore.flagConfig) {
      DEBUG_PRINT(String("WiFi SSID: ")  + configStore.wifiSSID   + " Pass: " + configStore.wifiPass);
      DEBUG_PRINT(String("Cloud Auth: ") + configStore.cloudToken + " Host: " + configStore.cloudHost + " Port: " + configStore.cloudPort);

      Blynk.begin(configStore.cloudToken, configStore.wifiSSID, configStore.wifiPass, configStore.cloudHost, configStore.cloudPort);
    } else {
      enterConfigMode();
    }
  }

  void setState(State m) {
    if (g_State != m) {
      g_State = m;
      indicator.updateState();
    }
  }
  
};

Provisioning BlynkProvisioning;
