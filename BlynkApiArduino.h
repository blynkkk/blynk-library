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

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();
#ifndef BLYNK_NO_INFO
    if (!strcmp(cmd, "info")) {
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
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, mem, profile_len);
#else
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, profile, profile_len);
#endif
        return;
    }
#endif

    if (++it >= param.end())
        return;
    unsigned pin = it.asInt();
#if defined(analogInputToDigitalPin)
    if (pin == 0 && it.asStr()[0] == 'A') {
        pin = analogInputToDigitalPin(atoi(it.asStr()+1));
    }
    #pragma message("Good! Analog pins can be referenced on this device by name.")
#else
    #warning "analogInputToDigitalPin not defined => Named analog pins will not work"
#endif

#ifndef BLYNK_NO_BUILTIN

    if (!strcmp(cmd, "dr")) {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("dw");
        rsp.add(pin);
        rsp.add(digitalRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } else if (!strcmp(cmd, "ar")) {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("aw");
        rsp.add(pin);
        rsp.add(analogRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } else

#endif

    if (!strcmp(cmd, "vr")) {
        BlynkReq req = { (uint8_t)pin };
        WidgetReadHandler handler;
        if ((handler = GetReadHandler(pin)) &&
            (handler != BlynkWidgetRead))
        {
            handler(req);
        } else {
            BlynkWidgetReadDefault(req);
        }
    } else {

        if (!strcmp(cmd, "vw")) {
            ++it;
            char* start = (char*)it.asStr();
            BlynkParam param2(start, len - (start - (char*)buff));
            BlynkReq req = { (uint8_t)pin };
            WidgetWriteHandler handler;
            if ((handler = GetWriteHandler(pin)) &&
                (handler != BlynkWidgetWrite))
            {
                handler(req, param2);
            } else {
                BlynkWidgetWriteDefault(req, param2);
            }
            return;
        }

#ifndef BLYNK_NO_BUILTIN

        if (!strcmp(cmd, "pm")) {
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
        }

        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        if (!strcmp(cmd, "dw")) {
            //BLYNK_LOG("digitalWrite %d -> %d", pin, it.asInt());
#ifdef ESP8266
            // Disable PWM...
            analogWrite(pin, 0);
#endif
#ifndef BLYNK_MINIMIZE_PINMODE_USAGE
            pinMode(pin, OUTPUT);
#endif
            digitalWrite(pin, it.asInt() ? HIGH : LOW);
        } else if (!strcmp(cmd, "aw")) {
            //BLYNK_LOG("analogWrite %d -> %d", pin, it.asInt());
#ifndef BLYNK_MINIMIZE_PINMODE_USAGE
            pinMode(pin, OUTPUT);
#endif
            analogWrite(pin, it.asInt());
        } else {
            BLYNK_LOG("Invalid HW cmd: %s", cmd);
            static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_RESPONSE, static_cast<Proto*>(this)->currentMsgId, NULL, BLYNK_ILLEGAL_COMMAND);
        }

#endif

    }
}

#endif
