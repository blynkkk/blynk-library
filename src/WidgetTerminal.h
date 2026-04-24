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

#ifndef BLYNK_TERMINAL_BUFF_OUT
    #define BLYNK_TERMINAL_BUFF_OUT 250
#endif
#ifndef BLYNK_TERMINAL_BUFF_IN
    #define BLYNK_TERMINAL_BUFF_IN BLYNK_MAX_READBYTES
#endif

#if defined(ARDUINO) && !(defined(LINUX) || defined(__MBED__))
    #define BLYNK_USE_STREAM_CLASS
    #define BLYNK_STREAM_CLASS_OVERRIDE
#elif defined(SPARK) || defined(PARTICLE)
    #define BLYNK_USE_STREAM_CLASS
    #define BLYNK_STREAM_CLASS_OVERRIDE
#endif

#include <Blynk/BlynkWidgetBase.h>
#include <utility/BlynkFifo.h>

#ifdef BLYNK_USE_STREAM_CLASS
    #include <Stream.h>
#endif

class WidgetTerminal
    : public BlynkWidgetBase
#ifdef BLYNK_USE_STREAM_CLASS
    , public Stream
#endif
{
public:
    WidgetTerminal(uint8_t vPin = -1)
        : BlynkWidgetBase(vPin)
        , mOutQty(0)
    {}

    virtual ~WidgetTerminal() {}

    /*
     * Writing
     */

    virtual size_t write(uint8_t byte) BLYNK_STREAM_CLASS_OVERRIDE {
        mOutBuf[mOutQty++] = byte;
        if (byte == '\n' && Blynk.connected()) {
            flush();
        }
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    virtual void flush() BLYNK_STREAM_CLASS_OVERRIDE {
        if (mOutQty) {
            Blynk.virtualWriteBinary(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }

    /*
     * Reading
     */

    virtual int read()      BLYNK_STREAM_CLASS_OVERRIDE { return mRxBuff.readable() ? mRxBuff.get() : -1;  }
    virtual int available() BLYNK_STREAM_CLASS_OVERRIDE { return mRxBuff.size(); }
    virtual int peek()      BLYNK_STREAM_CLASS_OVERRIDE { return mRxBuff.readable() ? mRxBuff.peek() : -1; }

    void process(const BlynkParam& param) {
        mRxBuff.put((uint8_t*)param.getBuffer(), param.getLength());

        if (mAppendCR) { mRxBuff.put('\r'); }
        if (mAppendLF) { mRxBuff.put('\n'); }
    }

    void onWrite(BlynkReq BLYNK_UNUSED &request, const BlynkParam& param) {
        process(param);
    }

    /*
     * Extra
     */
    
    void clear() {
        mOutQty = 0;
        Blynk.virtualWrite(mPin, "clr");
    }

    // Append '\r' on input
    void autoAppendCR(bool v = true) {
        mAppendCR = v;
    }

    // Append '\n' on input
    void autoAppendLF(bool v = true) {
        mAppendLF = v;
    }

#ifdef BLYNK_USE_STREAM_CLASS

    using Stream::write;

#else

    virtual size_t write(const uint8_t* buff, size_t len) {
        size_t left = len;
        while (left--) {
            write(*buff++);
        }
        return len;
    }

    virtual size_t write(const char* str) {
        return write(str, strlen(str));
    }

#endif

private:
    typedef BlynkFifo<uint8_t, BLYNK_TERMINAL_BUFF_IN> InputFifo;

    bool        mAppendCR = false;
    bool        mAppendLF = false;
    InputFifo   mRxBuff;

    uint8_t     mOutBuf[BLYNK_TERMINAL_BUFF_OUT];
    unsigned    mOutQty;
};

#endif
