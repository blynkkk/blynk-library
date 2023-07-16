/**
 * @file       BlynkDetectDevice.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkDetectDevice_h
#define BlynkDetectDevice_h

#include <Blynk/BlynkHelpers.h>

#ifndef BLYNK_INFO_CPU

    /******************************************
     * ATmega
     */

    #if   defined(__AVR_ATmega168__)
    #define BLYNK_INFO_CPU      "ATmega168"
    #elif defined(__AVR_ATmega328P__)
    #define BLYNK_INFO_CPU      "ATmega328P"
    #elif defined(__AVR_ATmega1280__)
    #define BLYNK_INFO_CPU      "ATmega1280"
    #elif defined(__AVR_ATmega1284__)
    #define BLYNK_INFO_CPU      "ATmega1284"
    #elif defined(__AVR_ATmega2560__)
    #define BLYNK_INFO_CPU      "ATmega2560"
    #elif defined(__AVR_ATmega32U4__)
    #define BLYNK_INFO_CPU      "ATmega32U4"
    #elif defined(__SAM3X8E__)
    #define BLYNK_INFO_CPU      "AT91SAM3X8E"

    /******************************************
     * ATtiny
     */

    #elif defined(__AVR_ATtiny25__)
    #define BLYNK_INFO_CPU      "ATtiny25"
    #elif defined(__AVR_ATtiny45__)
    #define BLYNK_INFO_CPU      "ATtiny45"
    #elif defined(__AVR_ATtiny85__)
    #define BLYNK_INFO_CPU      "ATtiny85"
    #elif defined(__AVR_ATtiny24__)
    #define BLYNK_INFO_CPU      "ATtiny24"
    #elif defined(__AVR_ATtiny44__)
    #define BLYNK_INFO_CPU      "ATtiny44"
    #elif defined(__AVR_ATtiny84__)
    #define BLYNK_INFO_CPU      "ATtiny84"
    #elif defined(__AVR_ATtiny2313__)
    #define BLYNK_INFO_CPU      "ATtiny2313"
    #elif defined(__AVR_ATtiny4313__)
    #define BLYNK_INFO_CPU      "ATtiny4313"
    #endif
#endif

#ifndef BLYNK_INFO_DEVICE

    #if   defined(ENERGIA)

        #define BLYNK_NO_YIELD
        #define BLYNK_USE_128_VPINS
        #define BLYNK_USE_INTERNAL_DTOSTRF

        #if   defined(ENERGIA_ARCH_MSP430)
            #define BLYNK_INFO_DEVICE  "LaunchPad MSP430"
            #define BLYNK_INFO_CPU     "MSP430"
            #define BLYNK_NO_FLOAT
            #define BLYNK_NO_LONGLONG
        #elif defined(ENERGIA_ARCH_MSP432)
            #define BLYNK_INFO_DEVICE  "LaunchPad MSP432"
            #define BLYNK_INFO_CPU     "MSP432"
        #elif defined(ENERGIA_ARCH_TIVAC)
            #define BLYNK_INFO_DEVICE  "LaunchPad"

        #elif defined(ENERGIA_ARCH_CC3200EMT) || defined(ENERGIA_ARCH_CC3200)
            #define BLYNK_INFO_CONNECTION  "CC3200"
            #define BLYNK_SEND_CHUNK 64
            #define BLYNK_BUFFERS_SIZE 1024

            #if   defined(ENERGIA_CC3200_LAUNCHXL) //TODO: This is a bug in Energia IDE
            #define BLYNK_INFO_DEVICE  "CC3200 LaunchXL"
            #elif defined(ENERGIA_RedBearLab_CC3200)
            #define BLYNK_INFO_DEVICE  "RBL CC3200"
            #elif defined(ENERGIA_RedBearLab_WiFiMini)
            #define BLYNK_INFO_DEVICE  "RBL WiFi Mini"
            #elif defined(ENERGIA_RedBearLab_WiFiMicro)
            #define BLYNK_INFO_DEVICE  "RBL WiFi Micro"
            #endif
        #elif defined(ENERGIA_ARCH_CC3220EMT) || defined(ENERGIA_ARCH_CC3220)
            #define BLYNK_INFO_CONNECTION  "CC3220"
            #define BLYNK_SEND_CHUNK 64
            #define BLYNK_BUFFERS_SIZE 1024

            #define BLYNK_INFO_DEVICE  "CC3220"
            #define BLYNK_INFO_CPU     "CC3220"
        #endif

        #if !defined(BLYNK_INFO_DEVICE)
        #define BLYNK_INFO_DEVICE  "Energia"
        #endif

    #elif defined(LINUX)

        #define BLYNK_INFO_DEVICE  "Linux"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 4096

    #elif defined(SPARK) || defined(PARTICLE)

        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        #if PLATFORM_ID==0
        #define BLYNK_INFO_DEVICE  "Particle Core"
        #undef BLYNK_BUFFERS_SIZE // Use default on Core
        #elif PLATFORM_ID==6
        #define BLYNK_INFO_DEVICE  "Particle Photon"
        #elif PLATFORM_ID==8
        #define BLYNK_INFO_DEVICE  "Particle P1"
        #elif PLATFORM_ID==9
        #define BLYNK_INFO_DEVICE  "Particle Ethernet"
        #elif PLATFORM_ID==10
        #define BLYNK_INFO_DEVICE  "Particle Electron"

        #elif PLATFORM_ID==12
        #define BLYNK_INFO_DEVICE  "Particle Argon"
        #elif PLATFORM_ID==13
        #define BLYNK_INFO_DEVICE  "Particle Boron"
        #elif PLATFORM_ID==14
        #define BLYNK_INFO_DEVICE  "Particle Xenon"
        #elif PLATFORM_ID==26
        #define BLYNK_INFO_DEVICE  "Particle Tracker"

        #elif PLATFORM_ID==31
        #define BLYNK_INFO_DEVICE  "Particle RPi"
        #elif PLATFORM_ID==82
        #define BLYNK_INFO_DEVICE  "Digistump Oak"
        #elif PLATFORM_ID==88
        #define BLYNK_INFO_DEVICE  "RedBear Duo"
        #elif PLATFORM_ID==103
        #define BLYNK_INFO_DEVICE  "Bluz"
        #else
        #if defined(BLYNK_DEBUG_ALL)
            #warning "Cannot detect board type"
        #endif
        #define BLYNK_INFO_DEVICE  "Particle"
        #endif

    #elif defined(__MBED__)

        #define BLYNK_INFO_DEVICE  "MBED"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 512
        #define noInterrupts() __disable_irq()
        #define interrupts()   __enable_irq()

    #elif defined(ARDUINO) && defined(MPIDE)
        #define BLYNK_NO_YIELD

        #if   defined(_BOARD_UNO_)
        #define BLYNK_INFO_DEVICE  "chipKIT Uno32"
        #else
        #define BLYNK_INFO_DEVICE  "chipKIT"
        #endif

    #elif defined(ARDUINO) && defined(ARDUINO_AMEBA)
        #if defined(BOARD_RTL8710)
        #define BLYNK_INFO_DEVICE  "RTL8710"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(BOARD_RTL8711AM)
        #define BLYNK_INFO_DEVICE  "RTL8711AM"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(BOARD_RTL8195A)
        #define BLYNK_INFO_DEVICE  "RTL8195A"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #else
        #define BLYNK_INFO_DEVICE  "Ameba"
        #endif

    #elif defined(ARDUINO) && defined(TEENSYDUINO)

        #if   defined(__MK66FX1M0__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.6"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(__MK64FX512__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.5"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(__MK20DX256__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.2/3.1"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif   defined(__MK20DX128__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.0"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif   defined(__MKL26Z64__)
        #define BLYNK_INFO_DEVICE  "Teensy LC"
        #define BLYNK_BUFFERS_SIZE 512
        #elif   defined(ARDUINO_ARCH_AVR)
        #define BLYNK_INFO_DEVICE  "Teensy 2.0"
        #else
        #define BLYNK_INFO_DEVICE  "Teensy"
        #endif

    #elif defined(ARDUINO)

        #if defined(ARDUINO_ARCH_SAMD) || \
            defined(ESP32) || defined(ESP8266) || \
            defined(ARDUINO_ARCH_RP2040) || \
            defined(ARDUINO_ARCH_NRF52840) || \
            defined(ARDUINO_ARCH_RENESAS)

            #define BLYNK_USE_128_VPINS
            #define BLYNK_BUFFERS_SIZE 1024
        #endif

        #if defined(ESP32)
            #define BLYNK_NO_ANALOG_PINS
        #endif

        #if defined(ARDUINO_ARCH_AVR)
            #define BLYNK_USE_INTERNAL_ATOLL
            #define BLYNK_MAX_TIMERS 8
        #endif

        #if defined(ARDUINO_ARCH_SAMD)
            #define BLYNK_USE_INTERNAL_DTOSTRF
        #endif

        /* Arduino AVR */
        #if   defined(ARDUINO_AVR_NANO)
        #define BLYNK_INFO_DEVICE  "Arduino Nano"
        #elif defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_DUEMILANOVE)
        #define BLYNK_INFO_DEVICE  "Arduino Uno"
        #elif defined(ARDUINO_AVR_YUN)
        #define BLYNK_INFO_DEVICE  "Arduino Yun"
        #elif defined(ARDUINO_AVR_MINI)
        #define BLYNK_INFO_DEVICE  "Arduino Mini"
        #elif defined(ARDUINO_AVR_ETHERNET)
        #define BLYNK_INFO_DEVICE  "Arduino Ethernet"
        #elif defined(ARDUINO_AVR_FIO)
        #define BLYNK_INFO_DEVICE  "Arduino Fio"
        #elif defined(ARDUINO_AVR_BT)
        #define BLYNK_INFO_DEVICE  "Arduino BT"
        #elif defined(ARDUINO_AVR_PRO)
        #define BLYNK_INFO_DEVICE  "Arduino Pro"
        #elif defined(ARDUINO_AVR_NG)
        #define BLYNK_INFO_DEVICE  "Arduino NG"
        #elif defined(ARDUINO_AVR_GEMMA)
        #define BLYNK_INFO_DEVICE  "Arduino Gemma"
        #elif defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)
        #define BLYNK_INFO_DEVICE  "Arduino Mega"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(ARDUINO_AVR_ADK)
        #define BLYNK_INFO_DEVICE  "Arduino Mega ADK"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(ARDUINO_AVR_LEONARDO)
        #define BLYNK_INFO_DEVICE  "Arduino Leonardo"
        #elif defined(ARDUINO_AVR_MICRO)
        #define BLYNK_INFO_DEVICE  "Arduino Micro"
        #elif defined(ARDUINO_AVR_ESPLORA)
        #define BLYNK_INFO_DEVICE  "Arduino Esplora"
        #elif defined(ARDUINO_AVR_LILYPAD)
        #define BLYNK_INFO_DEVICE  "Lilypad"
        #elif defined(ARDUINO_AVR_LILYPAD_USB)
        #define BLYNK_INFO_DEVICE  "Lilypad USB"
        #elif defined(ARDUINO_AVR_ROBOT_MOTOR)
        #define BLYNK_INFO_DEVICE  "Robot Motor"
        #elif defined(ARDUINO_AVR_ROBOT_CONTROL)
        #define BLYNK_INFO_DEVICE  "Robot Control"

        /* Arduino megaAVR */
        #elif defined(ARDUINO_AVR_UNO_WIFI_REV2)
        #define BLYNK_INFO_DEVICE  "Arduino UNO WiFi Rev2"

        /* Arduino SAM */
        #elif defined(ARDUINO_SAM_DUE)
        #define BLYNK_INFO_DEVICE  "Arduino Due"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        /* Arduino SAMD */
        #elif defined(ARDUINO_SAMD_ZERO)
        #define BLYNK_INFO_DEVICE  "Arduino Zero"
        #elif defined(ARDUINO_SAMD_MKR1000)
        #define BLYNK_INFO_DEVICE  "MKR1000"
        #elif defined(ARDUINO_SAMD_MKRZERO)
        #define BLYNK_INFO_DEVICE  "MKRZERO"
        #elif defined(ARDUINO_SAMD_MKRNB1500)
        #define BLYNK_INFO_DEVICE  "MKR NB 1500"
        #elif defined(ARDUINO_SAMD_MKRGSM1400)
        #define BLYNK_INFO_DEVICE  "MKR GSM 1400"
        #elif defined(ARDUINO_SAMD_MKRWAN1300)
        #define BLYNK_INFO_DEVICE  "MKR WAN 1300"
        #elif defined(ARDUINO_SAMD_MKRFox1200)
        #define BLYNK_INFO_DEVICE  "MKR FOX 1200"
        #elif defined(ARDUINO_SAMD_MKRWIFI1010)
        #define BLYNK_INFO_DEVICE  "MKR WiFi 1010"
        #elif defined(ARDUINO_SAMD_MKRVIDOR4000)
        #define BLYNK_INFO_DEVICE  "MKR Vidor 4000"
        #elif defined(ARDUINO_SAMD_NANO_33_IOT)
        #define BLYNK_INFO_DEVICE  "Nano 33 IoT"
        #elif defined(TARGET_ARDUINO_NANO33BLE) || defined(ARDUINO_ARDUINO_NANO33BLE) || defined(ARDUINO_NANO33BLE)
        #define BLYNK_INFO_DEVICE  "Nano 33 BLE"

        /* Arduino RA */
        #elif defined(ARDUINO_MINIMA)
        #define BLYNK_INFO_DEVICE  "UNO R4 Minima"
        #elif defined(ARDUINO_UNOWIFIR4)
        #define BLYNK_INFO_DEVICE  "UNO R4 WiFi"
        #elif defined(ARDUINO_PORTENTA_C33)
        #define BLYNK_INFO_DEVICE  "Portenta C33"

        /* RapsberryPi */
        #elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
        #define BLYNK_INFO_DEVICE  "RPi Pico W"
        #elif defined(ARDUINO_RASPBERRY_PI_PICO)
        #define BLYNK_INFO_DEVICE  "RPi Pico"
        #elif defined(ARDUINO_ARCH_RP2040)
        #define BLYNK_INFO_DEVICE  "RP2040"

        /* Intel */
        #elif defined(ARDUINO_GALILEO)
        #define BLYNK_INFO_DEVICE  "Galileo"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 4096
        #elif defined(ARDUINO_GALILEOGEN2)
        #define BLYNK_INFO_DEVICE  "Galileo Gen2"
        #define BLYNK_BUFFERS_SIZE 4096
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_EDISON)
        #define BLYNK_INFO_DEVICE  "Edison"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 4096
        #elif defined(ARDUINO_ARCH_ARC32)
        #define BLYNK_INFO_DEVICE  "Arduino 101"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        /* Konekt */
        #elif defined(ARDUINO_DASH)
        #define BLYNK_INFO_DEVICE  "Dash"
        #elif defined(ARDUINO_DASHPRO)
        #define BLYNK_INFO_DEVICE  "Dash Pro"

        /* Red Bear Lab */
        #elif defined(ARDUINO_RedBear_Duo)
        #define BLYNK_INFO_DEVICE  "RedBear Duo"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(ARDUINO_BLEND)
        #define BLYNK_INFO_DEVICE  "Blend"
        #elif defined(ARDUINO_BLEND_MICRO_8MHZ) || defined(ARDUINO_BLEND_MICRO_16MHZ)
        #define BLYNK_INFO_DEVICE  "Blend Micro"
        #elif defined(ARDUINO_RBL_nRF51822)
        #define BLYNK_INFO_DEVICE  "BLE Nano"

        /* ESP8266 */
        #elif defined(ARDUINO_ESP8266_NODEMCU)
        #define BLYNK_INFO_DEVICE  "NodeMCU"
        #elif defined(ARDUINO_ARCH_ESP8266)
        #define BLYNK_INFO_DEVICE  "ESP8266"

        /* ESP32 */
        #elif defined(ARDUINO_ESP32C3_DEV) || defined(CONFIG_IDF_TARGET_ESP32C3)
        #define BLYNK_INFO_DEVICE  "ESP32-C3"
        #elif defined(ARDUINO_ESP32C6_DEV) || defined(CONFIG_IDF_TARGET_ESP32C6)
        #define BLYNK_INFO_DEVICE  "ESP32-C6"
        #elif defined(ARDUINO_ESP32S3_DEV) || defined(CONFIG_IDF_TARGET_ESP32S3)
        #define BLYNK_INFO_DEVICE  "ESP32-S3"
        #elif defined(ARDUINO_ESP32S2_DEV) || defined(CONFIG_IDF_TARGET_ESP32S2)
        #define BLYNK_INFO_DEVICE  "ESP32-S2"
        #elif defined(ARDUINO_ARCH_ESP32)  || defined(CONFIG_IDF_TARGET_ESP32)
        #define BLYNK_INFO_DEVICE  "ESP32"

        /* STM32 */
        #elif defined(ARDUINO_ARCH_STM32F1)
        #define BLYNK_INFO_DEVICE  "STM32F1"
        #define BLYNK_NO_YIELD
        #elif defined(ARDUINO_ARCH_STM32F3)
        #define BLYNK_INFO_DEVICE  "STM32F3"
        #define BLYNK_NO_YIELD
        #elif defined(ARDUINO_ARCH_STM32F4)
        #define BLYNK_INFO_DEVICE  "STM32F4"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #define BLYNK_NO_YIELD

        /* Digistump */
        #elif defined(ARDUINO_ESP8266_OAK)
        #define BLYNK_INFO_DEVICE  "Oak"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_AVR_DIGISPARK)
        #define BLYNK_INFO_DEVICE  "Digispark"
        #define BLYNK_NO_YIELD
        #elif defined(ARDUINO_AVR_DIGISPARKPRO)
        #define BLYNK_INFO_DEVICE  "Digispark Pro"
        #define BLYNK_NO_YIELD

        /* Microduino */
        #elif defined(ARDUINO_AVR_USB)
        #define BLYNK_INFO_DEVICE  "CoreUSB"
        #elif defined(ARDUINO_AVR_PLUS)
        #define BLYNK_INFO_DEVICE  "Core+"
        #elif defined(ARDUINO_AVR_RF)
        #define BLYNK_INFO_DEVICE  "CoreRF"

        /* Wildfire */
        #elif defined(ARDUINO_WILDFIRE_V2)
        #define BLYNK_INFO_DEVICE  "Wildfire V2"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(ARDUINO_WILDFIRE_V3)
        #define BLYNK_INFO_DEVICE  "Wildfire V3"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024
        #elif defined(ARDUINO_WILDFIRE_V4)
        #define BLYNK_INFO_DEVICE  "Wildfire V4"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

         /* Seeed studio */
        #elif defined(SEEED_WIO_TERMINAL)
        #define BLYNK_INFO_DEVICE "Seeed Wio Terminal"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        /* Simblee */
        #elif defined(__Simblee__)
        #define BLYNK_INFO_DEVICE  "Simblee"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 512

        /* RFduino */
        #elif defined(__RFduino__)
        #define BLYNK_INFO_DEVICE  "RFduino"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 512

        /* Nordic nRF52 */
        #elif defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)
        #define BLYNK_INFO_DEVICE  "nRF52"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        /* Nordic nRF51 */
        #elif defined(ARDUINO_ARCH_NRF5)
        #define BLYNK_INFO_DEVICE  "nRF51"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 512

        #else
        #if defined(BLYNK_DEBUG_ALL)
            #warning "Cannot detect board type"
        #endif
        #define BLYNK_INFO_DEVICE  "Arduino"
        #endif

    #elif defined(TI_CC3220)
        #define BLYNK_INFO_DEVICE  "TI CC3220"
        #define BLYNK_USE_128_VPINS
        #define BLYNK_BUFFERS_SIZE 1024

        #define BLYNK_USE_INTERNAL_DTOSTRF

    #else

        #define BLYNK_INFO_DEVICE  "Custom platform"

    #endif

    #if !defined(BLYNK_MAX_READBYTES) && defined(BLYNK_BUFFERS_SIZE)
    #define BLYNK_MAX_READBYTES  BLYNK_BUFFERS_SIZE
    #endif

    #if !defined(BLYNK_MAX_SENDBYTES) && defined(BLYNK_BUFFERS_SIZE)
    #define BLYNK_MAX_SENDBYTES  BLYNK_BUFFERS_SIZE
    #endif

    // Print diagnostics

    #if defined(BLYNK_DEBUG_ALL)
        #if defined(BLYNK_INFO_DEVICE)
        #pragma message ("BLYNK_INFO_DEVICE=" BLYNK_TOSTRING(BLYNK_INFO_DEVICE))
        #endif

        #if defined(BLYNK_INFO_CPU)
        #pragma message ("BLYNK_INFO_CPU="    BLYNK_TOSTRING(BLYNK_INFO_CPU))
        #endif

        #if defined(BLYNK_BUFFERS_SIZE)
        #pragma message ("BLYNK_BUFFERS_SIZE=" BLYNK_TOSTRING(BLYNK_BUFFERS_SIZE))
        #endif
    #endif

#endif

#endif
