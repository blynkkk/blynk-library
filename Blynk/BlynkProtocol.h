#ifndef BlynkProtocol_h
#define BlynkProtocol_h

#include <string.h>
#include <stdlib.h>
#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkProtocolDefs.h>
#include <Blynk/BlynkApi.h>
#include <Arduino.h>

template <class Transp>
class BlynkProtocol
    : public BlynkApi< BlynkProtocol<Transp> >
{
public:
    BlynkProtocol(Transp& conn) : conn(conn), authkey(NULL) {}
    int connect();
    void processInput(void);
    void send(uint8_t command, const void* data, int16_t length);

protected:
    Transp& conn;
    const char* authkey;
};

template <class Transp>
inline
int BlynkProtocol<Transp>::connect()
{
    /*conn.read(inputBuffer, 64);
    conn.flush();
    conn.disconnect();
    */
    while(!(conn.connect()));
    {
        delay(1000);
        BLYNK_LOG(".");
    }

    BLYNK_LOG("Connected");
    send(BLYNK_CMD_LOGIN, authkey, 32);
    return 0;
}

template <class Transp>
inline
void BlynkProtocol<Transp>::processInput(void)
{
    BlynkHeader hdr;
    if (sizeof(hdr) != conn.read(&hdr, sizeof(hdr))) {
        BLYNK_LOG("Can't read header");
        return;
    }
    hdr.msg_id = ntohs(hdr.msg_id);
    hdr.length = ntohs(hdr.length);

    //BLYNK_LOG("Message %d,%d,%d", hdr.type, hdr.msg_id, hdr.length);

    if (hdr.type == BLYNK_CMD_RESPONSE)
        return;

    if (hdr.length > BLYNK_MAX_READBYTES) { //! \todo memFree? ^_^
        BLYNK_LOG("Big packet, discarded");
        return;
    }

    uint8_t inputBuffer[hdr.length+2];
    if (hdr.length != conn.read(inputBuffer, hdr.length)) {
        BLYNK_LOG("Can't read bodTransp");
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
    case BLYNK_CMD_HARDWARE: {
        BlynkParam param(inputBuffer, hdr.length+2);
        this->processCmd(param);
    } break;
    default:
        BLYNK_LOG("Invalid header type: %d", hdr.type);
        break;
    }

    hdr.type = BLYNK_CMD_RESPONSE;
    hdr.msg_id = htons(hdr.msg_id);
    hdr.length = htons(BLYNK_SUCCESS);
    conn.write(&hdr, sizeof(hdr));

}

template <class Transp>
inline
void BlynkProtocol<Transp>::send(uint8_t command, const void* data, int16_t length)
{
    BlynkHeader hdr;
    hdr.type = command;
    hdr.msg_id = htons(12345);
    hdr.length = htons(length);
    conn.write(&hdr, sizeof(hdr));
    conn.write(data, length);
}

#endif
