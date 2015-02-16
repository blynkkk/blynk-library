/**
 * @file       BlynkParam.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Blynk protocol implementation
 *
 */

#ifndef BlynkProtocol_h
#define BlynkProtocol_h

#include <string.h>
#include <stdlib.h>
#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkProtocolDefs.h>
#include <Blynk/BlynkApi.h>

template <class Transp>
class BlynkProtocol
    : public BlynkApi< BlynkProtocol<Transp> >
{
public:
    BlynkProtocol(Transp& conn)
        : conn(conn), authkey(NULL)
        , lastActivityIn(0)
        , lastActivityOut(0)
    {}
    bool connect();
    void processInput(void);
    void send(const void* data, size_t length) {
        sendCmd(BLYNK_CMD_HARDWARE, data, length);
    }

private:
    bool readHeader(BlynkHeader& hdr);
    void sendCmd(uint8_t cmd, const void* data, size_t length);

protected:
    Transp& conn;
    const char* authkey;
    unsigned long lastActivityIn;
    unsigned long lastActivityOut;
};

template <class Transp>
bool BlynkProtocol<Transp>::connect()
{
    conn.disconnect();
    if (conn.connect())
    {
        BLYNK_LOG("Transport connected");
        sendCmd(BLYNK_CMD_LOGIN, authkey, 32);

        BlynkHeader hdr;
        if (!readHeader(hdr) ||
            BLYNK_CMD_RESPONSE != hdr.type ||
            BLYNK_SUCCESS != hdr.length)
        {
            conn.disconnect();
            return false;
        }

        lastActivityIn = lastActivityOut;
        BLYNK_LOG("Blynk connected");
        return true;
    }
    return false;
}

template <class Transp>
inline
void BlynkProtocol<Transp>::processInput(void)
{
    BlynkHeader hdr;
    if (!readHeader(hdr))
        return;

    const uint32_t t = millis();
    lastActivityIn = t;

    //BLYNK_LOG("Message %d,%d,%d", hdr.type, hdr.msg_id, hdr.length);

    if (hdr.type == BLYNK_CMD_RESPONSE)
        return;

    if (hdr.length > BLYNK_MAX_READBYTES) {
        BLYNK_LOG("Packet size (%d) > max allowed (%d)", hdr.length, BLYNK_MAX_READBYTES);
        return;
    }

    uint8_t inputBuffer[hdr.length+2];
    if (hdr.length != conn.read(inputBuffer, hdr.length)) {
        BLYNK_LOG("Can't read body");
        return;
    }
    inputBuffer[hdr.length] = 0;
    inputBuffer[hdr.length+1] = 0;

    //Serial.print("Body:");
    //Serial.write(inputBuffer, hdr.length);

    switch (hdr.type)
    {
    case BLYNK_CMD_LOGIN:
        break;
    case BLYNK_CMD_PING:
        break;
    case BLYNK_CMD_HARDWARE: {
        BlynkParam param(inputBuffer, hdr.length+2);
        this->processCmd(param);
    } break;
    default:
        BLYNK_LOG("Invalid header type: %d", hdr.type);
        break;
    }

    // Send response sometimes
    //if (t - lastActivityOut > 1000UL*BLYNK_KEEPALIVE) {
        hdr.type = BLYNK_CMD_RESPONSE;
        hdr.msg_id = htons(hdr.msg_id);
        hdr.length = htons(BLYNK_SUCCESS);
        conn.write(&hdr, sizeof(hdr));
        lastActivityOut = t;
    //}

}

template <class Transp>
bool BlynkProtocol<Transp>::readHeader(BlynkHeader& hdr)
{
    if (sizeof(hdr) != conn.read(&hdr, sizeof(hdr))) {
        BLYNK_LOG("Can't read header");
        return false;
    }
    hdr.msg_id = ntohs(hdr.msg_id);
    hdr.length = ntohs(hdr.length);
    return true;
}

template <class Transp>
inline
void BlynkProtocol<Transp>::sendCmd(uint8_t cmd, const void* data, size_t length)
{
    BlynkHeader hdr;
    hdr.type = cmd;
    hdr.msg_id = htons(12345);
    hdr.length = htons(length);
    conn.write(&hdr, sizeof(hdr));
    conn.write(data, length);
    lastActivityOut = millis();
}

#endif
