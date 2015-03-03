/**
 * @file       BlynkProtocol.h
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
        , lastHeartbeat(0)
        , currentMsgId(0)
    {}
    bool connect();

    void send(const void* data, size_t length) {
        sendCmd(BLYNK_CMD_HARDWARE, data, length);
    }

    void send(const void* data, size_t length, const void* data2, size_t length2) {
        sendCmd(BLYNK_CMD_HARDWARE, data, length, data2, length2);
    }

    void run(void);

private:
    bool readHeader(BlynkHeader& hdr);
    void sendCmd(uint8_t cmd, const void* data, size_t length);
    void sendCmd(uint8_t cmd, const void* data, size_t length, const void* data2, size_t length2);

    uint16_t getNextMsgId();

protected:
    void begin(const char* authkey) {
        this->authkey = authkey;
    }
    void processInput(void);

    Transp& conn;

private:
    const char* authkey;
    unsigned long lastActivityIn;
    unsigned long lastActivityOut;
    unsigned long lastHeartbeat;
    uint16_t currentMsgId;
};

template <class Transp>
bool BlynkProtocol<Transp>::connect()
{
    if (!conn.connect()) {
        return false;
    }

    sendCmd(BLYNK_CMD_LOGIN, authkey, 32);

    BlynkHeader hdr;
    if (!readHeader(hdr)) {
        BLYNK_LOG("Login timeout");
        conn.disconnect();
        return false;
    }

    if (BLYNK_CMD_RESPONSE != hdr.type ||
        BLYNK_SUCCESS != hdr.length)
    {
        BLYNK_LOG("Login failed (code: %d)", hdr.length);
        conn.disconnect();
        delay(5000);
        return false;
    }

    lastHeartbeat = lastActivityIn = lastActivityOut = millis();
    BLYNK_LOG("Blynk v" BLYNK_VERSION " connected");
    return true;
}

template <class Transp>
inline
void BlynkProtocol<Transp>::run(void)
{
    if (!conn.connected()) {
        if (!connect()) {
            return;
        }
    }

    if (conn.available()) {
        processInput();
    }

    unsigned long t = millis();

    if (t - lastActivityIn > (1000UL * BLYNK_HEARTBEAT + BLYNK_TIMEOUT_MS*3)) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Heartbeat timeout (last in: %lu)", lastActivityIn);
#else
        BLYNK_LOG("Heartbeat timeout");
#endif
        conn.disconnect();
    } else if ((t - lastActivityIn > 1000UL * BLYNK_HEARTBEAT ||
               t - lastActivityOut > 1000UL * BLYNK_HEARTBEAT) &&
               t - lastHeartbeat     > BLYNK_TIMEOUT_MS)
    {
        // Send ping if we didn't both send and receive something for BLYNK_HEARTBEAT seconds
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Heartbeat");
#endif
        BlynkHeader hdr;
        hdr.type = BLYNK_CMD_PING;
        hdr.msg_id = htons(getNextMsgId());
        hdr.length = htons(0);
        conn.write(&hdr, sizeof(hdr));
        lastActivityOut = lastHeartbeat = t;
    }
}

template <class Transp>
void BlynkProtocol<Transp>::processInput(void)
{
    BlynkHeader hdr;
    if (!readHeader(hdr))
        return;

    //BLYNK_LOG("Message %d,%d,%d", hdr.type, hdr.msg_id, hdr.length);

    if (hdr.type == BLYNK_CMD_RESPONSE) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Got response: %d", hdr.length);
#endif
        // TODO: return code may indicate App presence
        lastActivityIn = millis();
        return;
    }

    if (hdr.length > BLYNK_MAX_READBYTES) {
        BLYNK_LOG("Packet size (%d) > max allowed (%d)", hdr.length, BLYNK_MAX_READBYTES);
        return;
    }

    uint8_t inputBuffer[hdr.length+1]; // Add 1 to zero-terminate
    if (hdr.length != conn.read(inputBuffer, hdr.length)) {
        BLYNK_LOG("Can't read body");
        return;
    }
    inputBuffer[hdr.length] = 0;

#ifdef BLYNK_DEBUG
    BLYNK_PRINT.write('>');
    BLYNK_PRINT.write(inputBuffer, hdr.length);
    BLYNK_PRINT.println();
#endif

    lastActivityIn = millis();

    switch (hdr.type)
    {
    case BLYNK_CMD_PING:
        hdr.type = BLYNK_CMD_RESPONSE;
        hdr.msg_id = htons(hdr.msg_id);
        hdr.length = htons(BLYNK_SUCCESS);
        conn.write(&hdr, sizeof(hdr));
        lastActivityOut = lastActivityIn;
        break;
    case BLYNK_CMD_HARDWARE: {
        currentMsgId = hdr.msg_id;
        this->processCmd(inputBuffer, hdr.length);
        currentMsgId = 0;
    } break;
    default:
        BLYNK_LOG("Invalid header type: %d", hdr.type);
        break;
    }

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
void BlynkProtocol<Transp>::sendCmd(uint8_t cmd, const void* data, size_t length)
{
    BlynkHeader hdr;
    hdr.type = cmd;
    hdr.msg_id = htons(getNextMsgId());
    hdr.length = htons(length);
    conn.write(&hdr, sizeof(hdr));
    conn.write(data, length);

    lastActivityOut = millis();

#ifdef BLYNK_DEBUG
    BLYNK_PRINT.write('<');
    BLYNK_PRINT.write((uint8_t*)data, length);
    BLYNK_PRINT.println();
#endif
}

template <class Transp>
void BlynkProtocol<Transp>::sendCmd(uint8_t cmd, const void* data, size_t length, const void* data2, size_t length2)
{
    BlynkHeader hdr;
    hdr.type = cmd;
    hdr.msg_id = htons(getNextMsgId());
    hdr.length = htons(length+length2);
    conn.write(&hdr, sizeof(hdr));
    conn.write(data, length);
    conn.write(data2, length2);

    lastActivityOut = millis();

#ifdef BLYNK_DEBUG
    BLYNK_PRINT.write('<');
    BLYNK_PRINT.write((uint8_t*)data, length);
    BLYNK_PRINT.write((uint8_t*)data2, length2);
    BLYNK_PRINT.println();
#endif
}

template <class Transp>
uint16_t BlynkProtocol<Transp>::getNextMsgId()
{
    static uint16_t last = 0;
    if (currentMsgId != 0)
        return currentMsgId;
    if (++last == 0)
        last = 1;
    return last;
}

#endif
