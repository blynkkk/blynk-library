/**
 * @file       BlynkSerial.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSerial_h
#define BlynkSerial_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Serial"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>

template <class T>
class BlynkTransportSerial
{
public:
    BlynkTransportSerial(T& stream)
        : stream(stream), conn(0)
    {}

    void begin(uint32_t baud) {
        stream.begin(baud);
    }

    bool connect() {
        BLYNK_LOG("Connecting...");
        return conn = true;
    }
    void disconnect() { conn = false; }

    size_t read(void* buf, size_t len) {
        return stream.readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return stream.write((const uint8_t*)buf, len);
    }

    bool connected() { return conn; }
    int available() { return stream.available(); }

protected:
    T&     stream;
    bool   conn;
};

template <class T>
class BlynkTransportSerialChecked
    : public BlynkTransportSerial<T>
{
public:
    BlynkTransportSerialChecked(T& stream)
        : BlynkTransportSerial<T>(stream)
    {}

    void begin(uint32_t baud) {
        this->stream.begin(baud);
        while (!this->stream) {
            // wait for serial port to connect. Needed for Leonardo only
        }
    }

    int connected() { return this->conn && this->stream; }
};

template <class T>
class BlynkSerial
    : public BlynkProtocol<T>
{
    typedef BlynkProtocol<T> Base;
public:
    BlynkSerial(T& transp)
        : Base(transp)
    {}

    void begin(const char* auth, uint32_t baud = 9600) {
        Base::begin(auth);
        this->conn.begin(baud);
    }

};

#endif
