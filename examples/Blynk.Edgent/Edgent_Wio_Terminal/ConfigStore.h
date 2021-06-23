
#define CONFIG_FLAG_VALID       0x01
#define CONFIG_FLAG_STATIC_IP   0x02

#define BLYNK_PROV_ERR_NONE     0      // All good
#define BLYNK_PROV_ERR_CONFIG   700    // Invalid config from app (malformed token,etc)
#define BLYNK_PROV_ERR_NETWORK  701    // Could not connect to the router
#define BLYNK_PROV_ERR_CLOUD    702    // Could not connect to the cloud
#define BLYNK_PROV_ERR_TOKEN    703    // Invalid token error (after connection)
#define BLYNK_PROV_ERR_INTERNAL 704    // Other issues (i.e. hardware failure)

struct ConfigStore {
  uint32_t  magic;
  char      version[15];
  uint8_t   flags;

  char      wifiSSID[34];
  char      wifiPass[64];

  char      cloudToken[34];
  char      cloudHost[34];
  uint16_t  cloudPort;

  uint32_t  staticIP;
  uint32_t  staticMask;
  uint32_t  staticGW;
  uint32_t  staticDNS;
  uint32_t  staticDNS2;

  int       last_error;

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
  BLYNK_FIRMWARE_VERSION,
  0x00,
  
  "",
  "",
  
  "invalid token",
  CONFIG_DEFAULT_SERVER,
  CONFIG_DEFAULT_PORT,
  0,
  BLYNK_PROV_ERR_NONE
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
    BLYNK_PARAM_KV("host" , CONFIG_DEFAULT_SERVER)
    BLYNK_PARAM_KV("port" , BLYNK_TOSTRING(CONFIG_DEFAULT_PORT))
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

#include <sfud.h>
const sfud_flash *_flash = sfud_get_device_table() + 0;

void config_load()
{

  uint8_t has_config;
  sfud_err result = sfud_read(_flash, 0, 1, &has_config);
  
  if (!has_config == 42)
  {
    DEBUG_PRINT("Using default config.");
    configStore = configDefault;
    return;
  }
    
  memset(&configStore, 0, sizeof(configStore));
  
  uint8_t b[sizeof(configStore)];

  result = sfud_read(_flash, 1, sizeof(b), b);
  memcpy(&configStore, b, sizeof(configStore));
  
}

bool config_save()
{

  sfud_err result = sfud_erase(_flash, 0, sizeof(configStore)+1);
  delay(100);
  if (!result == SFUD_SUCCESS) { DEBUG_PRINT("Erase flash data failed"); return false; }

  uint8_t b[sizeof(configStore)];
  memcpy(b, &configStore, sizeof(configStore));
  
  uint8_t status = 42;
  result = sfud_write(_flash, 0, sizeof(status), &status);
  delay(10);
  
  if (!result == SFUD_SUCCESS) { DEBUG_PRINT("Write the flash data failed"); return false; }
  
  result = sfud_write(_flash, 1, sizeof(b), b);
  delay(50);
  
  if (!result == SFUD_SUCCESS) { DEBUG_PRINT("Write the flash data failed"); return false; }
  
  DEBUG_PRINT("Configuration stored to flash");
  return true;
}

bool config_init()
{
  if (sfud_init() != SFUD_SUCCESS) { DEBUG_PRINT("SFUD init failed"); return false; }
  
  sfud_qspi_fast_read_enable(sfud_get_device(SFUD_W25Q32_DEVICE_INDEX), 2);
  
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

void config_set_last_error(int error) {
  // Only set error if not provisioned
  if (!configStore.getFlag(CONFIG_FLAG_VALID)) {
    configStore = configDefault;
    
    sfud_err result = sfud_erase(_flash, 0, 1);
    
    configStore.last_error = error;
    BLYNK_LOG2("Last error code: ", error);
    config_save();
  }
}

