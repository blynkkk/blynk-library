/*
 * General options
 */

#define BOARD_FIRMWARE_VERSION      "1.0.1"
#define BOARD_HARDWARE_VERSION      "1.0.0"
#define BOARD_NAME                  "My Product"
#define BOARD_VENDOR                "My Company"
#define BOARD_CONFIG_AP_URL         "my-product.cc"       // Config page will be available in a browser at 'http://my-product.cc/'

#define PRODUCT_WIFI_SSID           "TestProduct"

// Board configuration (see examples below).
#define BOARD_BUTTON_PIN            0                     // Pin where user button is attached
#define BOARD_BUTTON_ACTIVE_HIGH    false                 // true if button is "active-high"

#define BOARD_LED_PIN               2                     // Set LED pin - if you have a simple LED attached
//#define BOARD_LED_PIN_R           15                    // Set R,G,B pins - if your LED is PWM RGB 
//#define BOARD_LED_PIN_G           12
//#define BOARD_LED_PIN_B           13
//#define BOARD_LED_PIN_WS2812      4                     // Set if your LED is WS2812 RGB
#define BOARD_LED_INVERSE           true                  // true, if you need to inverse LED signal
#define BOARD_RGB_BRIGHTNESS        32                    // 0..255 brightness control

// Example configuration for NodeMCU v1.0 Board:
/*
#define BOARD_BUTTON_PIN            0
#define BOARD_LED_PIN               16
#define BOARD_LED_INVERSE           true
*/

// Example configuration for SparkFun Blynk Board:
/*
#define BOARD_BUTTON_PIN            0
#define BOARD_LED_PIN_WS2812        4
#define BOARD_RGB_BRIGHTNESS        32
*/

// Example configuration for Witty cloud Board:
/*
#define BOARD_BUTTON_PIN            4                     // Pin where user button is attached
#define BOARD_LED_PIN_R             15
#define BOARD_LED_PIN_G             12
#define BOARD_LED_PIN_B             13
#define BOARD_LED_INVERSE           false
#define BOARD_RGB_BRIGHTNESS        32                    // 0..255
*/


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION 3000
#define BUTTON_HOLD_TIME_ACTION     10000

#define BOARD_PWM_MAX               1023

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

