/**
 * @file       BlynkProtocol.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
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
#include <utility/BlynkUtility.h>

template <class Transp>
class BlynkProtocol
    : public BlynkApi< BlynkProtocol<Transp> >
{
    friend class BlynkApi< BlynkProtocol<Transp> >;
public:
    BlynkProtocol(Transp& transp)
        : conn(transp), authkey(NULL)
        , lastActivityIn(0)
        , lastActivityOut(0)
        , lastHeartbeat(0)
#ifdef BLYNK_MSG_LIMIT
        , deltaCmd(0)
#endif
        , currentMsgId(0)
    {}

    bool connected() { return conn.connected(); }

    bool connect();

    bool run(bool avail = false);

    void sendCmd(uint8_t cmd, uint16_t id = 0, const void* data = NULL, size_t length = 0, const void* data2 = NULL, size_t length2 = 0);

private:
    int readHeader(BlynkHeader& hdr);
    uint16_t getNextMsgId();

protected:
    void begin(const char* auth) {
        BLYNK_LOG("Blynk v" BLYNK_VERSION);
        this->authkey = auth;
    }
    bool processInput(void);

    Transp& conn;

private:
    const char* authkey;
    uint32_t lastActivityIn;
    uint32_t lastActivityOut;
    uint32_t lastHeartbeat;
#ifdef BLYNK_MSG_LIMIT
    uint32_t deltaCmd;
#endif
    uint16_t currentMsgId;
};

template <class Transp>
BLYNK_FORCE_INLINE
bool BlynkProtocol<Transp>::connect()
{
    conn.disconnect();
    if (!conn.connect()) {
        ::delay(5000);
        return false;
    }

#ifdef BLYNK_MSG_LIMIT
    deltaCmd = 1000;
#endif

    uint16_t id = getNextMsgId();
    sendCmd(BLYNK_CMD_LOGIN, id, authkey, strlen(authkey));

#ifdef BLYNK_DEBUG
    const unsigned long t = millis();
#endif

    BlynkHeader hdr;
    if (readHeader(hdr) == 0) {
        hdr.length = BLYNK_TIMEOUT;
    }

    if (BLYNK_CMD_RESPONSE != hdr.type ||
        id != hdr.msg_id ||
        (BLYNK_SUCCESS != hdr.length && BLYNK_ALREADY_LOGGED_IN != hdr.length))
    {
        if (BLYNK_TIMEOUT == hdr.length) {
            BLYNK_LOG("Timeout");
        } else if (BLYNK_INVALID_TOKEN == hdr.length) {
            BLYNK_LOG("Invalid auth token");
        } else {
            BLYNK_LOG("Connect failed (code: %d)", hdr.length);
            // Send some invalid headers to server for disconnection
            hdr.type = 255;
            hdr.msg_id = 0;
            hdr.length = 0;
            for (int i=0; i<10; i++) {
                conn.write(&hdr, sizeof(hdr));
            }
        }
        conn.disconnect();
        ::delay(5000);
        return false;
    }

    lastHeartbeat = lastActivityIn = lastActivityOut = millis();
    BLYNK_LOG("Ready!");
#ifdef BLYNK_DEBUG
    BLYNK_LOG("Roundtrip: %dms", lastActivityIn-t);
#endif

    return true;
}

template <class Transp>
bool BlynkProtocol<Transp>::run(bool avail)
{
    if (!conn.connected()) {
        if (!connect()) {
            return false;
        }
    }

    if (avail || conn.available() > 0) {
        //BLYNK_LOG("Available: %d", conn.available());
        //const unsigned long t = micros();
        if (!processInput()) {
            conn.disconnect();
            return false;
        }
        //BLYNK_LOG("Proc time: %d", micros() - t);
    }

    const unsigned long t = millis();

    if (t - lastActivityIn > (1000UL * BLYNK_HEARTBEAT + BLYNK_TIMEOUT_MS*3)) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Heartbeat timeout (last in: %lu)", lastActivityIn);
#else
        BLYNK_LOG("Heartbeat timeout");
#endif
        conn.disconnect();
        return false;
    } else if ((t - lastActivityIn  > 1000UL * BLYNK_HEARTBEAT ||
                t - lastActivityOut > 1000UL * BLYNK_HEARTBEAT) &&
                t - lastHeartbeat   > BLYNK_TIMEOUT_MS)
    {
        // Send ping if we didn't either send or receive something
        // for BLYNK_HEARTBEAT seconds
        sendCmd(BLYNK_CMD_PING);
        lastHeartbeat = t;
    }
    return true;
}

template <class Transp>
BLYNK_FORCE_INLINE
bool BlynkProtocol<Transp>::processInput(void)
{
    BlynkHeader hdr;
    const int ret = readHeader(hdr);

    if (ret == 0) {
        return true; // Considered OK (no data on input)
    }

    if (ret < 0 || hdr.msg_id == 0) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Wrong header on input");
#endif
        return false;
    }

    if (hdr.type == BLYNK_CMD_RESPONSE) {
        lastActivityIn = millis();
        if (BLYNK_NOT_AUTHENTICATED == hdr.length) {
            return false;
        }
        // TODO: return code may indicate App presence
        return true;
    }

    if (hdr.length > BLYNK_MAX_READBYTES) {
#ifdef DEBUG
        BLYNK_LOG("Packet size (%u) > max allowed (%u)", hdr.length, BLYNK_MAX_READBYTES);
#endif
        return false;
    }

    uint8_t inputBuffer[hdr.length+1]; // Add 1 to zero-terminate
    if (hdr.length != conn.read(inputBuffer, hdr.length)) {
#ifdef DEBUG
        BLYNK_LOG("Can't read body");
#endif
        return false;
    }
    inputBuffer[hdr.length] = '\0';

#ifdef BLYNK_DEBUG
    BLYNK_DBG_DUMP(">", inputBuffer, hdr.length);
#endif

    lastActivityIn = millis();

    switch (hdr.type)
    {
    case BLYNK_CMD_PING: {
        sendCmd(BLYNK_CMD_RESPONSE, hdr.msg_id, NULL, BLYNK_SUCCESS);
    } break;
    case BLYNK_CMD_HARDWARE:
    case BLYNK_CMD_BRIDGE: {
        currentMsgId = hdr.msg_id;
        this->processCmd(inputBuffer, hdr.length);
        currentMsgId = 0;
    } break;
    default:
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Invalid header type: %d", hdr.type);
#endif
        return false;
    }

    return true;
}

template <class Transp>
int BlynkProtocol<Transp>::readHeader(BlynkHeader& hdr)
{
    size_t rlen = conn.read(&hdr, sizeof(hdr));
    if (rlen == 0) {
        return 0;
    }

    if (sizeof(hdr) != rlen) {
        return -1;
    }
    hdr.msg_id = ntohs(hdr.msg_id);
    hdr.length = ntohs(hdr.length);
#ifdef BLYNK_DEBUG
    BLYNK_LOG(">msg %d,%u,%u", hdr.type, hdr.msg_id, hdr.length);
#endif
    return rlen;
}

template <class Transp>
void BlynkProtocol<Transp>::sendCmd(uint8_t cmd, uint16_t id, const void* data, size_t length, const void* data2, size_t length2)
{
    if (!conn.connected()) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Cmd not sent");
#endif
        return;
    }

    if (0 == id) {
        id = getNextMsgId();
    }

#ifdef BLYNK_DEBUG
    BLYNK_LOG("<msg %d,%u,%u", cmd, id, length+length2);
#endif

#if defined(ESP8266) || defined(SPARK)
    // Those have more RAM and like single write at a time...

    static uint8_t buff[BLYNK_MAX_READBYTES];
    BlynkHeader* hdr = (BlynkHeader*)buff;
    hdr->type = cmd;
    hdr->msg_id = htons(id);
    hdr->length = htons(length+length2);

    size_t len2s = sizeof(BlynkHeader);
    if (data && length) {
        memcpy(buff + len2s, data, length);
        len2s += length;
    }
    if (data2 && length2) {
        memcpy(buff + len2s, data2, length2);
        len2s += length2;
    }
#ifdef BLYNK_DEBUG
    BLYNK_DBG_DUMP("<", buff+5, len2s-5);
#endif
    size_t wlen = conn.write(buff, len2s);
    if (wlen != len2s) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Sent %u/%u", wlen, len2s);
#endif
        conn.disconnect();
        return;
    }
#else

    BlynkHeader hdr;
    hdr.type = cmd;
    hdr.msg_id = htons(id);
    hdr.length = htons(length+length2);

    size_t wlen = conn.write(&hdr, sizeof(hdr));

    if (cmd != BLYNK_CMD_RESPONSE) {
        if (length) {
#ifdef BLYNK_DEBUG
            BLYNK_DBG_DUMP("<", data, length);
#endif
            wlen += conn.write(data, length);
        }
        if (length2) {
#ifdef BLYNK_DEBUG
            BLYNK_DBG_DUMP("<", data2, length2);
#endif
            wlen += conn.write(data2, length2);
        }

        if (wlen != sizeof(hdr)+length+length2) {
#ifdef BLYNK_DEBUG
            BLYNK_LOG("Sent %u/%u", wlen, sizeof(hdr)+length+length2);
#endif
            conn.disconnect();
            return;
        }
    }

#endif

#ifdef BLYNK_MSG_LIMIT
    const uint32_t ts = millis();
    BlynkAverageSample<32>(deltaCmd, ts - lastActivityOut);
    lastActivityOut = ts;
    //BLYNK_LOG("Delta: %u", deltaCmd);
    if (deltaCmd < (1000/BLYNK_MSG_LIMIT)) {
        BLYNK_LOG_TROUBLE("flood");
        conn.disconnect();
        ::delay(5000);
    }
#else
    lastActivityOut = millis();
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
