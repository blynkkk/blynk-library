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
        #define BLYNK_INFO_DEVICE  "Energia"
        #define BLYNK_USE_128_VPINS

        #if   defined(__MSP430F5529__)
        #define BLYNK_INFO_CPU  "MSP430F5529"
        #define BLYNK_NO_FLOAT
        #endif

    #elif defined(LINUX)

        #define BLYNK_INFO_DEVICE  "Linux"
        #define BLYNK_USE_128_VPINS

    #elif defined(SPARK) || defined(PARTICLE)

        #define BLYNK_USE_128_VPINS

        #if PLATFORM_ID==0
        #define BLYNK_INFO_DEVICE  "Particle Core"
        #elif PLATFORM_ID==6
        #define BLYNK_INFO_DEVICE  "Particle Photon"
        #elif PLATFORM_ID==8
        #define BLYNK_INFO_DEVICE  "Particle P1"
        #elif PLATFORM_ID==9
        #define BLYNK_INFO_DEVICE  "Particle Ethernet"
        #elif PLATFORM_ID==10
        #define BLYNK_INFO_DEVICE  "Particle Electron"
        #elif PLATFORM_ID==82
        #define BLYNK_INFO_DEVICE  "Digistump Oak"
        #elif PLATFORM_ID==88
        #define BLYNK_INFO_DEVICE  "RedBear Duo"
        #elif PLATFORM_ID==103
        #define BLYNK_INFO_DEVICE  "Bluz"
        #else
        #warning "Cannot detect board type"
        #define BLYNK_INFO_DEVICE  "Particle"
        #endif

    #elif defined(MBED_LIBRARY_VERSION)

        #define BLYNK_INFO_DEVICE  "MBED"
        #define BLYNK_USE_128_VPINS

    #elif defined(ARDUINO) && defined(MPIDE)
        #define BLYNK_NO_YIELD

        #if   defined(_BOARD_UNO_)
        #define BLYNK_INFO_DEVICE  "chipKIT Uno32"
        #else
        #define BLYNK_INFO_DEVICE  "chipKIT"
        #endif

    #elif defined(ARDUINO) && defined(TEENSYDUINO)

        #if   defined(__MK20DX256__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.2/3.1"
        #define BLYNK_USE_128_VPINS
        #elif   defined(__MK20DX128__)
        #define BLYNK_INFO_DEVICE  "Teensy 3.0"
        #define BLYNK_USE_128_VPINS
        #elif   defined(__MKL26Z64__)
        #define BLYNK_INFO_DEVICE  "Teensy LC"
        #elif   defined(ARDUINO_ARCH_AVR)
        #define BLYNK_INFO_DEVICE  "Teensy 2.0"
        #else
        #define BLYNK_INFO_DEVICE  "Teensy"
        #endif

    #elif defined(ARDUINO)

        #ifdef ESP8266
            #define BLYNK_USE_128_VPINS
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
        #elif defined(ARDUINO_AVR_ADK)
        #define BLYNK_INFO_DEVICE  "Arduino Mega ADK"
        #define BLYNK_USE_128_VPINS
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

        /* Arduino SAM/SAMD */
        #elif defined(ARDUINO_SAM_DUE)
        #define BLYNK_INFO_DEVICE  "Arduino Due"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_SAMD_ZERO)
        #define BLYNK_INFO_DEVICE  "Arduino Zero"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_SAMD_MKR1000)
        #define BLYNK_INFO_DEVICE  "Arduino MKR1000"
        #define BLYNK_USE_128_VPINS

        /* Intel */
        #elif defined(ARDUINO_GALILEO)
        #define BLYNK_INFO_DEVICE  "Galileo"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_GALILEOGEN2)
        #define BLYNK_INFO_DEVICE  "Galileo Gen2"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_EDISON)
        #define BLYNK_INFO_DEVICE  "Edison"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_ARCH_ARC32)
        #define BLYNK_INFO_DEVICE  "Arduino 101"
        #define BLYNK_USE_128_VPINS

        /* Konekt */
        #elif defined(ARDUINO_DASH)
        #define BLYNK_INFO_DEVICE  "Dash"
        #elif defined(ARDUINO_DASHPRO)
        #define BLYNK_INFO_DEVICE  "Dash Pro"

        /* Red Bear Lab */
        #elif defined(ARDUINO_RedBear_Duo)
        #define BLYNK_INFO_DEVICE  "RedBear Duo"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_BLEND)
        #define BLYNK_INFO_DEVICE  "Blend"
        #elif defined(ARDUINO_BLEND_MICRO_8MHZ) || defined(ARDUINO_BLEND_MICRO_16MHZ)
        #define BLYNK_INFO_DEVICE  "Blend Micro"
        #elif defined(ARDUINO_RBL_nRF51822)
        #define BLYNK_INFO_DEVICE  "BLE Nano"

        /* ESP8266 */
        #elif defined(ARDUINO_ESP8266_ESP01)
        #define BLYNK_INFO_DEVICE  "ESP8266"
        #elif defined(ARDUINO_ESP8266_ESP12)
        #define BLYNK_INFO_DEVICE  "ESP-12"
        #elif defined(ARDUINO_ESP8266_NODEMCU)
        #define BLYNK_INFO_DEVICE  "NodeMCU"
        #elif defined(ARDUINO_ESP8266_THING)
        #define BLYNK_INFO_DEVICE  "Esp Thing"
        #elif defined(ARDUINO_ESP8266_THING_DEV)
        #define BLYNK_INFO_DEVICE  "Esp Thing Dev"

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
        #elif defined(ARDUINO_WILDFIRE_V3)
        #define BLYNK_INFO_DEVICE  "Wildfire V3"
        #define BLYNK_USE_128_VPINS
        #elif defined(ARDUINO_WILDFIRE_V4)
        #define BLYNK_INFO_DEVICE  "Wildfire V4"
        #define BLYNK_USE_128_VPINS

        /* Simblee */
        #elif defined(__Simblee__)
        #define BLYNK_INFO_DEVICE  "Simblee"
        #define BLYNK_USE_128_VPINS

        #else
        #warning "Cannot detect board type"
        #define BLYNK_INFO_DEVICE  "Arduino"
        #endif

	#endif
#endif

#endif
