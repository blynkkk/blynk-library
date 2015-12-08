/**
 * @file       BlynkApiArduino.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkApiArduino_h
#define BlynkApiArduino_h

#include <Blynk/BlynkApi.h>
#include <Arduino.h>

#if   defined(__AVR_ATmega168__)
    #define BLYNK_INFO_CPU      "ATmega168"
#elif defined(__AVR_ATmega328P__)
    #define BLYNK_INFO_CPU      "ATmega328P"
#elif defined(__AVR_ATmega1280__)
    #define BLYNK_INFO_CPU      "ATmega1280"
#elif defined(__AVR_ATmega1284__)
    #define BLYNK_INFO_CPU      "ATmega1284"
#elif defined(__AVR_ATmega2560__)
    #define BLYNK_INFO_CPU      "ATmega2560"
#elif defined(__AVR_ATmega32U4__)
    #define BLYNK_INFO_CPU      "ATmega32U4"
#elif defined(__SAM3X8E__)
    #define BLYNK_INFO_CPU      "AT91SAM3X8E"
#endif

#ifndef BLYNK_INFO_DEVICE
    #if defined(ENERGIA)
        #define BLYNK_INFO_DEVICE  "Energia"
    #elif defined(SPARK) || defined(PARTICLE)
        #define BLYNK_INFO_DEVICE  "Particle"
    #else
        #define BLYNK_INFO_DEVICE  "Arduino"
    #endif
#endif

template<class Proto>
void BlynkApi<Proto>::Init()
{
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
        BLYNK_PARAM_KV("h-beat" , TOSTRING(BLYNK_HEARTBEAT))
        BLYNK_PARAM_KV("buff-in", TOSTRING(BLYNK_MAX_READBYTES))
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

#ifdef BLYNK_HAS_PROGMEM
    char mem[profile_len];
    memcpy_P(mem, profile, profile_len);
    static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE_INFO, 0, mem, profile_len);
#else
    static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE_INFO, 0, profile, profile_len);
#endif
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
    const uint16_t cmd16 = *(uint16_t*)cmd;

    if (++it >= param.end())
        return;

#if defined(analogInputToDigitalPin)
    // Good! Analog pins can be referenced on this device by name.
    const uint8_t pin = (it.asStr()[0] == 'A') ?
			 analogInputToDigitalPin(atoi(it.asStr()+1)) :
                         it.asInt();
#else
    #warning "analogInputToDigitalPin not defined => Named analog pins will not work"
    const uint8_t pin = it.asInt();
#endif

    switch(cmd16) {

#ifndef BLYNK_NO_BUILTIN

    case BLYNK_HW_PM: {
        while (it < param.end()) {
            ++it;
            //BLYNK_LOG("pinMode %u -> %s", pin, it.asStr());
            if (!strcmp(it.asStr(), "in")) {
                pinMode(pin, INPUT);
            } else if (!strcmp(it.asStr(), "out") || !strcmp(it.asStr(), "pwm")) {
                pinMode(pin, OUTPUT);
#ifdef INPUT_PULLUP
            } else if (!strcmp(it.asStr(), "pu")) {
                pinMode(pin, INPUT_PULLUP);
#endif
#ifdef INPUT_PULLDOWN
            } else if (!strcmp(it.asStr(), "pd")) {
                pinMode(pin, INPUT_PULLDOWN);
#endif
            } else {
#ifdef BLYNK_DEBUG
                BLYNK_LOG("Invalid pinMode %u -> %s", pin, it.asStr());
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

        //BLYNK_LOG("digitalWrite %d -> %d", pin, it.asInt());
#ifdef ESP8266
        // Disable PWM...
        analogWrite(pin, 0);
#endif
#ifndef BLYNK_MINIMIZE_PINMODE_USAGE
        pinMode(pin, OUTPUT);
#endif
        digitalWrite(pin, it.asInt() ? HIGH : LOW);
    } break;
    case BLYNK_HW_AR: {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("aw");
        rsp.add(pin);
        rsp.add(analogRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } break;
    case BLYNK_HW_AW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        //BLYNK_LOG("analogWrite %d -> %d", pin, it.asInt());
#ifndef BLYNK_MINIMIZE_PINMODE_USAGE
        pinMode(pin, OUTPUT);
#endif
        analogWrite(pin, it.asInt());
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
        BLYNK_LOG("Invalid HW cmd: %s", cmd);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_RESPONSE, static_cast<Proto*>(this)->currentMsgId, NULL, BLYNK_ILLEGAL_COMMAND);
    }
}

#endif
