/**
 * @file       BlynkDebug.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Debug utilities for Arduino
 */
#include <Blynk/BlynkDebug.h>

#if defined(BLYNK_USE_AVR_WDT)

    #include <Arduino.h>
    #include <avr/wdt.h>

    size_t BlynkFreeRam()
    {
        extern int __heap_start, *__brkval;
        int v;
        return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    }

    void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

    void wdt_init(void)
    {
        MCUSR = 0;
        wdt_disable();

        return;
    }

    void BlynkReset()
    {
        wdt_enable(WDTO_15MS);
        delay(50);
        void(*resetFunc)(void) = 0;
        resetFunc();
        for(;;) {} // To make compiler happy
    }

#elif defined(__AVR__)

    #include <Arduino.h>

    size_t BlynkFreeRam()
    {
        extern int __heap_start, *__brkval;
        int v;
        return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    }

    void BlynkReset()
    {
        void(*resetFunc)(void) = 0;
        resetFunc();
        for(;;) {}
    }

#elif defined(ESP8266)

    #include <Arduino.h>

    size_t BlynkFreeRam()
    {
        return ESP.getFreeHeap();
    }

    void BlynkReset()
    {
        ESP.restart();
        for(;;) {}
    }

#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)

    #include <Arduino.h>

    size_t BlynkFreeRam()
    {
        return 0;
    }

    void BlynkReset()
    {
        NVIC_SystemReset();
        for(;;) {}
    }

#elif defined (PARTICLE) || defined(SPARK)

    #include "application.h"

    size_t BlynkFreeRam()
    {
        return 0;
    }

    void BlynkReset()
    {
        System.reset();
        for(;;) {} // To make compiler happy
    }

#elif defined(__STM32F1__) || defined(__STM32F3__)

    #include <Arduino.h>
    #include <libmaple/nvic.h>

    size_t BlynkFreeRam()
    {
        return 0;
    }

    void BlynkReset()
    {
        nvic_sys_reset();
        for(;;) {}
    }

// TODO:
//#elif defined (TEENSYDUINO)
//#elif defined (__STM32F4__)
//#elif defined (ARDUINO_ARCH_ARC32)
//#elif defined (__RFduino__) || defined (__Simblee__)

#elif defined(MBED_LIBRARY_VERSION)

    #include "mbed.h"

    size_t BlynkFreeRam()
    {
        return 0;
    }

    void BlynkReset()
    {
        for(;;) {} // To make compiler happy
    }

#else

    #if defined(BLYNK_DEBUG_ALL)
        #warning "Need to implement board-specific utilities"
    #endif

    size_t BlynkFreeRam()
    {
        return 0;
    }

    void BlynkReset()
    {
        for(;;) {} // To make compiler happy
    }

#endif

void BlynkFatal()
{
    delay(10000L);
    BlynkReset();
}
