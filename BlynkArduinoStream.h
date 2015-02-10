#ifndef BlynkArduinoStream_h
#define BlynkArduinoStream_h

#include <Arduino.h>
#include <Blynk/BlynkDebug.h>

template <class T>
class BlynkArduinoStream
{
public:
    BlynkArduinoStream(T& stream)
        : stream(stream), conn(0)
    {}

    void begin(uint32_t baud) {
        stream.begin(baud);
    }

    int connect()     { return conn = 1; }
    void disconnect() { conn = 0; }

    size_t read(void* buf, size_t len) {
        return stream.readBytes((char*)buf, len);
    }
    size_t write(const void* buf, size_t len) {
        return stream.write((const uint8_t*)buf, len);
    }
    void flush()    { stream.flush(); }
    int connected() { return conn; }
    int available() { return stream.available(); }

protected:
    T&     stream;
    int    conn;
};

template <class T>
class BlynkArduinoStreamChecked
    : public BlynkArduinoStream<T>
{
public:
    BlynkArduinoStreamChecked(T& stream)
        : BlynkArduinoStream<T>(stream)
    {}

    void begin(uint32_t baud) {
        this->stream.begin(baud);
        while (!this->stream) {
            // wait for serial port to connect. Needed for Leonardo only
        }
    }

    int connected() { return this->conn && this->stream; }
};

#endif
