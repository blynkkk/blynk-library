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
#include <stddef.h>

// General defines

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define BLYNK_ATTR_PACKED __attribute__ ((__packed__))
#define BLYNK_FORCE_INLINE __attribute__((always_inline))
#define BLYNK_NORETURN __attribute__ ((noreturn))

#if defined(__AVR__)
    #include <avr/pgmspace.h>
    #define BLYNK_HAS_PROGMEM
    #define BLYNK_PROGMEM PROGMEM
#else
    #define BLYNK_PROGMEM
#endif

// Diagnostic defines

size_t BlynkFreeRam();
void BlynkReset();
void BlynkFatal() BLYNK_NORETURN;

#define BLYNK_FATAL(msg, ...){ BLYNK_LOG(msg, ##__VA_ARGS__); BlynkFatal(); }
#define BLYNK_LOG_RAM()      { BLYNK_LOG("Free RAM: %d", BlynkFreeRam()); }
#define BLYNK_LOG_FN()       BLYNK_LOG("%s@%d", __FUNCTION__, __LINE__);

#ifdef BLYNK_PRINT

    #if defined(ARDUINO)

        #include <avr/pgmspace.h>
        #include <stdio.h>
        #include <stdarg.h>
        #include <Arduino.h>
        #define _S(s) PSTR(s)

        #define BLYNK_DBG_DUMP(msg, addr, len) { BLYNK_PRINT.print(msg); BLYNK_PRINT.write((uint8_t*)addr, len); BLYNK_PRINT.println(); }
        #define BLYNK_DBG_BREAK()    { for(;;); }
        #define BLYNK_LOG(msg, ...)  blynk_dbg_print(PSTR(msg), ##__VA_ARGS__)
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_LOG("Assertion %s failed.", #expr); BLYNK_DBG_BREAK() } }

        static
        void blynk_dbg_print(const PROGMEM char *fmt, ...)
        {
            va_list ap;
            va_start(ap, fmt);
            char buff[128];
            BLYNK_PRINT.print('[');
            BLYNK_PRINT.print(millis());
            BLYNK_PRINT.print(F("] "));
            vsnprintf_P(buff, sizeof(buff), fmt, ap);
            BLYNK_PRINT.println(buff);
            va_end(ap);
        }

    #elif defined(LINUX)

        #include <assert.h>
        #include <stdio.h>
        #include <string.h>
        #include <errno.h>
        #include <signal.h>

        #define BLYNK_DBG_DUMP(msg, addr, len) { BLYNK_LOG(msg); fwrite(addr, len, 1, stderr); }
        #define BLYNK_DBG_BREAK()    raise(SIGTRAP);
        #define BLYNK_LOG(msg, ...)  { fprintf(stderr, msg "\n", ##__VA_ARGS__); }
        #define BLYNK_ASSERT(expr)   assert(expr)

    #elif defined(WINDOWS)

        #include <windows.h>
        #include <stdio.h>

        #define BLYNK_DBG_BREAK()    DebugBreak();
        #define BLYNK_LOG(...)       { char buff[1024]; snprintf(buff, sizeof(buff), __VA_ARGS__); OutputDebugString(buff); }
        #define BLYNK_ASSERT(expr)   { if(!(expr)) { BLYNK_DBG_BREAK() } }

    #else

        #warning Could not detect platform
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
