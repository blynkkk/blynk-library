/**
 * @file       BlynkSerial.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkStream_h
#define BlynkStream_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Serial"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>

class BlynkTransportStream
{
public:
    BlynkTransportStream()
        : stream(NULL), conn(0)
    {}

    // IP Redirect not available
    void begin(char* h, uint16_t p) {}

    void begin(Stream& s) {
        stream = &s;
    }

    bool connect() {
        BLYNK_LOG1(BLYNK_F("Connecting..."));
        stream->flush();
        return conn = true;
    }
    void disconnect() { conn = false; }

    size_t read(void* buf, size_t len) {
        char* beg = (char*)buf;
        char* end = beg + len;
        millis_time_t startMillis = millis();
        while ((beg < end) && (millis() - startMillis < BLYNK_TIMEOUT_MS)) {
            int c = stream->read();
            if (c < 0)
                continue;
            *beg++ = (char)c;
        }
        return beg-(char*)buf;
    }
    size_t write(const void* buf, size_t len) {
        stream->write((const uint8_t*)buf, len);
        return len;
    }

    bool connected() { return conn; }
    int available() { return stream->available(); }

protected:
    Stream* stream;
    bool    conn;
};

class BlynkStream
    : public BlynkProtocol<BlynkTransportStream>
{
    typedef BlynkProtocol<BlynkTransportStream> Base;
public:
    BlynkStream(BlynkTransportStream& transp)
        : Base(transp)
    {}

    void begin(const char* auth, Stream& stream) {
        Base::begin(auth);
        this->conn.begin(stream);
    }
};

#endif
