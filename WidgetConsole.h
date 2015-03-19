/**
 * @file       WidgetConsole.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 */

#ifndef WidgetConsole_h
#define WidgetConsole_h

#include <Print.h>
#include <Blynk/BlynkApi.h>

class WidgetConsole
    : public Print
{
public:
    WidgetConsole(int pin)
        : mPin(pin), mOutQty(0)
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
