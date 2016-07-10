/*
 * General options
 */

#define BOARD_FIRMWARE_VERSION      "1.0.1"
#define BOARD_HARDWARE_VERSION      "1.0.0"
#define BOARD_NAME                  "My Product"
#define BOARD_VENDOR                "My Company"
#define BOARD_CONFIG_AP_URL         "my-product.cc"       // Config page will be available in a browser at 'http://my-product.cc/'

#define PRODUCT_WIFI_SSID           "TestProduct"

#define BOARD_BUTTON_PIN            0                     // Pin where reset button is attached
#define BOARD_LED_PIN               4                     // Pin where indicator LED is attached
#define BOARD_LED_IS_WS2812         false
#define BOARD_RGB_BRIGHTNESS        32                    // 0..255


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_MIN        3000
#define BUTTON_HOLD_TIME_MAX        10000

#define WIFI_NET_CONNECT_TIMEOUT    30000
#define WIFI_CLOUD_CONNECT_TIMEOUT  15000
#define WIFI_AP_CONFIG_PORT         80
#define WIFI_AP_IP                  IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet              IPAddress(255, 255, 255, 0)
//#define WIFI_CAPTIVE_PORTAL_ENABLE

#define USE_TICKER
//#define USE_TIMER_ONE
//#define USE_TIMER_THREE

#if defined(DEBUG)
  #define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

