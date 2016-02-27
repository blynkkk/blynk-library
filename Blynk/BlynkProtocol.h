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
    enum BlynkState {
        CONNECTING,
        CONNECTED,
        DISCONNECTED,
    };

    BlynkProtocol(Transp& transp)
        : conn(transp), authkey(NULL)
#ifdef BLYNK_USE_DIRECT_CONNECT
        , profile(NULL)
#endif
        , lastActivityIn(0)
        , lastActivityOut(0)
        , lastHeartbeat(0)
#ifdef BLYNK_MSG_LIMIT
        , deltaCmd(0)
#endif
        , currentMsgId(0)
        , state(CONNECTING)
    {}

    bool connected() { return state == CONNECTED; }

    bool connect(uint32_t timeout = BLYNK_TIMEOUT_MS*3) {
    	conn.disconnect();
    	state = CONNECTING;
    	uint32_t started = millis();
    	while ((state != CONNECTED) &&
    	       (millis() - started < timeout))
    	{
    		run();
    	}
    	return state == CONNECTED;
    }

    void disconnect() {
        conn.disconnect();
        state = DISCONNECTED;
        BLYNK_LOG("Disconnected");
    }

    bool run(bool avail = false);

#ifdef BLYNK_USE_DIRECT_CONNECT
    void setProfile(const char* json) {
    	profile = json;
    }

    void startSession() {
    	state = CONNECTING;
#ifdef BLYNK_MSG_LIMIT
        deltaCmd = 1000;
#endif
    	currentMsgId = 0;
    	lastHeartbeat = lastActivityIn = lastActivityOut = millis();
    }
#endif

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
#ifdef BLYNK_USE_DIRECT_CONNECT
    const char* profile;
#endif
    uint32_t lastActivityIn;
    uint32_t lastActivityOut;
    uint32_t lastHeartbeat;
#ifdef BLYNK_MSG_LIMIT
    uint32_t deltaCmd;
#endif
    uint16_t currentMsgId;
    BlynkState state;
};

template <class Transp>
bool BlynkProtocol<Transp>::run(bool avail)
{
#if !defined(BLYNK_NO_YIELD)
    yield();
#endif

    if (state == DISCONNECTED) {
        return false;
    }

    const bool tconn = conn.connected();

    if (tconn) {
        if (avail || conn.available() > 0) {
            //BLYNK_LOG("Available: %d", conn.available());
            //const unsigned long t = micros();
            if (!processInput()) {
                conn.disconnect();
// TODO: Only when in direct mode?
#ifdef BLYNK_USE_DIRECT_CONNECT
                state = CONNECTING;
#endif
                //BlynkOnDisconnected();
                return false;
            }
            //BLYNK_LOG("Proc time: %d", micros() - t);
        }
    }

    const unsigned long t = millis();

    if (state == CONNECTED) {
        if (!tconn) {
            state = CONNECTING;
            lastHeartbeat = t;
            //BlynkOnDisconnected();
            return false;
        }

        if (t - lastActivityIn > (1000UL * BLYNK_HEARTBEAT + BLYNK_TIMEOUT_MS*3)) {
    #ifdef BLYNK_DEBUG
            BLYNK_LOG("Heartbeat timeout (last in: %lu)", lastActivityIn);
    #else
            BLYNK_LOG("Heartbeat timeout");
    #endif
            conn.disconnect();
            state = CONNECTING;
            //BlynkOnDisconnected();
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
    } else if (state == CONNECTING) {
#ifndef BLYNK_USE_DIRECT_CONNECT
        if (tconn && (t - lastHeartbeat > BLYNK_TIMEOUT_MS)) {
            BLYNK_LOG("Login timeout");
            conn.disconnect();
            state = CONNECTING;
            return false;
        } else if (!tconn && (t - lastHeartbeat > 5000UL)) {
            conn.disconnect();
            if (!conn.connect()) {
                lastHeartbeat = t;
                return false;
            }

#ifdef BLYNK_MSG_LIMIT
            deltaCmd = 1000;
#endif
            sendCmd(BLYNK_CMD_LOGIN, 1, authkey, strlen(authkey));
            lastHeartbeat = lastActivityOut;
            return true;
        }
#endif
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

#ifndef BLYNK_USE_DIRECT_CONNECT
        if (state == CONNECTING && (1 == hdr.msg_id)) {
            switch (hdr.length) {
            case BLYNK_SUCCESS:
            case BLYNK_ALREADY_LOGGED_IN:
                BLYNK_LOG("Ready (ping: %dms).", lastActivityIn-lastHeartbeat);
                lastHeartbeat = lastActivityIn;
                state = CONNECTED;
                this->sendInfo();
#if !defined(BLYNK_NO_YIELD)
                yield();
#endif
                BlynkOnConnected();
                return true;
            case BLYNK_INVALID_TOKEN:
                BLYNK_LOG("Invalid auth token");
                break;
            default:
                BLYNK_LOG("Connect failed (code: %d)", hdr.length);
            }
            return false;
        }
        if (BLYNK_NOT_AUTHENTICATED == hdr.length) {
            return false;
        }
#endif
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
#ifdef BLYNK_USE_DIRECT_CONNECT
    case BLYNK_CMD_REGISTER:
    case BLYNK_CMD_SAVE_PROF:
    case BLYNK_CMD_ACTIVATE:
    case BLYNK_CMD_DEACTIVATE:
    case BLYNK_CMD_REFRESH:
    	break; // those make no sense in direct mode
    case BLYNK_CMD_LOAD_PROF: {
    	sendCmd(BLYNK_CMD_LOAD_PROF, hdr.msg_id, profile, strlen(profile));
    } break;
    case BLYNK_CMD_GET_TOKEN: {
    	sendCmd(BLYNK_CMD_GET_TOKEN, hdr.msg_id, authkey, strlen(authkey));
    } break;
    case BLYNK_CMD_LOGIN:
    	state = CONNECTED;
    	// Fall-through
#endif
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
    if (0 == id) {
        id = getNextMsgId();
    }

#ifdef BLYNK_DEBUG
    BLYNK_LOG("<msg %d,%u,%u", cmd, id, length+length2);
#endif

    if (!conn.connected() || (cmd != BLYNK_CMD_LOGIN && state != CONNECTED) ) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Cmd skipped");
#endif
        return;
    }


#if defined(BLYNK_SEND_ATOMIC)|| defined(ESP8266) || defined(SPARK) || defined(PARTICLE) || defined(ENERGIA)
    // Those have more RAM and like single write at a time...

    uint8_t buff[BLYNK_MAX_READBYTES]; // TODO: Eliminate constant
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
    size_t wlen = 0;

#ifndef BLYNK_SEND_CHUNK
#define BLYNK_SEND_CHUNK 1024 // Just a big number
#endif

    while (wlen < len2s) {
        const size_t chunk = BlynkMin(size_t(BLYNK_SEND_CHUNK), len2s - wlen);
        const size_t wlentmp = conn.write(buff + wlen, chunk);
    	if (wlentmp == 0) {
#ifdef BLYNK_DEBUG
            BLYNK_LOG("Cmd error");
#endif
            conn.disconnect();
            state = CONNECTING;
            //BlynkOnDisconnected();
            return;
    	}
        wlen += wlentmp;
#ifdef BLYNK_SEND_THROTTLE
        delay(BLYNK_SEND_THROTTLE);
#endif
    }

    if (wlen != len2s) {
#ifdef BLYNK_DEBUG
        BLYNK_LOG("Sent %u/%u", wlen, len2s);
#endif
        conn.disconnect();
        state = CONNECTING;
        //BlynkOnDisconnected();
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
            state = CONNECTING;
            //BlynkOnDisconnected();
            return;
        }
    }

#endif

#if defined BLYNK_MSG_LIMIT && BLYNK_MSG_LIMIT > 0
    const uint32_t ts = millis();
    BlynkAverageSample<32>(deltaCmd, ts - lastActivityOut);
    lastActivityOut = ts;
    //BLYNK_LOG("Delta: %u", deltaCmd);
    if (deltaCmd < (1000/BLYNK_MSG_LIMIT)) {
        BLYNK_LOG_TROUBLE("flood-error");
        conn.disconnect();
        state = CONNECTING;
        //BlynkOnDisconnected();
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
