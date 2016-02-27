/**
 * @file       WidgetRTC.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2016
 * @brief
 *
 */

#ifndef WidgetRTC_h
#define WidgetRTC_h

#include <Blynk/BlynkApi.h>

class WidgetRTC
{
public:
    WidgetRTC() {}
    void setVPin(int vPin);
    void onWrite(BlynkReq& request, const BlynkParam& param);
private:
    static unsigned long requestTimeSync();
    static uint8_t mPin;
};

#ifndef WidgetRTC_h_NO_IMPL

#include <Time.h>

inline
void WidgetRTC::setVPin(int vPin)
{
    mPin = vPin;
    setSyncProvider(requestTimeSync);
}

// This is called by Time library when it needs time sync
unsigned long WidgetRTC::requestTimeSync()
{
    Blynk.syncVirtual(mPin); // Request RTC widget update from the server
    return 0;                // Tell the Time library that we'll set it later
}

inline
void WidgetRTC::onWrite(BlynkReq& request, const BlynkParam& param)
{
    const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
    unsigned long blynkTime = param.asLong();

    if ( blynkTime >= DEFAULT_TIME) {   // Check the integer is a valid time (greater than Jan 1 2013)
        setTime(blynkTime);             // Sync Time library clock to the value received from Blynk
        BLYNK_LOG("Time sync: OK");
    }
}

#endif

#endif
