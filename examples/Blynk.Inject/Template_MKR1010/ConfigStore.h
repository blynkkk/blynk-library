/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#define CONFIG_FLAG_VALID      0x01
#define CONFIG_FLAG_STATIC_IP  0x02

struct ConfigStore {
  uint32_t  magic;
  char      version[15];
  uint8_t   flags;

  char      wifiSSID[34];
  char      wifiPass[64];

  char      cloudToken[34];
  char      cloudHost[34];
  uint16_t  cloudPort;

  uint16_t  checksum;

  void setFlag(uint8_t mask, bool value) {
    if (value) {
      flags |= mask;
    } else {
      flags &= ~mask;
    }
  }

  bool getFlag(uint8_t mask) {
    return (flags & mask) == mask;
  }
} __attribute__((packed));

ConfigStore configStore;

const ConfigStore configDefault = {
  0x626C6E6B,
  BOARD_FIRMWARE_VERSION,
  0x00,
  
  "",
  "",
  
  "invalid token",
  BOARD_DEFAULT_SERVER,
  BOARD_DEFAULT_PORT,
  0
};

template<typename T, int size>
void CopyString(const String& s, T(&arr)[size]) {
  s.toCharArray(arr, size);
}

static bool config_load_blnkopt()
{
  static const char blnkopt[] = "blnkopt\0"
    BLYNK_PARAM_KV("ssid" , BLYNK_PARAM_PLACEHOLDER_64
                            BLYNK_PARAM_PLACEHOLDER_64
                            BLYNK_PARAM_PLACEHOLDER_64
                            BLYNK_PARAM_PLACEHOLDER_64)
    BLYNK_PARAM_KV("host" , BOARD_DEFAULT_SERVER)
    BLYNK_PARAM_KV("port" , BLYNK_TOSTRING(BOARD_DEFAULT_PORT))
    "\0";

  BlynkParam prov(blnkopt+8, sizeof(blnkopt)-8-2);
  BlynkParam::iterator ssid = prov["ssid"];
  BlynkParam::iterator pass = prov["pass"];
  BlynkParam::iterator auth = prov["auth"];
  BlynkParam::iterator host = prov["host"];
  BlynkParam::iterator port = prov["port"];

  if (!(ssid.isValid() && auth.isValid())) {
    return false;
  }

  // reset to defaut before loading values from blnkopt
  configStore = configDefault;

  if (ssid.isValid()) { CopyString(ssid.asStr(), configStore.wifiSSID); }
  if (pass.isValid()) { CopyString(pass.asStr(), configStore.wifiPass); }
  if (auth.isValid()) { CopyString(auth.asStr(), configStore.cloudToken); }
  if (host.isValid()) { CopyString(host.asStr(), configStore.cloudHost); }
  if (port.isValid()) { configStore.cloudPort = port.asInt(); }

  return true;
}

#include <FlashStorage.h>
FlashStorage(flash, ConfigStore);

void config_load()
{
  memset(&configStore, 0, sizeof(configStore));
  configStore = flash.read();
  if (configStore.magic != configDefault.magic) {
    DEBUG_PRINT("Using default config.");
    configStore = configDefault;
    return;
  }
}

bool config_save()
{
  flash.write(configStore);
  DEBUG_PRINT("Configuration stored to flash");
  return true;
}

bool config_init()
{
  config_load();
  return true;
}

void enterResetConfig()
{
  DEBUG_PRINT("Resetting configuration!");
  configStore = configDefault;
  config_save();
  BlynkState::set(MODE_WAIT_CONFIG);
}

