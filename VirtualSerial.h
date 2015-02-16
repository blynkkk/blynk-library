#ifndef VirtualSerial_h
#define VirtualSerial_h

#include <Print.h>
#include <Blynk/BlynkApi.h>

class VirtualSerial
    : public Print
{
public:
    VirtualSerial(int pin)
        : mPin(pin), mOutQty(0)
    {}

    virtual size_t write(uint8_t byte) override {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty > sizeof(mOutBuf) || byte == '\n') {
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
    uint8_t mOutBuf[16];
    uint8_t mOutQty;
};

#endif
