/**
 * @file       WidgetTerminal.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 */

#ifndef WidgetTerminal_h
#define WidgetTerminal_h

#if defined(ARDUINO) && !(defined(LINUX) || defined(__MBED__))
    #define BLYNK_USE_PRINT_CLASS
#endif

#include <Blynk/BlynkWidgetBase.h>

#ifdef BLYNK_USE_PRINT_CLASS
    #if !(defined(SPARK) || defined(PARTICLE) || (PLATFORM_ID==88) || defined(ARDUINO_RedBear_Duo)) // 88 -> RBL Duo
        // On Particle this is auto-included
        #include <Print.h>
    #endif
#endif

class WidgetTerminal
    : public BlynkWidgetBase
#ifdef BLYNK_USE_PRINT_CLASS
    , public Print
#endif
{
public:
    WidgetTerminal(uint8_t vPin)
        : BlynkWidgetBase(vPin)
        , mOutQty(0)
    {}

    //virtual ~WidgetTerminal() {}

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    virtual void flush() {
        if (mOutQty) {
            Blynk.virtualWriteBinary(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }
    
    void clear() {
        flush();
        Blynk.virtualWrite(mPin, "clr");
    }

#ifdef BLYNK_USE_PRINT_CLASS

    using Print::write;

    virtual size_t write(const void* buff, size_t len) {
        return write((char*)buff, len);
    }

#else

    virtual size_t write(const void* buff, size_t len) {
        uint8_t* buf = (uint8_t*)buff;
        size_t left = len;
        while (left--) {
            write(*buf++);
        }
        return len;
    }

    virtual size_t write(const char* str) {
        return write(str, strlen(str));
    }

#endif

private:
    uint8_t mOutBuf[64];
    uint8_t mOutQty;
};

#endif
