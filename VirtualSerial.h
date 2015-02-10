#ifndef VirtualSerial_h
#define VirtualSerial_h

#include <Print.h>
#include <Blynk/BlynkApi.h>

class VirtualSerial
    : public Print
{
public:
    typedef void (*WriteHandler) (const uint8_t* buff, size_t len);

    VirtualSerial(int pin, WriteHandler on_write = NULL)
        : mPin(pin), mOutQty(0), mOnWrite(on_write)
    {
        Blynk.attachVitrualPin(pin, WidgetWriteHandler(this, &VirtualSerial::onWrite));
    }
    ~VirtualSerial() {}

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty > sizeof(mOutBuf)) {
            Blynk.virtualWrite(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
        return 1;
    }
	
    using Print::write;
	
private:
    void onWrite(BlynkReq& req, const BlynkParam& param) {
        if (mOnWrite) {
            mOnWrite((uint8_t*)param.getBuffer(), param.getLength());
        }
    }

private:
    //BlynkApi&	mBlynk;
    int			mPin;
    uint8_t		mOutBuf[16];
    uint8_t		mOutQty;
    WriteHandler mOnWrite;
};

#endif
