/**
 * @file       BlynkSimpleUserManaged.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jun 2015
 * @brief
 *
 */

#ifndef BlynkSimpleUserManaged_h
#define BlynkSimpleUserManaged_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "custom"
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>

extern size_t BlynkStreamRead(void* buf, size_t len);
extern size_t BlynkStreamWrite(const void* buf, size_t len);

class BlynkTransportUserDefined
{
public:
    BlynkTransportUserDefined()
        : mConn (false)
    {}

    // IP redirect not available
    void begin(char* h, uint16_t p) {}

    bool connect() {
        return mConn = true;
    }

    void disconnect() { mConn = false; }

    bool connected() {
        return mConn;
    }

    size_t read(void* buf, size_t len) {
        return BlynkStreamRead(buf, len);
    }

    size_t write(const void* buf, size_t len) {
        return BlynkStreamWrite(buf, len);
    }

    size_t available() {
        return 0;
    }

private:
    bool mConn;
};

class BlynkUserDefined
    : public BlynkProtocol<BlynkTransportUserDefined>
{
    typedef BlynkProtocol<BlynkTransportUserDefined> Base;
public:
    BlynkUserDefined(BlynkTransportUserDefined& transp)
        : Base(transp)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
    }

private:
};

static BlynkTransportUserDefined _blynkTransport;
BlynkUserDefined Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
