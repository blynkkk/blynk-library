/**
 * @file       BlynkApiWiringPi.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkApiWiringPi_h
#define BlynkApiWiringPi_h

#include <Blynk/BlynkApi.h>
#include <wiringPi.h>

#ifndef BLYNK_INFO_DEVICE
    #define BLYNK_INFO_DEVICE  "Raspberry"
#endif

template<class Proto>
void BlynkApi<Proto>::Init()
{
    wiringPiSetupGpio();
}

template<class Proto>
BLYNK_FORCE_INLINE
millis_time_t BlynkApi<Proto>::getMillis()
{
    return millis();
}

#ifdef BLYNK_NO_INFO

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo() {}

#else

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo()
{
    static const char profile[] BLYNK_PROGMEM =
        BLYNK_PARAM_KV("ver"    , BLYNK_VERSION)
        BLYNK_PARAM_KV("h-beat" , BLYNK_TOSTRING(BLYNK_HEARTBEAT))
        BLYNK_PARAM_KV("buff-in", BLYNK_TOSTRING(BLYNK_MAX_READBYTES))
#ifdef BLYNK_INFO_DEVICE
        BLYNK_PARAM_KV("dev"    , BLYNK_INFO_DEVICE)
#endif
#ifdef BLYNK_INFO_CPU
        BLYNK_PARAM_KV("cpu"    , BLYNK_INFO_CPU)
#endif
#ifdef BLYNK_INFO_CONNECTION
        BLYNK_PARAM_KV("con"    , BLYNK_INFO_CONNECTION)
#endif
        BLYNK_PARAM_KV("build"  , __DATE__ " " __TIME__)
    ;
    const size_t profile_len = sizeof(profile)-1;

    char mem_dyn[32];
    BlynkParam profile_dyn(mem_dyn, 0, sizeof(mem_dyn));
    profile_dyn.add_key("conn", "Socket");

    static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_INTERNAL, 0, profile, profile_len, profile_dyn.getBuffer(), profile_dyn.getLength());
    return;
}

#endif

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();
    uint16_t cmd16;
    memcpy(&cmd16, cmd, sizeof(cmd16));
    if (++it >= param.end())
        return;

#if defined(analogInputToDigitalPin)
    // Good! Analog pins can be referenced on this device by name.
    const uint8_t pin = (it.asStr()[0] == 'A') ?
                         analogInputToDigitalPin(atoi(it.asStr()+1)) :
                         it.asInt();
#else
    #if defined(BLYNK_DEBUG_ALL)
        #pragma message "analogInputToDigitalPin not defined"
    #endif
    const uint8_t pin = it.asInt();
#endif

    switch(cmd16) {

#ifndef BLYNK_NO_BUILTIN

    case BLYNK_HW_PM: {
        while (it < param.end()) {
            ++it;
            if (!strcmp(it.asStr(), "in")) {
                pinMode(pin, INPUT);
                pullUpDnControl(pin, PUD_OFF);
            } else if (!strcmp(it.asStr(), "out")) {
                pinMode(pin, OUTPUT);
            } else if (!strcmp(it.asStr(), "pu")) {
                pinMode(pin, INPUT);
                pullUpDnControl(pin, PUD_UP);
            } else if (!strcmp(it.asStr(), "pd")) {
                pinMode(pin, INPUT);
                pullUpDnControl(pin, PUD_DOWN);
            } else if (!strcmp(it.asStr(), "pwm")) {
                pinMode(pin, PWM_OUTPUT);
            } else {
#ifdef BLYNK_DEBUG
                BLYNK_LOG4(BLYNK_F("Invalid pin "), pin, BLYNK_F(" mode "), it.asStr());
#endif
            }
            ++it;
        }
    } break;
    case BLYNK_HW_DR: {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("dw");
        rsp.add(pin);
        rsp.add(digitalRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } break;
    case BLYNK_HW_DW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        pinMode(pin, OUTPUT);
        digitalWrite(pin, it.asInt() ? HIGH : LOW);
    } break;
    case BLYNK_HW_AW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        pinMode(pin, PWM_OUTPUT);
        pwmWrite(pin, it.asInt());
    } break;

#endif

    case BLYNK_HW_VR: {
        BlynkReq req = { pin };
        WidgetReadHandler handler = GetReadHandler(pin);
        if (handler && (handler != BlynkWidgetRead)) {
            handler(req);
        } else {
            BlynkWidgetReadDefault(req);
        }
    } break;
    case BLYNK_HW_VW: {
        ++it;
        char* start = (char*)it.asStr();
        BlynkParam param2(start, len - (start - (char*)buff));
        BlynkReq req = { pin };
        WidgetWriteHandler handler = GetWriteHandler(pin);
        if (handler && (handler != BlynkWidgetWrite)) {
            handler(req, param2);
        } else {
            BlynkWidgetWriteDefault(req, param2);
        }
    } break;
    default:
        BLYNK_LOG2(BLYNK_F("Invalid HW cmd: "), cmd);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_RESPONSE, static_cast<Proto*>(this)->currentMsgId, NULL, BLYNK_ILLEGAL_COMMAND);
    }
}

#endif
