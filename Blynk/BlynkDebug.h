/**
 * @file       BlynkDebug.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Debug utilities
 *
 */

#ifndef BlynkDebug_h
#define BlynkDebug_h

#include <Blynk/BlynkConfig.h>

#define BLYNK_ATTR_PACKED __attribute__ ((__packed__))
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#ifdef BLYNK_DEBUG

    #if defined(ARDUINO)

        #include <avr/pgmspace.h>
        #include <stdio.h>
        #include <Arduino.h>
        #define _S(s) PSTR(s)

        #define BLYNK_DBG_BREAK()    { for(;;); }
        #define BLYNK_LOG(msg, ...)  { char buff[128]; snprintf_P(buff, sizeof(buff), PSTR(msg "\n"), ##__VA_ARGS__); Serial.print(buff); }
        #define BLYNK_LOG_RAM()      { BLYNK_LOG("Free RAM: %d", freeRam()); }
        #define BLYNK_TRACE()        BLYNK_LOG("> %s @%d", __PRETTY_FUNCTION__, __LINE__);
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_LOG("Assertion %s failed.", #expr); BLYNK_DBG_BREAK() } }

        static inline int freeRam() {
            extern int __heap_start, *__brkval;
            int v;
            return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
        }

    #elif defined(LINUX)

        #include <assert.h>
        #include <stdio.h>
        #include <string.h>
        #include <errno.h>
        #include <signal.h>

        #define BLYNK_DBG_BREAK()    raise(SIGTRAP);
        #define BLYNK_LOG(msg, ...)  { fprintf(stderr, msg, ##__VA_ARGS__); }
        #define BLYNK_TRACE()
        #define BLYNK_ASSERT(expr)   assert(expr)

    #elif defined(WINDOWS)

        #include <windows.h>
        #include <stdio.h>

        #define BLYNK_DBG_BREAK()    DebugBreak();
        #define BLYNK_LOG(...)       { char buff[1024]; snprintf(buff, sizeof(buff), __VA_ARGS__); OutputDebugString(buff); }
        #define BLYNK_TRACE()
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_DBG_BREAK() } }

    #else

        #warning Could not detect platform
        #define BLYNK_DBG_BREAK()    { *(char*)(NULL) = 0xFF; } // SEGV!!!
        #define BLYNK_LOG(...)
        #define BLYNK_TRACE()
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_DBG_BREAK() } }

    #endif

#else

    #define BLYNK_DBG_BREAK()
    #define BLYNK_LOG(msg, ...)
    #define BLYNK_LOG_RAM()
    #define BLYNK_TRACE()
    #define BLYNK_ASSERT(expr)

#endif

#endif
