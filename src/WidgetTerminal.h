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
    #define BLYNK_TERMINAL_BUFF_OUT BLYNK_MAX_SENDBYTES
#endif
#ifndef BLYNK_TERMINAL_BUFF_IN
    #define BLYNK_TERMINAL_BUFF_IN BLYNK_MAX_READBYTES
#endif

#if defined(ARDUINO) && !(defined(LINUX) || defined(__MBED__))
    #define BLYNK_USE_STREAM_CLASS
#elif defined(SPARK) || defined(PARTICLE)
    #define BLYNK_USE_STREAM_CLASS
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

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (byte == '\n' && Blynk.connected()) {
            flush();
        }
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    virtual void flush() {
        if (mOutQty) {
            // TODO: split into BLYNK_MAX_SENDBYTES
            Blynk.virtualWriteBinary(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }

    /*
     * Reading
     */

    virtual int read()      { return mRxBuff.readable() ? mRxBuff.get() : -1;  }
    virtual int available() { return mRxBuff.size(); }
    virtual int peek()      { return mRxBuff.readable() ? mRxBuff.peek() : -1; }

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
    typedef BlynkFifo<uint8_t, BLYNK_TERMINAL_BUFF_IN> InputFifo;

    bool        mAppendCR = false;
    bool        mAppendLF = false;
    InputFifo   mRxBuff;

    uint8_t     mOutBuf[BLYNK_TERMINAL_BUFF_OUT];
    unsigned    mOutQty;
};

#endif
