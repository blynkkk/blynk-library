/*
 * General options
 */

#define BOARD_HARDWARE_VERSION        "1.0.0"

/*
 * Board configuration (see examples below).
 */

  // Custom board configuration
  #define BOARD_BUTTON_PIN            WIO_KEY_A              // Pin where user button is attached
  #define BOARD_BUTTON_ACTIVE_LOW     true                  // true if button is "active-low"

  #define BOARD_LED_PIN             LED_BUILTIN           // Set LED pin - if you have a single-color LED attached
  #define BOARD_LED_INVERSE           false                 // true if LED is common anode, false if common cathode
  #define BOARD_LED_BRIGHTNESS        255                    // 0..255 brightness control

/**
 * @file       Settings.h
 * @author     Blynk Inc.
 * @modified   Dmitry Maslov (Seeed Studio)
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2021 Blynk Inc.
 * @date       May 2021
 * @brief
 *
 */

/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000

#define BOARD_PWM_MAX                 1023

#define LEDC_CHANNEL_1     1
#define LEDC_CHANNEL_2     2
#define LEDC_CHANNEL_3     3
#define LEDC_TIMER_BITS    10
#define LEDC_BASE_FREQ     12000

#define CONFIG_AP_URL                 "blynk.setup"
#define CONFIG_DEFAULT_SERVER         "blynk.cloud"
#define CONFIG_DEFAULT_PORT           80


#define WIFI_NET_CONNECT_TIMEOUT      30000
#define WIFI_CLOUD_CONNECT_TIMEOUT    30000
#define WIFI_AP_CONFIG_PORT           80
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)
#define WIFI_CAPTIVE_PORTAL_ENABLE 1

//#define USE_TC3
//#define USE_TCC0

//#define BLYNK_NO_DEFAULT_BANNER 

#if defined(APP_DEBUG)
  #define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif
