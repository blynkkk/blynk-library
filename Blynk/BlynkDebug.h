/**
 * @file       BlynkDebug.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Debug utilities
 *
 */

#ifndef BlynkDebug_h
#define BlynkDebug_h

#include <Blynk/BlynkConfig.h>
#include <stddef.h>
#ifdef ESP8266
    extern "C" {
    #include "ets_sys.h"
    #include "os_type.h"
    #include "mem.h"
    }
#else
    #include <inttypes.h>
#endif

#if defined(SPARK) || defined(PARTICLE)
    #include "application.h"
#endif

#if defined(ARDUINO)
    #if ARDUINO >= 100
        #include "Arduino.h"
    #else
        #include "WProgram.h"
    #endif
#endif

#if !defined(ARDUINO) || (ARDUINO < 151)
    #define BLYNK_NO_YIELD
#endif

// General defines

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define BLYNK_ATTR_PACKED __attribute__ ((__packed__))
#define BLYNK_NORETURN __attribute__ ((noreturn))

// Causes problems on some platforms
#define BLYNK_FORCE_INLINE inline //__attribute__((always_inline))

#if defined(__AVR__)
    #include <avr/pgmspace.h>
    #define BLYNK_HAS_PROGMEM
    #define BLYNK_PROGMEM PROGMEM
    #define BLYNK_PSTR(s) PSTR(s)
#else
    #define BLYNK_PROGMEM
    #define BLYNK_PSTR(s) s
#endif

#ifndef LED_BUILTIN
# define LED_BUILTIN 2
#endif

// Diagnostic defines

size_t BlynkFreeRam();
void BlynkReset() BLYNK_NORETURN;
void BlynkFatal() BLYNK_NORETURN;

#define BLYNK_FATAL(msg, ...){ BLYNK_LOG(msg, ##__VA_ARGS__); BlynkFatal(); }
#define BLYNK_LOG_RAM()      { BLYNK_LOG("Free RAM: %d", BlynkFreeRam()); }
#define BLYNK_LOG_FN()       BLYNK_LOG("%s@%d", __FUNCTION__, __LINE__);
#define BLYNK_LOG_TROUBLE(t) BLYNK_LOG("Trouble detected: http://docs.blynk.cc/#troubleshooting-%s", t)

#ifdef BLYNK_PRINT

    #if defined(ARDUINO) || defined(SPARK) || defined(PARTICLE)
        #include <stdio.h>
        #include <stdarg.h>

        #define BLYNK_DBG_DUMP(msg, addr, len) if (len) { BLYNK_PRINT.print(msg); BLYNK_PRINT.write((uint8_t*)addr, len); BLYNK_PRINT.println(); }
        #define BLYNK_DBG_BREAK()    { for(;;); }
#if defined(__SAM3X8E__)
        #define BLYNK_LOG(msg, ...)  blynk_dbg_print(msg, ##__VA_ARGS__)
#else
        #define BLYNK_LOG(msg, ...)  blynk_dbg_print(BLYNK_PSTR(msg), ##__VA_ARGS__)
#endif
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_LOG("Assertion %s failed.", #expr); BLYNK_DBG_BREAK() } }

        static
        void blynk_dbg_print(const char* BLYNK_PROGMEM fmt, ...)
        {
            va_list ap;
            va_start(ap, fmt);
            char buff[128];
            BLYNK_PRINT.print('[');
            BLYNK_PRINT.print(millis());
            BLYNK_PRINT.print(F("] "));
#if defined(__AVR__)
            vsnprintf_P(buff, sizeof(buff), fmt, ap);
#else
            vsnprintf(buff, sizeof(buff), fmt, ap);
#endif
            BLYNK_PRINT.println(buff);
            va_end(ap);
        }

    #elif defined(LINUX) || defined(MBED_LIBRARY_VERSION)

        #include <assert.h>
        #include <stdio.h>
        #include <string.h>
        #include <errno.h>
        #include <signal.h>

        #define BLYNK_DBG_DUMP(msg, addr, len) if (len) { fprintf(BLYNK_PRINT, msg); fwrite(addr, len, 1, BLYNK_PRINT); fputc('\n', BLYNK_PRINT); }
        #define BLYNK_DBG_BREAK()    raise(SIGTRAP);
        #define BLYNK_LOG(msg, ...)  { fprintf(BLYNK_PRINT, "[%ld] " msg "\n", millis(), ##__VA_ARGS__); }
        #define BLYNK_ASSERT(expr)   assert(expr)

    #elif defined(WINDOWS)

        #include <windows.h>
        #include <stdio.h>

        #define BLYNK_DBG_DUMP(msg, addr, len)
        #define BLYNK_DBG_BREAK()    DebugBreak();
        #define BLYNK_LOG(...)       { char buff[1024]; snprintf(buff, sizeof(buff), __VA_ARGS__); OutputDebugString(buff); }
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_DBG_BREAK() } }

    #else

        #warning Could not detect platform
        #define BLYNK_DBG_DUMP(msg, addr, len)
        #define BLYNK_DBG_BREAK()    { *(char*)(NULL) = 0xFF; } // SEGV!!!
        #define BLYNK_LOG(...)
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_DBG_BREAK() } }

    #endif

#else

    #define BLYNK_DBG_BREAK()
    #define BLYNK_LOG(msg, ...)
    #define BLYNK_ASSERT(expr)

#endif

#endif
