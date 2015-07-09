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

#include <Blynk/BlynkApi.h>
#if !defined(SPARK) // On Spark this is auto-included
#include <Print.h>
#endif

class WidgetTerminal
    : public Print
{
public:
    WidgetTerminal(int vPin)
        : mPin(vPin), mOutQty(0)
    {}

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    void flush() {
        if (mOutQty) {
            Blynk.virtualWrite(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }

    using Print::write;

private:
    uint8_t mPin;
    uint8_t mOutBuf[32];
    uint8_t mOutQty;
};

#endif
