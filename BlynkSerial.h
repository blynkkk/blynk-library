#ifndef BlynkSerial_h
#define BlynkSerial_h

#include <Blynk/BlynkProtocol.h>
#include <BlynkArduinoStream.h>

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
