#ifndef BlynkSerial_h
#define BlynkSerial_h

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
public:
    BlynkSerial(T& conn)
        : BlynkProtocol<T>(conn)
    {}

    void begin(const char* auth, uint32_t baud = 115200) {
        this->authkey = auth;
        this->conn.begin(baud);
    }

    void run(void)
    {
        if(!this->conn.connected()) {
            BLYNK_LOG("Reconnecting...");
            this->connect();
        }
        if (this->conn.available()) {
            this->processInput();
        }
    }

};

#endif
