
/*
 * Board configuration (see examples below).
 */

  // Example configuration for MKR1000 Board
  #define BOARD_BUTTON_PIN            0                     // Pin where user button is attached
  #define BOARD_BUTTON_ACTIVE_LOW     true                  // true if button is "active-low"

  #define BOARD_LED_PIN               6                     // Set LED pin - if you have a single-color LED attached
  //#define BOARD_LED_PIN_R           15                    // Set R,G,B pins - if your LED is PWM RGB
  //#define BOARD_LED_PIN_G           12
  //#define BOARD_LED_PIN_B           13
  //#define BOARD_LED_PIN_WS2812      4                     // Set if your LED is WS2812 RGB
  #define BOARD_LED_INVERSE           false                 // true if LED is common anode, false if common cathode
  #define BOARD_LED_BRIGHTNESS        255                   // 0..255 brightness control


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000
#define BUTTON_PRESS_TIME_ACTION      50

#define BOARD_PWM_MAX                 255

#define CONFIG_AP_URL                 "blynk.setup"
#define CONFIG_DEFAULT_SERVER         "blynk.cloud"
#define CONFIG_DEFAULT_PORT           443

#define WIFI_NET_CONNECT_TIMEOUT      50000
#define WIFI_CLOUD_CONNECT_TIMEOUT    50000
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
//#define WIFI_AP_Subnet              IPAddress(255, 255, 255, 0)
//#define WIFI_CAPTIVE_PORTAL_ENABLE

//#define USE_TICKER
//#define USE_TIMER_ONE
//#define USE_TIMER_THREE
#define USE_TIMER_FIVE
//#define USE_PTHREAD

#define BLYNK_NO_DEFAULT_BANNER

#if defined(APP_DEBUG)
  #define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

