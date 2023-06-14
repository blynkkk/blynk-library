/**
 * @file       BlynkHelpers.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       May 2016
 * @brief
 *
 */

#ifndef BlynkHelpers_h
#define BlynkHelpers_h

#if defined(SPARK) || defined(PARTICLE)
    #include "application.h"
#endif

#if defined(ARDUINO)
    #if ARDUINO >= 100
        #include <Arduino.h>
    #else
        #include <WProgram.h>
    #endif
#endif

#if defined(LINUX)
    #if defined(RASPBERRY)
        #include <wiringPi.h>
    #endif
#endif

#include <stddef.h>
#include <inttypes.h>

// General defines

#define BLYNK_NEWLINE         "\r\n"

#define BLYNK_CONCAT(a, b)    a ## b
#define BLYNK_CONCAT2(a, b)   BLYNK_CONCAT(a, b)

#define BLYNK_STRINGIFY(x)    #x
#define BLYNK_TOSTRING(x)     BLYNK_STRINGIFY(x)

#define BLYNK_COUNT_OF(x)     ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define BLYNK_ATTR_PACKED     __attribute__ ((__packed__))
#define BLYNK_NORETURN        __attribute__ ((noreturn))
#define BLYNK_UNUSED          __attribute__ ((__unused__))
#define BLYNK_DEPRECATED      __attribute__ ((deprecated))
#define BLYNK_CONSTRUCTOR     __attribute__ ((constructor))
#define BLYNK_FALLTHROUGH     __attribute__ ((fallthrough))

// Causes problems on some platforms
#define BLYNK_FORCE_INLINE    inline //__attribute__((always_inline))

#if !defined(BLYNK_RUN_YIELD)
    #if defined(BLYNK_NO_YIELD)
        #define BLYNK_RUN_YIELD() {}
    #elif defined(SPARK) || defined(PARTICLE)
        #define BLYNK_RUN_YIELD() { Particle.process(); }
    #elif !defined(ARDUINO) || (ARDUINO < 151)
        #define BLYNK_RUN_YIELD() {}
    #else
        #define BLYNK_RUN_YIELD() { BlynkDelay(0); }
    #endif
#endif

#if defined(__AVR__)
    #include <avr/pgmspace.h>
    #define BLYNK_HAS_PROGMEM
    #define BLYNK_PROGMEM     PROGMEM
    #define BLYNK_F(s)        F(s)
    #define BLYNK_PSTR(s)     PSTR(s)
#else
    #define BLYNK_PROGMEM
    #define BLYNK_F(s)        s
    #define BLYNK_PSTR(s)     s
#endif

#if defined(__has_include)
    #if __has_include(<functional>)
        #include <functional>
        #define BLYNK_HAS_FUNCTIONAL_H
    #endif
#endif

#if defined(BLYNK_MULTITHREADED)
    #include <mutex>

    #define BLYNK_MUTEX_DECL(x)     std::mutex x
    #define BLYNK_MUTEX_GUARD(x)    std::lock_guard<std::mutex> BLYNK_CONCAT2(lg_, __LINE__)(x)
#else
    #define BLYNK_MUTEX_DECL(x)
    #define BLYNK_MUTEX_GUARD(x)
#endif

#ifdef ARDUINO_AVR_DIGISPARK
    typedef fstr_t __FlashStringHelper;
#endif

#endif /* BlynkHelpers_h */
